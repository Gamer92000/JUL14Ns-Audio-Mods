#pragma once

#include <cmath>
#include "buffer.h"

class LufsMeter {
	class Filter {
	private:
		float a1, a2, b0, b1, b2;
		float z1, z2;
	public:
		Filter(float _a1, float _a2, float _b0, float _b1, float _b2) {
			z1 = z2 = 0;
			a1 = _a1;
			a2 = _a2;
			b0 = _b0;
			b1 = _b1;
			b2 = _b2;
		}
		float compute(float x) {
			float z0 = x - a1 * z1 - a2 * z2;
			float result = b0 * z0 + b1 * z1 + b2 * z2;
			z2 = z1;
			z1 = z0;
			return result;
		}
	};

private:
	// stage 1 of the pre-filter to model a spherical head
	Filter stage1Filter{ -1.69065929318241f, 0.73248077421585f, 1.53512485958697f, -2.69169618940638f, 1.19839281085285f };
	// second stage weighting curve
	Filter stage2Filter{ -1.99004745483398f, 0.99007225036621f, 1.0f, -2.0f, 1.0f };

	RingBuffer<float> sampleBuf{480};

public:
	float calculate_lufs(float sample) {
		sample = stage1Filter.compute(sample);
		sample = stage2Filter.compute(sample);
		sampleBuf.push(sample / SHRT_MAX);
		float rms = sampleBuf.accumulate(0, [](float current, std::size_t index, float next) {
			return current + next * next;
		});
		rms /= sampleBuf.size();
		rms = std::sqrt(rms);
		return -0.691 + 10 * std::log10(rms);
	}
};