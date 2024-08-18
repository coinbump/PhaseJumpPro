#pragma once

#include "Vector2.h"
#include "VectorList.h"
#include <stdio.h>

// TODO: WORK IN PROGRESS
namespace PJ {
    class SomeMatrix {
    public:
        virtual ~SomeMatrix() {}

        enum class RotateDirection { Left, Right };
    };

    /// Stores objects in an X,Y matrix structure (Y tuples of X cells)
    /// <typeparam name="T">Type of thing to store</typeparam>
    template <class T>
    class Matrix : public SomeMatrix {
    public:
        using Tuple = VectorList<T>;

    protected:
        VectorList<Tuple> tuples;
        Vector2Int size;

    public:
        int Width() const {
            return size.x;
        }

        int Height() const {
            return size.y;
        }

        Vector2Int Size() const {
            return size;
        }

        Matrix(Vector2Int matrixSize) {
            Vector2Int size(std::max(1, matrixSize.x), std::max(1, matrixSize.y));
            this->size = size;

            for (int i = 0; i < size.y; i++) {
                tuples.Add(Tuple(size.x));
            }
        }

        Tuple* TupleAt(int index) {
            return index < tuples.size() ? &tuples[index] : nullptr;
        }

        void Rotate(RotateDirection direction) {
            VectorList<T> cells;
            VectorList<Vector2Int> locations;

            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    Vector2Int location(x, y);
                    auto myCell = CellAt(location);
                    cells.Add(myCell);

                    locations.Add(location);
                }
            }

            auto oldWidth = Width();
            auto oldHeight = Height();

            Vector2Int newSize(size.y, size.x);
            Resize(newSize);

            for (int i = 0; i < cells.size(); i++) {
                auto oldLocation = locations[i];
                auto cell = cells[i];

                Vector2Int newLocation(0, 0);

                switch (direction) {
                case RotateDirection::Left:
                    newLocation = Vector2Int(oldLocation.y, Height() - oldLocation.x - 1);
                    break;
                case RotateDirection::Right:
                    newLocation = Vector2Int(Width() - 1 - oldLocation.y, oldLocation.x);
                    break;
                }

                SetCell(newLocation, cell);
            }
        }

        void Resize(Vector2Int newSize) {
            auto newWidth = newSize.x;
            auto newHeight = newSize.y;

            if (newWidth < 1 || newHeight < 1) {
                return;
            }

            // Same size.
            if (newWidth == Width() && newHeight == Height()) {
                return;
            }

            // Change width of existing tuples first.
            int y = 0;
            for (auto& tuple : tuples) {
                // Don't bother resizing tuples that are going to be deleted if
                // the grid is shrinking.
                if (y >= newHeight) {
                    break;
                }

                tuple.resize(newWidth);

                y++;
            }

            // Add new tuples
            while (tuples.size() < newHeight) {
                tuples.Add(Tuple(newWidth));
            }

            // Remove deleted tuples
            if (tuples.size() > newHeight) {
                tuples.resize(newHeight);
            }

            size.x = newWidth;
            size.y = newHeight;
        }

        bool IsValidLocation(Vector2Int loc) {
            return (loc.x >= 0 && loc.x < Width() && loc.y >= 0 && loc.y < Height());
        }

        virtual T CellAt(Vector2Int loc) {
            if (!IsValidLocation(loc)) {
                return T();
            }

            return tuples[loc.y][loc.x];
        }

        virtual void SetCell(Vector2Int loc, T content) {
            if (!IsValidLocation(loc)) {
                return;
            }

            tuples[loc.y][loc.x] = content;
        }
    };

    /// Matrix of bools
    class MatrixBool : public Matrix<bool> {
    public:
        using Base = Matrix<bool>;

        MatrixBool(Vector2Int matrixSize) :
            Base(matrixSize) {}
    };

    /// Matrix of ints
    class MatrixInt : public Matrix<int> {
    public:
        using Base = Matrix<int>;

        MatrixInt(Vector2Int matrixSize) :
            Base(matrixSize) {}
    };
} // namespace PJ
