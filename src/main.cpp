#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <stdint.h>
#include <fstream>
#include <random>

#include <sys/stat.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "main.hpp"
#include "math.hpp"

#include "rain.hpp"

static const char* vertex_shader_source =
    "#version 330 core\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = projection * model * vec4(aPos, 1.0f);\n"
    "}\n";

static const char* fragment_shader_source =
    "#version 330 core\n"
    "out vec4 finalColor;\n"
    "uniform vec3 color;\n"
    "void main() {\n"
    "    finalColor = vec4(color, 1.0f);\n"
    "}\n";

Game game;

int main()
{
    init_window(&game.window);

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) 
    {
        throw(std::string("Failed to initialize GLAD"));
    }

    unsigned int program = rain::shaderProgram(vertex_shader_source, fragment_shader_source);
    
    unsigned int vao;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    float entity_vertices[] =
    {
        //vertices
         0.5,  0.5, -0.7, //move z back to test perspective proj matrix
        -0.5,  0.5, -0.7,
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
    };

    unsigned int entity_indices[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    unsigned int entity_vbo, entity_ebo;

    glGenBuffers(1, &entity_vbo);
    glGenBuffers(1, &entity_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, entity_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(entity_vertices), entity_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(entity_indices), entity_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while(game.window.running)
    {
        update_window(&game.window);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, entity_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity_ebo);
        
        //TEST SCENE START
        glUseProgram(program);

        rain::Mat model = rain::identity();

        rain::Mat projection = rain::identity();

        rain::ortho(projection, -640.0f, 640.0f, 360.0f, -360.0f, 0.1f, 100.0f);

        rain::scale(model, vector3(32.0f, 32.0f, 1.0f));

        rain::setConstant(program, "model", model);
        rain::setConstant(program, "projection", projection);

        rain::setColor(program, vector3(1.0f, 0.0f, 0.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        model = rain::identity();

        rain::translate(model, vector3(128.0f, -64.0f, 0.0f));
        rain::scale(model, vector3(64.0f, 64.0f, 1.0f));
        rain::rotate(model, 45.0f);

        rain::setConstant(program, "model", model);

        rain::setColor(program, vector3(0.0f, 1.0f, 1.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        model = rain::identity();

        rain::translate(model, vector3(-128.0f, 96.0f, 0.0f));
        rain::scale(model, vector3(64.0f, 64.0f, 1.0f));
        rain::rotate(model, 45.0f);

        rain::setConstant(program, "model", model);

        rain::setColor(program, vector3(1.0f, 1.0f, 0.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);;

        model = rain::identity();

        rain::translate(model, vector3(256.0f, -128.0f, 0.0f));
        rain::scale(model, vector3(128.0f, 128.0f, 1.0f));
        rain::rotate(model, 15.0f);

        rain::setConstant(program, "model", model);

        rain::setColor(program, vector3(0.0f, 1.0f, 0.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        model = rain::identity();

        rain::translate(model, vector3(-256.0f, 128.0f, 0.0f));
        rain::scale(model, vector3(128.0f, 128.0f, 1.0f));
        rain::rotate(model, -35.0f);

        rain::setConstant(program, "model", model);

        rain::setColor(program, vector3(0.0f, 0.5f, 1.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        model = rain::identity();

        rain::translate(model, vector3(512.0f, -256.0f, 0.0f));
        rain::scale(model, vector3(256.0f, 256.0f, 1.0f));
        rain::rotate(model, -15.0f);

        rain::setConstant(program, "model", model);

        rain::setColor(program, vector3(0.7f, 0.0f, 1.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        model = rain::identity();

        rain::translate(model, vector3(-512.0f, 256.0f, 0.0f));
        rain::scale(model, vector3(256.0f, 256.0f, 1.0f));
        rain::rotate(model, 30.0f);

        rain::setConstant(program, "model", model);

        rain::setColor(program, vector3(1.0f, 0.5f, 0.0f));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        //TEST SCENE END
        
        //print matrix
        // std::cout << r.m[0]  << "," << r.m[1]  << "," << r.m[2]  << "," << r.m[3] << std::endl;
        // std::cout << r.m[4]  << "," << r.m[5]  << "," << r.m[6]  << "," << r.m[7] << std::endl;
        // std::cout << r.m[8]  << "," << r.m[9]  << "," << r.m[10] << "," << r.m[11] << std::endl;
        // std::cout << r.m[12] << "," << r.m[13] << "," << r.m[14] << "," << r.m[15] << std::endl;

        SDL_GL_SwapWindow(game.window.window);
    }
    
    SDL_GL_DeleteContext(game.window.context);
    SDL_DestroyWindow(game.window.window);

    return 0;
}