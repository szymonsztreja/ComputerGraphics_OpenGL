#ifndef MOVABLEMODEL_H
#define MOVABLEMODEL_H

#include "matma.h"


class MovableModel{
protected:
    Mat4 model_matrix_;
    Mat4 model_matrix_prim_;
};

#endif // MOVABLEMODEL_H
