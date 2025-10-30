#pragma once


// #include <ctype.h>
#include <stdint.h>
#include <assert.h>
#include <glib.h>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>


#include <qemu/osdep.h>
#include <qemu/compiler.h>

#include <hw/acpi/tpm.h>
#include <sysemu/tpm.h>
#include <hw/i386/pc.h>
#include "qemu/bitops.h"
#include "qemu/thread.h"
#include "qemu/bitmap.h"        
#include <assert.h>
#include "hw/xbox/xbox.h"
#include "qemu/main-loop.h"
#include "xemu-config.h"
#include "hw/virtio/virtio.h"
#include "hw/xbox/nv2a/nv2a.h"