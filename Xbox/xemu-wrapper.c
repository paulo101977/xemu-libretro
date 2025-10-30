#include "xemu-precompiled.h"
#include "xemu-wrapper.h"
#include <GL/glew.h>

#ifdef _WIN32
    #include <windows.h>
    #define GL_GET_PROC_ADDRESS wglGetProcAddress
#else
    #include <GL/glx.h>
    #define GL_GET_PROC_ADDRESS glXGetProcAddress
#endif

static SDL_Window* gl_window = NULL;
static SDL_GLContext gl_context = NULL;
static bool initialized = false;

int16_t* xemu_get_audio_buffer_direct(void) {
    extern int16_t (*capture_audio_buffer(void))[2];
    return (int16_t*)capture_audio_buffer();
}

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
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        100, 100,
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
    printf("glActiveTexture: %p\n", glActiveTexture);
    
    return true;
}

bool xemu_init(void) {
    extern int start_xemu(int argc, char **argv);

    char *argv[] = {
      "xemu",
      "-D", "sync,rcu", 
      "-accel", "tcg,thread=single",
      "-smp", "1",
      "-no-shutdown",
      "-display", "xemu",
      "-audio", "none",
      "-audiodev", "none,id=audio0",
      NULL
    };
    int argc = 15;

    start_xemu(argc, argv);
    if(load_gl_functions()) {
        initialized = true;
    }
    return true;
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


    for (int i = 0; i < width * height * 3; i++) {
        pixels[i] = 0;
    }

    // return black pixels if not initialized
    if (!initialized) {
        return pixels;
    }
    
    GLuint tex = get_texture();
    if (tex == 0) {
        return pixels;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    
    if (status == GL_FRAMEBUFFER_COMPLETE) {
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
    glBindTexture(GL_TEXTURE_2D, 0);

    return pixels;
}

void xemu_free_frame_data(uint8_t* data) {
    if (data) {
        free(data);
    }
}