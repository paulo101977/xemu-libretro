#ifndef __XEMU_AUDIO
#define __XEMU_AUDIO
#include <vector>
#include <mutex>
#include <cstring>
#include <algorithm>
#include <cstdint>

const int16_t* xemu_audio_data();
int xemu_audio_samples();
void xemu_audio_clear(void);
#endif