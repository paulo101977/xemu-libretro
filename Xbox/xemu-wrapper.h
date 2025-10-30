#ifndef XEMU_WRAPPER_H
#define XEMU_WRAPPER_H


#ifdef __cplusplus
#include <cstdint>
extern "C" {
#endif

bool xemu_init(void);
void run_one_step(void);
uint8_t* xemu_get_frame_data(int width, int height);
void xemu_free_frame_data(uint8_t* data);
void deinit_xemu();

#ifdef __cplusplus
}
#endif
#endif