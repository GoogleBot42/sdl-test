#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <linmath.h>

#include "glsl.h"
#include "vao.h"
#include "texture.h"
#include "app.h"
#include "camera.h"

float cube_vertices[] = {
    // Position, Color
    -1.0f,  -1.0f,  -1.0f,      0.583f,  0.771f,  0.014f,
    -1.0f,  -1.0f,   1.0f,      0.609f,  0.115f,  0.436f,
    -1.0f,   1.0f,   1.0f,      0.327f,  0.483f,  0.844f,
     1.0f,   1.0f,  -1.0f,      0.822f,  0.569f,  0.201f,
    -1.0f,  -1.0f,  -1.0f,      0.435f,  0.602f,  0.223f,
    -1.0f,   1.0f,  -1.0f,      0.310f,  0.747f,  0.185f,
     1.0f,  -1.0f,   1.0f,      0.597f,  0.770f,  0.761f,
    -1.0f,  -1.0f,  -1.0f,      0.559f,  0.436f,  0.730f,
     1.0f,  -1.0f,  -1.0f,      0.359f,  0.583f,  0.152f,
     1.0f,   1.0f,  -1.0f,      0.483f,  0.596f,  0.789f,
     1.0f,  -1.0f,  -1.0f,      0.559f,  0.861f,  0.639f,
    -1.0f,  -1.0f,  -1.0f,      0.195f,  0.548f,  0.859f,
    -1.0f,  -1.0f,  -1.0f,      0.014f,  0.184f,  0.576f,
    -1.0f,   1.0f,   1.0f,      0.771f,  0.328f,  0.970f,
    -1.0f,   1.0f,  -1.0f,      0.406f,  0.615f,  0.116f,
     1.0f,  -1.0f,   1.0f,      0.676f,  0.977f,  0.133f,
    -1.0f,  -1.0f,   1.0f,      0.971f,  0.572f,  0.833f,
    -1.0f,  -1.0f,  -1.0f,      0.140f,  0.616f,  0.489f,
    -1.0f,   1.0f,   1.0f,      0.997f,  0.513f,  0.064f,
    -1.0f,  -1.0f,   1.0f,      0.945f,  0.719f,  0.592f,
     1.0f,  -1.0f,   1.0f,      0.543f,  0.021f,  0.978f,
     1.0f,   1.0f,   1.0f,      0.279f,  0.317f,  0.505f,
     1.0f,  -1.0f,  -1.0f,      0.167f,  0.620f,  0.077f,
     1.0f,   1.0f,  -1.0f,      0.347f,  0.857f,  0.137f,
     1.0f,  -1.0f,  -1.0f,      0.055f,  0.953f,  0.042f,
     1.0f,   1.0f,   1.0f,      0.714f,  0.505f,  0.345f,
     1.0f,  -1.0f,   1.0f,      0.783f,  0.290f,  0.734f,
     1.0f,   1.0f,   1.0f,      0.722f,  0.645f,  0.174f,
     1.0f,   1.0f,  -1.0f,      0.302f,  0.455f,  0.848f,
    -1.0f,   1.0f,  -1.0f,      0.225f,  0.587f,  0.040f,
     1.0f,   1.0f,   1.0f,      0.517f,  0.713f,  0.338f,
    -1.0f,   1.0f,  -1.0f,      0.053f,  0.959f,  0.120f,
    -1.0f,   1.0f,   1.0f,      0.393f,  0.621f,  0.362f,
     1.0f,   1.0f,   1.0f,      0.673f,  0.211f,  0.457f,
    -1.0f,   1.0f,   1.0f,      0.820f,  0.883f,  0.371f,
     1.0f,  -1.0f,   1.0f,      0.982f,  0.099f,  0.879f
};

class cubeapp : public Application {
public:
    cubeapp() : Application("Cube", 800, 600, true), camera(10, 0.001f, 45, 0.1f, 100) {}
    ~cubeapp() override
    {
        delete cubeVbo;
        delete program;
        delete vao;
        delete texture;
    }

private:
    Camera camera;
    VBO *cubeVbo = nullptr;
    VBO *colorVbo = nullptr;
    GLSLProgram *program = nullptr;
    VAO *vao = nullptr;
    GLTexture *texture = nullptr;

    void load() override
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // create VBOs
        cubeVbo = new VBO;
        cubeVbo->setData(sizeof(cube_vertices), cube_vertices);

        // shader
        program = new GLSLProgram;
        std::string vertexSource = GLSLProgram::readShader("assets/glsl/vertex.glsl");
        std::string fragmentSource = GLSLProgram::readShader("assets/glsl/fragment.glsl");
        program->addShader(GL_VERTEX_SHADER, vertexSource.c_str());
        program->addShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
        program->setFragOutputLocation(0, "outColor");
        program->linkProgram();
        program->bind();

        // create VAO
        vao = new VAO;
        cubeVbo->bind();
        VAOAttribute attributes[] = {
            VAOAttribute(program->getAttributeLocation("position"), GL_FLOAT, 3),
            VAOAttribute(program->getAttributeLocation("color"), GL_FLOAT, 3),
            VAOAttribute::END,
        };
        vao->setAttributePointers(attributes);
        vao->bind();
    }

    void update(double dt) override
    {
        camera.updatePosition(dt);
    }

    void draw() override
    {
        mat4x4 &mvp = camera.calcViewProjection(windowWidth(), windowHeight());
        mat4x4_translate_in_place(mvp, 7, -1, 1);
        program->setMat4(program->getUniformLocation("mvp"), mvp);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
    }

    void onKeyDown(int scancode) override
    {
        if (scancode == SDL_SCANCODE_ESCAPE) quit();
        camera.handleKeys(scancode, true);
    }

    void onKeyUp(int scancode) override
    {
        camera.handleKeys(scancode, false);
    }

    void onMouseMovementEvent(int dx, int dy) override
    {
        camera.handleMouse(dx, dy);
    }
};

int main(int argc, char *argv[])
{
    return cubeapp().runMain();
}

