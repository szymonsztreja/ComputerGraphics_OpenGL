#include "circle.h"

#include <GL/glew.h>
#include <cmath>
#include <iomanip> // for std::setprecision, std::fixed
#include <iostream> // for std::fixed

const int kN = 32;
const float radius = 0.8f;
const float x = 0.0f;
const float y = 0.0f;
const float doublePi = 2.0f * std::acos(-1.0f);
const float angle = doublePi / kN;

void Circle::Initialize(){
    GLfloat kVertices[4 * kN];
    GLfloat kColors[4 * kN];

    std::cout << std::fixed << std::setprecision(2);

    for(int i = 0; i < kN * 4; i+=4){
        int multiplier = i / 4;
        kVertices[i] = x + (radius * cos(multiplier * angle));
        kVertices[i+1] = y + (radius * sin(multiplier * angle));
        // kVertices[i] = radius * sin(angle * multiplier);
        // kVertices[i+1] = -radius * cos(angle * multiplier);

        // z axis not important in this case
        kVertices[i+2] = 0.0f;

        // w component not important in this case
        kVertices[i+3] = 1.0f;
    }
    
    /*
    int color = 26048 % 10;
    n = 8 -> oliwkowy
    */
    
    for (int i = 0; i < kN * 4; i+=4){
        kColors[i] = 0.5f;
        kColors[i+1] = 0.5f;
        kColors[i+2] = 0.0f;
        kColors[i+3] = 1.0f;
    }




    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Circle::~Circle(){
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &color_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
}

void Circle::Draw(const Program &program){

    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_LINE_LOOP, 0, kN);

    glBindVertexArray(0);
    glUseProgram(0);

}
