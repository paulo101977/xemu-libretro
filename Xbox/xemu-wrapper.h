#ifndef XEMU_WRAPPER_H
#define XEMU_WRAPPER_H

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#endif

bool xemu_init(const char* config_path, char* game_path);
void run_one_step(void);
uint8_t* xemu_get_frame_data(int width, int height);
void xemu_free_frame_data(uint8_t* data);
void deinit_xemu();
void xemu_input_controller(const signed short int *buttons);
void load_xemu_ext_snapshots(char* name);
void xemu_pause_unpause();
uint8_t* c_xemu_get_system_memory(void);
int c_xemu_get_system_memory_size(void);
void xemu_save_state(const char *vm_name);
#ifdef __cplusplus
}
#endif
#endif