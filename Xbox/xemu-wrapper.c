#include "xemu-precompiled.h"
#include "xemu-wrapper.h"
#include <GL/glew.h>


static SDL_Window* gl_window = NULL;
static SDL_GLContext gl_context = NULL;
static bool initialized = false;

static bool load_gl_functions(void) {
    printf("Start OpenGL Context...\n");
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    

    gl_window = SDL_CreateWindow(
        "Xemu GL Context",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1, 1,
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
    );
    
    if (!gl_window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }
    
    gl_context = SDL_GL_CreateContext(gl_window);
    if (!gl_context) {
        printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return false;
    }
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW init failed: %s\n", glewGetErrorString(err));
        return false;
    }
    
    printf("Context created...\n");
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLEW version: %s\n", glewGetString(GLEW_VERSION));

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(gl_window);
    
    return true;
}

bool xemu_init(const char* config_path, char* game_path) {
    extern void start_xemu(int argc, char* argv[], char* game_path);

    char *argv[] = {
      "xemu",
      "-D", "sync,rcu", 
      "-accel", "tcg,thread=single",
      "-smp", "1",
      "-no-shutdown",
      "-display", "xemu",
      "-audio", "none",
      "-audiodev", "none,id=audio0",
      "-config_path", config_path,
      NULL
    };
    int argc = 17;

    start_xemu(argc, argv, game_path);
    if(load_gl_functions()) {
        initialized = true;
    }

    return true;
}

void xemu_save_state(const char *vm_name) {
    extern void xemu_snapshots_save(const char *vm_name, Error **err);
    extern void qemu_mutex_lock_main_loop(void);
    extern void bql_unlock();
    extern void qemu_mutex_unlock_main_loop(void);

    qemu_mutex_lock_main_loop();
    bql_lock();

    Error *err = NULL;
    xemu_snapshots_save(vm_name, &err);

    if (err) {
        printf("Error: %s\n", error_get_pretty(err));
        error_free(err);
    }

    bql_unlock();
    qemu_mutex_unlock_main_loop();
}

uint8_t* c_xemu_get_system_memory(void) {
    extern uint8_t* xemu_get_system_memory(void);

    return xemu_get_system_memory();
}

int c_xemu_get_system_memory_size(void) {
    extern int xemu_get_system_memory_size(void);

    return xemu_get_system_memory_size();
}

void xemu_pause_unpause() {
    extern void toogle_pause_vm(void);
    toogle_pause_vm();
}

void load_xemu_ext_snapshots(char *name) {
    extern void xemu_load_snapshot(char *name);

    // TODO load specific state
    xemu_load_snapshot(name);
}

void run_one_step(void) {
      extern void run_step();

      run_step();
}

void deinit_xemu() {
    extern void xemu_deinit();

    xemu_deinit();
}

uint8_t* xemu_get_frame_data(int width, int height) {
    extern GLuint get_texture();
    uint8_t* pixels = (uint8_t*)malloc(width * height * 3);

    // Initialize with black
    memset(pixels, 0, width * height * 3);

    if (!initialized) {
        return pixels;
    }
    
    GLuint tex = get_texture();
    if (tex == 0) {
        return pixels;
    }

    GLint texW, texH;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texW);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texH);

    GLuint fbo_src, fbo_dest;
    GLuint render_tex;
    
    glGenFramebuffers(1, &fbo_src);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_src);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    
    glGenTextures(1, &render_tex);
    glBindTexture(GL_TEXTURE_2D, render_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenFramebuffers(1, &fbo_dest);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_dest);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_tex, 0);
    
    if (texH > height) {
        glBlitFramebuffer(0, texH - height, texW, texH,  // src: top portion
                          0, 0, width, height,           // dst: full
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);
    } else {
        glBlitFramebuffer(0, 0, texW, texH,
                          0, 0, width, height,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_dest);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo_src);
    glDeleteFramebuffers(1, &fbo_dest);
    glDeleteTextures(1, &render_tex);
    glBindTexture(GL_TEXTURE_2D, 0);

    return pixels;
}

void xemu_input_controller(const signed short int *buttons) {
    extern void xemu_input_update_controller_one(const signed short int *buttons);
    xemu_input_update_controller_one(buttons);
}

void xemu_free_frame_data(uint8_t* data) {
    if (data) {
        free(data);
    }
}