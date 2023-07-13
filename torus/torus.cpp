#include "torus.h"
#include "vertices.h"

#include <iostream>
#include <cmath>

Torus::Torus(float init_velocity, float init_angle){
    velocity_ = init_velocity;
    angle_ = init_angle;
    animated_ = true;
}


void Torus::Move(float delta_t){
    if (!animated_) return;
    angle_ += delta_t * velocity_;
    if(angle_>360) angle_ -= 360;
    if(angle_<-360) angle_ += 360;


    // Planeta
    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutY(2 * angle_);


    // Księżyc / Luna
    model_matrix_prim_.SetUnitMatrix();
    model_matrix_prim_.Scale(0.2, 0.2, 0.2);
    model_matrix_prim_.RotateAboutY(angle_);
    model_matrix_prim_.Translate(6, 0, 0);
    model_matrix_prim_.RotateAboutY( -3 * angle_);

}



void Torus::SpeedUp(){
    velocity_ *= 1.09544511501;
}

void Torus::SlowDown(){
    velocity_ /= 1.09544511501;
}

void Torus::ToggleAnimated(){
    animated_ = ! animated_;
}

void Torus::Initialize(int n, int m, float R, float r){
    n_=n;
    m_=m;
    TextureVertex * vertices=new TextureVertex[(m_ + 1)*(n_ + 1)];
    int i,j;
    for (i=0;i<=n_;i++) {
      float phi=(2*M_PI/(float)n_*i - M_PI) / 2;
      for (j=0;j<=m_;j++){
        float theta=2*M_PI/(float)m_*j;
        // vertices[i*(m_ + 1)+j].position[0]=(R + r*cos(phi))*sin(theta);
        // vertices[i*(m_ + 1)+j].position[1]=r*sin(phi);
        // vertices[i*(m_ + 1)+j].position[2]=(R + r*cos(phi))*cos(theta);
        vertices[i*(m_ + 1)+j].position[0]=(R*cos(phi))*sin(theta);
        vertices[i*(m_ + 1)+j].position[1]=R*sin(phi);
        vertices[i*(m_ + 1)+j].position[2]=(R*cos(phi))*cos(theta);
        vertices[i*(m_ + 1)+j].position[3]=1.0f;
        vertices[i*(m_ + 1)+j].texture[0]=(float)j/(float)m_;
        vertices[i*(m_ + 1)+j].texture[1]=(float)i/(float)n_;
      }
    }



    GLuint * indices=new GLuint[2*n_*(m_ + 1)];

    unsigned int k=0;

    for(i=0; i<=n_ - 1; i++){
      for(j=0; j<=m_; j++){
        indices[2*(i*(m_ + 1)+j)]=k;
        indices[2*(i*(m_ + 1)+j)+1]=k+m_+1;
        k++;
      }
    }

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);


    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER,
            (m_+1)*(n_+1)*sizeof(vertices[0]),
            vertices,
            GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].position));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (m_+1)*n_*2*sizeof(GLuint),
                 indices,
                 GL_STATIC_DRAW
                 );
    delete [] vertices;
    delete [] indices;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Torus::Draw(const TextureCameraProgram &program) const{

    glBindVertexArray(vao_);
    glUseProgram(program);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glActiveTexture(texture_unit_);


    // Planeta
    glBindTexture(GL_TEXTURE_2D, texture_);
    program.SetModelMatrix(model_matrix_);
    for (int i=0;i<n_;i++){
      glDrawElements(GL_TRIANGLE_STRIP,
                     2*(m_ + 1),
                     GL_UNSIGNED_INT,
                     (GLvoid*)(sizeof(GLuint) * 2 * i * (m_ + 1))
                     );
    }


    // Księżyc
    glBindTexture(GL_TEXTURE_2D, texturePrim_);
    program.SetModelMatrix(model_matrix_prim_);
    for (int i=0;i<n_;i++){
      glDrawElements(GL_TRIANGLE_STRIP,
                     2*(m_ + 1),
                     GL_UNSIGNED_INT,
                     (GLvoid*)(sizeof(GLuint) * 2 * i * (m_ + 1))
                     );
    }
    
    glDisable(GL_CULL_FACE);

    glBindVertexArray(0);
    glUseProgram(0);


}
