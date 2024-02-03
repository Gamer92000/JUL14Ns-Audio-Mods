#pragma once
#include <string>

#define PLUGIN_API_VERSION 26

/*-------------------------- CHANGE THESE --------------------------*/

// this needs to be unique for each plugin
#define CONFIG_FILE "jul14ns_audio_mods.ini"

#define PLUGIN_NAME "JUL14Ns Audio Mods"
#define PLUGIN_AUTHOR "JUL14N"
#define PLUGIN_DESCRIPTION "Some general improvements for the audio processing."

/*-------------------------- INTERNAL DEFINITIONS --------------------------*/
// don't change this, it is replaced by the build script
#define PLUGIN_VERSION "<version>"

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

#define MAX_STREAM_FILTER 128
#define MAX_RX_CHANNEL 21
#define ADJUSTMENT_BUF 100

#define VOICE_TARGET_GAIN -14
#define SILENCE_TARGET_GAIN -40