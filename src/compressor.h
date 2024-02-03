#pragma once
#include "lufs.h"
#include "fixed_map.h"

struct CompressorMetaData {
    float loudness;
    float adjustment;
    float adjustedLoudness;
};

class Compressor {
public:
    Compressor(float attackTime, float releaseTime) :
        attackTime(attackTime), releaseTime(releaseTime),
        meterMap(new FixedSizeMap<int, LufsMeter*>(MAX_RX_CHANNEL)) {}

    CompressorMetaData process(short* input, int channelCount, float targetGain) {
        float averageLUFS = 0;
        for (int i = 0; i < channelCount; i++) {
            LufsMeter* lm = meterMap->get_or_init(i, [] {return new LufsMeter();});
            float lufs = lm->calculate_lufs(input[i]);
            lufs = std::fmax(-69, lufs);
            averageLUFS += lufs;
        }
        averageLUFS /= channelCount;

        float gainAdjustment = targetGain - averageLUFS;
        float scaledGainAdjustment = pow(10, gainAdjustment / 20);
        // limit gain factor to 1.8
        scaledGainAdjustment = std::min(1.8f, scaledGainAdjustment);

        for (int i = 0; i < channelCount; i++) {
            input[i] *= scaledGainAdjustment;
        }

        return {
            averageLUFS,
            scaledGainAdjustment,
            averageLUFS + 10 * std::log10(scaledGainAdjustment),
        };
    }

    //void loudness(double);
    //void envelope(double);
    //void correctedLoudness(double);

private:
    float attackTime;
    float releaseTime;
    FixedSizeMap<int, LufsMeter*> *meterMap;
};