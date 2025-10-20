#pragma once

#ifndef COMPILING_PER_TARGET
#define COMPILING_PER_TARGET
#define CONFIG_TARGET "i386-softmmu-config-target.h"
#endif



#ifdef CONFIG_KVM
#undef CONFIG_KVM
#endif

#ifndef QEMU_KVM_I386_H
#define QEMU_KVM_I386_H
#endif

#ifndef HW_POISON_H
#define HW_POISON_H
#endif

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <glib.h>
#include <assert.h>
#include <stdint.h>
#include <qemu/compiler.h>
#include <qemu/osdep.h>
#include <qemu/typedefs.h>
// #include <i386/pc.h>


#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#endif