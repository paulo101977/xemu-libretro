
#include "xemu-precompiled.h"
#include "qemu/bitops.h"        // QEMU_ALIGN_DOWN
#include "qemu/thread.h"        // QemuMutex, QemuRecMutex, qemu_mutex_init
#include "qemu/bitmap.h"        // bitmap.h depende dos dois acima
#include <assert.h>
#include <libretro.h>
#include <hw/xbox/xbox.h>

#ifdef PERF_TEST
static struct retro_perf_callback perf_cb;

#define RETRO_PERFORMANCE_INIT(name)                                                               \
  retro_perf_tick_t current_ticks;                                                                 \
  static struct retro_perf_counter name = {#name};                                                 \
  if (!name.registered)                                                                            \
    perf_cb.perf_register(&(name));                                                                \
  current_ticks = name.total

#define RETRO_PERFORMANCE_START(name) perf_cb.perf_start(&(name))
#define RETRO_PERFORMANCE_STOP(name)                                                               \
  perf_cb.perf_stop(&(name));                                                                      \
  current_ticks = name.total - current_ticks;
#else
#define RETRO_PERFORMANCE_INIT(name)
#define RETRO_PERFORMANCE_START(name)
#define RETRO_PERFORMANCE_STOP(name)
#endif

// namespace Libretro
// {
extern retro_environment_t environ_cb;
static bool widescreen;
static bool g_emuthread_launched = false;
// }  // namespace Libretro

// extern "C" {

// TODO set the environment callback correctly
void retro_set_environment(retro_environment_t cb)
{
//   Libretro::environ_cb = cb;
//   Libretro::Options::SetVariables();
// #ifdef PERF_TEST
//   environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb);
// #endif
}

// TODO set the pixel format correctly based on the emulated system
void retro_init(void)
{
  enum retro_pixel_format xrgb888 = RETRO_PIXEL_FORMAT_XRGB8888;
  // Libretro::environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &xrgb888);
  // pc_machine_init_xbox();
  MachineState _m = {};
  MachineState *machine = &_m;

  xbox_init_common(machine, NULL, NULL);
}

void retro_deinit(void)
{
  g_emuthread_launched = false;
#ifdef PERF_TEST
  perf_cb.perf_log();
#endif
}

void retro_get_system_info(struct retro_system_info* info)
{
  info->need_fullpath = true;
  info->valid_extensions = "iso|xiso|bin|cue|nrg|mdf|img|ccd"; // TODO set valid extensions
  info->library_version = ""; // TODO get the correct version
  info->library_name = "xemu-emu";
  info->block_extract = true;
}

void retro_get_system_av_info(struct retro_system_av_info* info)
{

}

void retro_reset(void)
{

}

void retro_run(void)
{
  
}

size_t retro_serialize_size(void)
{
  return 0;
}

bool retro_serialize(void* data, size_t size)
{
  return true;
}
bool retro_unserialize(const void* data, size_t size)
{
  return true;
}

unsigned retro_get_region(void)
{
  // if (DiscIO::IsNTSC(SConfig::GetInstance().m_region) ||
  //     (Core::System::GetInstance().IsWii() && Config::Get(Config::SYSCONF_PAL60)))
  //   return RETRO_REGION_NTSC;

  return RETRO_REGION_PAL;
}

unsigned retro_api_version()
{
  return RETRO_API_VERSION;
}

size_t retro_get_memory_size(unsigned id)
{
  return 0;
}

void* retro_get_memory_data(unsigned id)
{
  return NULL;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char* code)
{
}
// } // extern "C"
