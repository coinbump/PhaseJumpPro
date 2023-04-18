#include "ZQuad.h"

using namespace PJ;

void ZQuad::FlipV()
{
    ZQuad quad = *this;
    (*this)[0] = quad[2];
    (*this)[1] = quad[3];
    (*this)[2] = quad[0];
    (*this)[3] = quad[1];
}

void ZQuad::FlipH()
{
    ZQuad quad = *this;
    (*this)[0] = quad[1];
    (*this)[1] = quad[0];
    (*this)[2] = quad[3];
    (*this)[3] = quad[2];
}

void ZQuad::TurnRight(int numRotations)
{
    numRotations = numRotations % 4;
    for (int i = 0; i < numRotations; i++) {
        ZQuad quad = *this;
        (*this)[0] = quad[1];
        (*this)[1] = quad[3];
        (*this)[2] = quad[0];
        (*this)[3] = quad[2];
    }
}

void ZQuad::TurnLeft(int numRotations)
{
    numRotations = numRotations % 4;
    for (int i = 0; i < numRotations; i++) {
        ZQuad quad = *this;

        (*this)[0] = quad[2];
        (*this)[1] = quad[0];
        (*this)[2] = quad[3];
        (*this)[3] = quad[1];
    }
}

void ZQuad::Offset(Vector3 offset)
{
    for (int i = 0; i < 4; i++) {
        (*this)[i] += offset;
    }
}

int ZQuad::VertexIndexToCornerIndex(int index) {
    if (index < 2) {
        return index;
    }
    if (2 == index) {
        return 3;
    }
    return 2;
}

int ZQuad::CornerIndexToVertexIndex(int index) {
    if (index < 2) {
        return index;
    }
    if (2 == index) {
        return 3;
    }
    return 2;
}
