#ifndef TEXTUREMODEL_H
#define TEXTUREMODEL_H

#include <GL/glew.h>

class TextureModel{
public:
    void SetTextureUnit(GLuint t){texture_unit_=t;}
    void SetTexture(GLuint t){texture_ = t;}
    void SetTexturePrim(GLuint t){texturePrim_ = t;}
    void SetTextureDblPrim(GLuint t){textureDblPrim_ = t;}
protected:
    GLuint texture_unit_;
    GLuint texture_;
    GLuint texturePrim_;
    GLuint textureDblPrim_;
};

#endif // TEXTUREMODEL_H
