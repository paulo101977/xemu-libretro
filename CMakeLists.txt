########################################
# General setup
#
cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0079 NEW) # let target_link_libraries() link to a target defined in a different directory
cmake_policy(SET CMP0080 OLD) # allow using BundleUtilities at configure time

if (POLICY CMP0099)
  cmake_policy(SET CMP0099 NEW) # Propagate INTERFACE_LINK_OPTIONS from private dependencies, used by MacOS framework builds of SDL
endif()

# Weird chicken-and-egg problem: We can't check the compiler before the project() call, but we have to set the policies before it.
# So we do this in two steps: Set the policies if they exist, then error out afterwards if we end up being MSVC and they don't exist.
if (POLICY CMP0117)
  cmake_policy(SET CMP0091 NEW) # MSVC runtime library flags are selected by an abstraction.
  cmake_policy(SET CMP0092 NEW) # MSVC warning flags are not in CMAKE_{C,CXX}_FLAGS by default.
  cmake_policy(SET CMP0117 NEW) # MSVC RTTI flag will not be added by default.
endif()

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW) # MSVC debug information format flags are selected by an abstraction.
endif()

# Minimum OS X version.
# This is inserted into the Info.plist as well.
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0.0" CACHE STRING "")

project(xemu-libretro)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Build type (Release/Debug/RelWithDebInfo/MinSizeRel)" FORCE)
endif()

if (MSVC)
  if (POLICY CMP0117)
    # cmake is a weird language. You can't do if(not POLICY)
  else()
    message(FATAL_ERROR "Please update to CMake 3.20 or higher.")
  endif()

  set(CMAKE_C_STANDARD 99)
  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(COMPILER ${CMAKE_CXX_COMPILER_ID})
if (COMPILER STREQUAL "GNU")
  set(COMPILER "GCC") # prefer printing GCC instead of GNU
endif()

# Enforce minimum compiler versions that support the c++20 features we use
set (GCC_min_version 11)
set (Clang_min_version 14)
set (AppleClang_min_version 14.0.3)
set (min_xcode_version "14.0") # corresponding xcode version for AppleClang_min_version
set (MSVC_min_version 14.32)
set (min_vs_version "2022 17.2.3") # corresponding Visual Studio version for MSVC_min_version
set(LIBRETRO_FOLDER XemuLibretro)
set(XBOX_FOLDER Xbox)

message(STATUS "Using ${COMPILER} ${CMAKE_CXX_COMPILER_VERSION}")

if ("-" STREQUAL "${${COMPILER}_min_version}-")
  message(WARNING "Unknown compiler ${COMPILER}, assuming it is new enough")
else()
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS  ${${COMPILER}_min_version})
    message(FATAL_ERROR "Requires GCC ${GCC_min_version}, Clang ${Clang_min_version},"
    " AppleClang ${AppleClang_min_version} (Xcode ${min_xcode_version}),"
    " or MSVC ${MSVC_min_version} (Visual Studio ${min_vs_version}) or higher")
  endif()
endif()

if(UNIX AND NOT APPLE AND NOT ANDROID)
  option(ENABLE_X11 "Enables X11 Support" ON)
endif()
if(NOT WIN32 AND NOT APPLE AND NOT HAIKU)
  option(ENABLE_EGL "Enables EGL OpenGL Interface" ON)
endif()

if(NOT ANDROID)
  option(ENABLE_CLI_TOOL "Enable xemu-tool, a CLI-based utility for functions such as managing disc images" ON)
endif()


set(USE_SYSTEM_LIBS "AUTO" CACHE STRING "Use system libraries instead of bundled libraries.  ON - Always use system and fail if unavailable, OFF - Always use bundled, AUTO - Use system if available, otherwise use bundled.  Default is AUTO")
if(APPROVED_VENDORED_DEPENDENCIES)
  message(WARNING "APPROVED_VENDORED_DEPENDENCIES is deprecated.  Please migrate to setting USE_SYSTEM_LIBS to ON and setting USE_SYSTEM_<dependency> to either AUTO or OFF to allow bundled libs.")
endif()
option(USE_UPNP "Enables UPnP port mapping support" ON)
option(ENABLE_NOGUI "Enable NoGUI frontend" ON)
option(ENABLE_QT "Enable Qt (Default)" ON)
option(ENABLE_LTO "Enables Link Time Optimization" OFF)
option(ENABLE_GENERIC "Enables generic build that should run on any little-endian host" OFF)
option(ENABLE_HEADLESS "Enables running Xemu as a headless variant" OFF)
option(ENABLE_ALSA "Enables ALSA sound backend" ON)
option(ENABLE_PULSEAUDIO "Enables PulseAudio sound backend" ON)
option(ENABLE_CUBEB "Enables Cubeb sound backend" ON)
option(ENABLE_LLVM "Enables LLVM support, for disassembly" ON)
option(ENABLE_TESTS "Enables building the unit tests" ON)
option(ENABLE_VULKAN "Enables vulkan video backend" OFF)
option(USE_DISCORD_PRESENCE "Enables Discord Rich Presence, show the current game on Discord" ON)
option(USE_MGBA "Enables GBA controllers emulation using libmgba" ON)
option(ENABLE_AUTOUPDATE "Enables support for automatic updates" ON)
option(USE_RETRO_ACHIEVEMENTS "Enables integration with retroachievements.org" ON)
option(LIBRETRO "Build as a libretro library" ON)

