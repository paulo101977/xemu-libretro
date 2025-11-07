#include "xemu_audio.h"

static std::vector<int16_t> g_audio_buffer;
static std::mutex g_audio_mutex;

constexpr size_t CHANNELS = 2;
constexpr size_t BLOCK_FRAMES = 256;


const int16_t* xemu_audio_data() {
    return g_audio_buffer.data();
}

int xemu_audio_samples() {
    return (int)(g_audio_buffer.size()) / 2; 
}

void xemu_audio_clear(void) {
    g_audio_buffer.clear();
}

extern "C" {
    void xemu_audio_push(const int16_t (*data)[CHANNELS]) {
        // std::lock_guard<std::mutex> lock(g_audio_mutex);
        g_audio_buffer.insert(g_audio_buffer.end(), &data[0][0], &data[0][0] + BLOCK_FRAMES * CHANNELS);
        if (g_audio_buffer.size() > 44100 * 10 * CHANNELS)
            g_audio_buffer.erase(g_audio_buffer.begin(), g_audio_buffer.begin() + 44100 * CHANNELS);
    }
}