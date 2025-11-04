#include "xemu-wrapper.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cstdint>
// #include "qemu/main-loop.h"

namespace py = pybind11;

struct XemuEmulator {
    bool init(const char* config_path) {
        return xemu_init(config_path);
    }

    void run() {
        run_one_step();
    }

    void deinit() {
        deinit_xemu();
    }

    void load_state() {
        load_xemu_ext_snapshots();
    }

    py::memoryview get_system_memory_base() {
        static uint8_t* memory_data = nullptr;
        static size_t memory_size = 0;
        
        if (!memory_data) {
            memory_data = c_xemu_get_system_memory();
            memory_size = c_xemu_get_system_memory_size();
            
            printf("System memory size: %zu bytes\n", memory_size);
        }
        
        if (!memory_data || memory_size == 0) {
            throw std::runtime_error("Memory not available");
        }
        
        return py::memoryview::from_buffer(
            memory_data,
            { memory_size },
            { sizeof(uint8_t) }
        );
    }

    void pause_unpause() {
        xemu_pause_unpause();
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
    
    
    pybind11::bytes get_audio_bytes() {
        int16_t* buffer = xemu_get_audio_buffer_direct();
        int size_bytes = 256 * 2 * sizeof(int16_t);
        
        return pybind11::bytes(
            reinterpret_cast<const char*>(buffer),
            size_bytes
        );
    }
    
    int get_audio_buffer_size() {
        return 256;
    }
    
    int get_audio_samples_per_frame() {
        return 256;
    }

    void update_input_controller(py::object arr_obj) {
        py::buffer buf = py::buffer(arr_obj);
        py::buffer_info info = buf.request();
        

        if (info.format != "B" && info.format != "b") {
            throw std::runtime_error("Expected uint8 array");
        }
        
        if (info.itemsize != sizeof(uint8_t)) {
            throw std::runtime_error("Expected uint8 itemsize");
        }
        
        const uint8_t *buttons = static_cast<const uint8_t *>(info.ptr);
        

        if (info.size == 0) return;
        if (buttons == nullptr) return;
        xemu_input_controller(buttons);
    }
};

PYBIND11_MODULE(xemu_module, m) {
    py::module_::import("numpy");

    py::class_<XemuEmulator>(m, "XemuEmulator")
        .def(py::init<>())
        .def("init", &XemuEmulator::init)
        .def("run", &XemuEmulator::run)
        .def("get_frame_data", &XemuEmulator::get_frame_data)
        .def("deinit", &XemuEmulator::deinit)
        .def("get_audio_bytes", &XemuEmulator::get_audio_bytes)
        .def("get_audio_buffer_size", &XemuEmulator::get_audio_buffer_size)
        .def("get_audio_samples_per_frame", &XemuEmulator::get_audio_samples_per_frame)
        .def("update_input_controller", &XemuEmulator::update_input_controller)
        .def("load_state", &XemuEmulator::load_state)
        .def("toggle_pause", &XemuEmulator::pause_unpause)
        .def("get_system_memory", &XemuEmulator::get_system_memory_base);
}