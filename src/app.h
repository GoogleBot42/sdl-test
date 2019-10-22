#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

class Application {
public:
    Application(const char *title, int width, int height, bool captureMouse = false, bool allowResize = true);
    virtual ~Application();

protected:
    // Event handling
    virtual void onMouseMovementEvent(int dx, int dy) {}
    virtual void onMouseDown(int posX, int posY, bool left) {}
    virtual void onMouseUp(int posX, int posY, bool left) {}
    virtual void onMouseWheel(int dx, int dy) {}
    virtual void onQuitEvent() { quit(); }
    virtual void onKeyUp(int scancode) {}
    virtual void onKeyDown(int scancode) {}
    virtual bool onWindowResizeEvent(int width, int height) {}

    // Runtime callbacks
    virtual void load() {}; // called after GL+window is initialized
    virtual void update(double dt) {};
    virtual void draw() = 0;

public:
    int runMain();

protected:
    // Management
    void quit() { impendingQuit = true; }

    // Info
    bool keyDown(int scancode);
    int mouseX() { return mPosX; }
    int mouseY() { return mPosY; }
    bool mouseLeft() { return mLeft; }
    bool mouseRight() { return mRight; }
    int windowWidth() { return width; }
    int windowHeight() { return height; }
    float FPS() { return fps; }

private:
    void loopIter();
    void handleEvent(const SDL_Event &event);

#ifdef __EMSCRIPTEN__
    friend void emLoopIterFunc(void *data);
#endif // __EMSCRIPTEN__

    bool impendingQuit;

    // mouse
    int mPosX, mPosY;
    bool mLeft, mRight;

    int width, height;

    SDL_Window* window;
    SDL_GLContext context;

    Uint64 timeLast;
    float fps;

    std::unordered_map<int, bool> keys;
};
