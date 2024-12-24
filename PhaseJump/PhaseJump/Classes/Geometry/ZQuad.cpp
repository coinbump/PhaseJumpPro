#include "ZQuad.h"

using namespace PJ;

void ZQuad::FlipV() {
    std::swap(value[0], value[2]);
    std::swap(value[1], value[3]);
}

void ZQuad::FlipH() {
    std::swap(value[0], value[1]);
    std::swap(value[2], value[3]);
}

void ZQuad::Turn(int numRotations) {
    if (numRotations < 0) {
        numRotations = -numRotations;
        numRotations = numRotations % 4;
        for (int i = 0; i < numRotations; i++) {
            ZQuad quad = *this;

            value[0] = quad.value[1];
            value[1] = quad.value[3];
            value[2] = quad.value[0];
            value[3] = quad.value[2];
        }
    } else {
        numRotations = numRotations % 4;
        for (int i = 0; i < numRotations; i++) {
            ZQuad quad = *this;
            value[0] = quad.value[2];
            value[1] = quad.value[0];
            value[2] = quad.value[3];
            value[3] = quad.value[1];
        }
    }
}

void ZQuad::Offset(Vector3 offset) {
    value[0] += offset;
    value[1] += offset;
    value[2] += offset;
    value[3] += offset;
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
