#include "ZQuad.h"

using namespace PJ;

void ZQuad::FlipV() {
    ZQuad quad = *this;
    std::swap(value[0], value[2]);
    std::swap(value[1], value[3]);
}

void ZQuad::FlipH() {
    std::swap(value[0], value[1]);
    std::swap(value[2], value[3]);
}

void ZQuad::TurnRight(int numRotations) {
    // TODO: I think this is wrong?
    numRotations = numRotations % 4;
    for (int i = 0; i < numRotations; i++) {
        ZQuad quad = *this;
        value[0] = quad[1];
        value[1] = quad[3];
        value[2] = quad[0];
        value[3] = quad[2];
    }
}

void ZQuad::TurnLeft(int numRotations) {
    numRotations = numRotations % 4;
    // TODO: I think this is wrong?
    for (int i = 0; i < numRotations; i++) {
        ZQuad quad = *this;

        value[0] = quad[2];
        value[1] = quad[0];
        value[2] = quad[3];
        value[3] = quad[1];
    }
}

void ZQuad::Offset(Vector3 offset) {
    for (int i = 0; i < 4; i++) {
        value[i] += offset;
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
