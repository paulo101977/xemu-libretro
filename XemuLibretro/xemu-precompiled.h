#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <glib.h>
#include <assert.h>
#include <stdint.h>
#include <qemu/compiler.h>
#include <qemu/osdep.h>
#include <qemu/typedefs.h>
#include <i386/pc.h>


#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#endif