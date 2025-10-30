#include "xemu-wrapper.h"
#include <pybind11/pybind11.h>
#include <cstdint>

struct XemuEmulator {
    bool init() {
        return xemu_init();
    }

    void run() {
        run_one_step();
    }

    void deinit() {
        deinit_xemu();
    }

    pybind11::bytes get_frame_data(int width, int height) {
        uint8_t* frame_data = xemu_get_frame_data(width, height);
        pybind11::bytes result(
            reinterpret_cast<const char*>(frame_data), 
            width * height * 3
        );
        
        xemu_free_frame_data(frame_data);
        return result;
    }
};

PYBIND11_MODULE(xemu_module, m) {
    pybind11::class_<XemuEmulator>(m, "XemuEmulator")
        .def(pybind11::init<>())
        .def("init", &XemuEmulator::init)
        .def("run", &XemuEmulator::run)
        .def("get_frame_data", &XemuEmulator::get_frame_data)
        .def("deinit", &XemuEmulator::deinit);
}