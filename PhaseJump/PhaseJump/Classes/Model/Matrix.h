#pragma once

#include "Vector2.h"
#include "VectorList.h"
#include <span>
#include <stdio.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    /// Non-templated matrix for collections and matrix types
    class SomeMatrix {
    public:
        virtual ~SomeMatrix() {}

        enum class RotateDirection { Left, Right };
    };

    /// Stores objects in an x-by-y matrix structure
    template <class Cell>
    class Matrix : public SomeMatrix {
    protected:
        VectorList<Cell> cells;
        Vector2Int size;

        std::span<Cell> CellDataAt(Vector2Int loc, size_t count) const {
            auto index = LocToIndex(loc);
            GUARDR(index, {})

            // Check: cells.size is 2, index is 1, max # of items copied is 1 (2 - 1)
            auto maxCount = cells.size() - *index;
            auto minCount = std::min(count, maxCount);
            return std::span<Cell>((Cell*)cells.data() + *index, minCount);
        }

    public:
        Matrix(Vector2Int matrixSize) :
            size(matrixSize) {
            GUARD(size.x > 0 && size.y > 0)
            cells.resize(size.x * size.y);
        }

        size_t Count() const {
            return cells.size();
        }

        int Width() const {
            return size.x;
        }

        int Height() const {
            return size.y;
        }

        Vector2Int Size() const {
            return size;
        }

        /// @return Returns true if the matrix is in a valid state
        bool IsValid() const {
            return size.x > 0 && size.y > 0 && cells.size() == size.x * size.y;
        }

        /// Rotates the matrix in the left or right direction
        void Rotate(RotateDirection direction) {
            auto oldCells = cells;
            auto oldSize = size;

            Vector2Int newSize(size.y, size.x);
            ResizeFast(newSize);

            for (int i = 0; i < oldCells.size(); i++) {
                auto oldY = i / oldSize.x;
                auto oldX = i % oldSize.x;

                Vector2Int newLocation(0, 0);

                switch (direction) {
                case RotateDirection::Left:
                    newLocation = Vector2Int(oldY, Height() - oldX - 1);
                    break;
                case RotateDirection::Right:
                    newLocation = Vector2Int(Width() - 1 - oldY, oldX);
                    break;
                }

                SetCell(newLocation, oldCells[i]);
            }
        }

        /// Resizes the matrix quickly, but might not keep the cells in their original positions
        void ResizeFast(Vector2Int size) {
            GUARD(this->size != size)
            GUARD(size.x > 0 && size.y > 0)

            cells.resize(size.x * size.y);
            this->size = size;
        }

        /// Resizes the matrix and keeps the cells in their original positions
        void ResizeStable(Vector2Int size) {
            GUARD(this->size != size)
            GUARD(size.x > 0 && size.y > 0)

            auto oldSize = this->size;
            auto oldCells = cells;

            size_t vectorSize = size.x * size.y;
            cells = VectorList<Cell>(vectorSize);

            this->size = size;

            GUARD(!IsEmpty(oldCells))

            // FUTURE: update with std::span and std::copy
            Cell* sourceData = oldCells.data();
            size_t sourceLineSize = oldSize.x;
            size_t destLineSize = size.x;

            auto lineCopySize = std::min(sourceLineSize, destLineSize);
            GUARD(lineCopySize > 0)

            auto copyHeight = std::min(oldSize.y, size.y);
            GUARD(copyHeight > 0)

            for (int y = 0; y < copyHeight; y++) {
                memcpy(
                    CellDataAt(Vector2Int(0, y), lineCopySize).data(), sourceData,
                    lineCopySize * sizeof(Cell)
                );
                sourceData += sourceLineSize;
            }
        }

        /// @return Returns true if the matrix location is valid for this matrix
        bool IsValidLocation(Vector2Int loc) const {
            return (loc.x >= 0 && loc.x < Width() && loc.y >= 0 && loc.y < Height());
        }

        /// Converts a matrix location to a cell index
        std::optional<size_t> LocToIndex(Vector2Int loc) const {
            GUARDR(IsValidLocation(loc), {})

            size_t index = loc.y * Width() + loc.x;
            GUARDR(index < cells.size(), {})

            return index;
        }

        /// @return Returns the cell if location is valid.
        /// @throw Throws an exception if c=location is invalid
        Cell& CellAt(Vector2Int loc) {
            auto index = LocToIndex(loc);
            GUARD_THROW(index, std::out_of_range("Invalid cell loc"))

            return cells[*index];
        }

        /// @return Returns the cell if location is valid.
        /// @throw Throws an exception if c=location is invalid
        Cell const& CellAt(Vector2Int loc) const {
            auto index = LocToIndex(loc);
            GUARD_THROW(index, std::out_of_range("Invalid cell loc"))

            return cells[*index];
        }

        /// Sets the cell value
        void SetCell(Vector2Int loc, Cell value) {
            GUARD(IsValidLocation(loc))

            CellAt(loc) = value;
        }

        void SetAllCells(Cell value) {
            GUARD(IsValid())

            cells = VectorList<Cell>(cells.size(), value);
        }
    };

    // C++ has a specialization for vector<bool>. We should avoid using it
    // using MatrixBool = Matrix<bool>;

    using MatrixInt = Matrix<int>;
} // namespace PJ