# Maintainers: if you consider blanket disabling this for your users, please
# consider the following points:
#  * No data is being sent without explicit user approval (pop up box at first
#    launch).
#  * The xemu team relies on the data in order to understand the behavior
#    of our software in the wild.
option(ENABLE_ANALYTICS "Enables opt-in Analytics collection" ON)

option(ENCODE_FRAMEDUMPS "Encode framedumps in AVI format" ON)

option(ENABLE_GPROF "Enable gprof profiling (must be using Debug build)" OFF)
option(FASTLOG "Enable all logs" OFF)
option(OPROFILING "Enable profiling" OFF)

if (MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG   "/MTd")
   set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()

# TODO: Add DSPSpy
option(DSPTOOL "Build dsptool" OFF)

# RetroAchievements developer tools require Windows hooks
if(WIN32)
  option(RC_CLIENT_SUPPORTS_RAINTEGRATION "Enables RetroAchievements developer tools" ON)
endif()

# Enable SDL by default on operating systems that aren't Android.
if(NOT ANDROID)
  option(ENABLE_SDL "Enables SDL as a generic controller backend" ON)
else()
  option(ENABLE_SDL "Enables SDL as a generic controller backend" OFF)
endif()

if(APPLE)
  enable_language(OBJC)
  enable_language(OBJCXX)
  option(MACOS_USE_DEFAULT_SEARCH_PATH "Don't prioritize system library paths" OFF)
  option(SKIP_POSTPROCESS_BUNDLE "Skip postprocessing bundle for redistributability" OFF)
  # Enable adhoc code signing by default (otherwise makefile builds on ARM will not work)
  option(MACOS_CODE_SIGNING "Enable codesigning" ON)
  option(USE_BUNDLED_MOLTENVK "Build MoltenVK from Externals with xemu-specific patches" ON)
  set(MACOS_CODE_SIGNING_IDENTITY "-" CACHE STRING "The identity used for codesigning.")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  option(ENABLE_VTUNE "Enable Intel VTune integration for JIT code." OFF)

  if(NOT ANDROID)
    option(ENABLE_HWDB "Enables the udev hardware database" ON)
    option(ENABLE_EVDEV "Enables the evdev controller backend" ON)
  endif()
endif()

if(UNIX)
  # Builds a relocatable binary on Linux.
  # The Sys folder will need to be copied to the Binaries folder.
  option(LINUX_LOCAL_DEV "Enable relocatable binary" OFF)
endif()

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake
)


# Enable folders for IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(xemu-libretro SHARED
  ${LIBRETRO_FOLDER}/main.c
  ${XBOX_FOLDER}/hw/xbox/xbox.c
)

if(ENABLE_VULKAN)
  target_sources(xemu-libretro PRIVATE
    ${LIBRETRO_FOLDER}/Vulkan.cpp
  )
endif()

include_directories(${CMAKE_SOURCE_DIR}/Externals/Libretro/Include)

if(ENABLE_VULKAN)
	include_directories(${CMAKE_SOURCE_DIR}/Externals/Vulkan-Headers/include)
	include_directories(${CMAKE_SOURCE_DIR}/Externals/VulkanMemoryAllocator/include)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

target_include_directories(
  xemu-libretro 
  PRIVATE
  ${GLIB2_INCLUDE_DIRS}
  ${XBOX_FOLDER}/build
  ${XBOX_FOLDER}/build/qapi
  ${CMAKE_SOURCE_DIR}/Externals/Libretro/Include
  ${LIBRETRO_FOLDER}/Include
  ${XBOX_FOLDER}
  ${XBOX_FOLDER}/roms
  ${XBOX_FOLDER}/include
  ${XBOX_FOLDER}/include/qemu
  ${XBOX_FOLDER}/hw
  ${XBOX_FOLDER}/include/hw
  ${XBOX_FOLDER}/hw/xbox
  ${XBOX_FOLDER}/target/i386
)

target_precompile_headers(
  xemu-libretro PRIVATE 
  ${LIBRETRO_FOLDER}/xemu-precompiled.h
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set_target_properties(xemu-libretro PROPERTIES PREFIX "")
if(ANDROID)
   target_sources(xemu-libretro PRIVATE
     ../../Android/jni/AndroidCommon/IDCache.cpp
   )
   set_target_properties(xemu-libretro PROPERTIES SUFFIX "_android.so")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
   set(CLANG 1)
endif()

if(NOT MSVC AND NOT CLANG)
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
endif()

if (MSVC)
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DELAYLOAD:OpenGL32.dll /DELAYLOAD:DInput8.dll")
   target_link_libraries(xemu-libretro PRIVATE
     dxguid
     windowsapp
   )
endif()

set(LIBRETRO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Externals/Libretro/Include)

if(MINGW)
  target_link_libraries(xemu-libretro PRIVATE
    common
    setupapi
    ${GLIB2_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/${XBOX_FOLDER}/build
    ${CMAKE_SOURCE_DIR}/${XBOX_FOLDER}/build/qapi
  )

  add_definitions(${GLIB_CFLAGS_OTHER})
endif()

if(MINGW OR LIBRETRO_STATIC)
	target_link_options(xemu-libretro PRIVATE -static-libgcc -static-libstdc++)
endif()


