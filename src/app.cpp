#include "app.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
void emLoopIterFunc(void *data);
#endif

Application::Application(const char *title, int width, int height, bool captureMouse, bool allowResize)
    : impendingQuit(false), mPosX(0), mPosY(0), mLeft(false), mRight(false), width(width), height(height)
{
    // setup SDL2 window and OpenGL 4

    SDL_Init(SDL_INIT_VIDEO);

    // modern opengl
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Enable multisampling
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

    if (captureMouse) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    if (allowResize) {
        SDL_SetWindowResizable(window, SDL_TRUE);
    }

    glewExperimental = GL_TRUE; // modern opengl
    glewInit();

    timeLast = SDL_GetPerformanceCounter();
}

Application::~Application()
{
    SDL_GL_DeleteContext(context);
    SDL_Quit();
}

int Application::runMain()
{
    load();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(emLoopIterFunc, this, 60, 1);
#else
    while (!impendingQuit) {
        loopIter();
    }
#endif // __EMSCRIPTEN__

    return 0;
}

bool Application::keyDown(int scancode)
{
    if (keys.count(scancode) == 0) keys[scancode] = false;
    return keys[scancode];
}

void Application::loopIter()
{
    Uint64 timeNow = SDL_GetPerformanceCounter();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handleEvent(event);
    }

    double perfFreq = SDL_GetPerformanceFrequency();
    double dt = ((double)timeNow - timeLast) / perfFreq;
    fps = 1.0f/(float)dt;

    update(dt);
    draw();

    SDL_GL_SwapWindow(window);

    timeLast = timeNow;
}

void Application::handleEvent(const SDL_Event &event)
{
    switch (event.type) {
    case SDL_QUIT:
        onQuitEvent();
        break;
    case SDL_MOUSEMOTION: {
        mPosX = event.motion.x;
        mPosY = event.motion.y;
        onMouseMovementEvent(event.motion.xrel, event.motion.yrel);
    } break;
    case SDL_MOUSEWHEEL:
        onMouseWheel(event.wheel.x, event.wheel.y);
        break;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN: {
        bool pressed = event.button.state == SDL_PRESSED;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            mLeft = pressed;
            break;
        case SDL_BUTTON_RIGHT:
            mRight = pressed;
            break;
        default:
            return; // we only do left and right buttons right now...
        }
        mPosX = event.button.x;
        mPosY = event.button.y;
        if (pressed)
            onMouseDown(mPosX, mPosY, event.button.button == SDL_BUTTON_LEFT);
        else
            onMouseUp(mPosX, mPosY, event.button.button == SDL_BUTTON_LEFT);
    } break;
    case SDL_KEYUP:
    case SDL_KEYDOWN: {
        bool pressed = event.key.state == SDL_PRESSED;
        keys[event.key.keysym.scancode] = pressed;
        if (pressed)
            onKeyDown(event.key.keysym.scancode);
        else
            onKeyUp(event.key.keysym.scancode);
    } break;
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            width = event.window.data1;
            height = event.window.data2;
            glViewport(0, 0, width, height);
            onWindowResizeEvent(width, height);
        }
    }
}

#ifdef __EMSCRIPTEN__
void emLoopIterFunc(void *data)
{
    static_cast<Application *>(data)->loopIter();
}
#endif // __EMSCRIPTEN__
