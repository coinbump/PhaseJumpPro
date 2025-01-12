#pragma once

#include "Color.h"
#include "Rect.h"
#include "Utils.h"
#include "Vector2.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Calls malloc to allocate data
    template <class Type = int8_t>
    struct MallocDataAllocator {
        Type* operator()(size_t size) const {
            // May return null if it fails
            return (Type*)malloc(size);
        }
    };

    /// Calls calloc to allocate data
    template <class Type = int8_t>
    struct CallocDataAllocator {
        Type* operator()(size_t size) const {
            // May return null if it fails
            return (Type*)calloc(1, size);
        }
    };

    /// Non templated data for storing in collections
    class SomeData {
    public:
        virtual ~SomeData() {}

        virtual size_t Size() const = 0;
    };

    /// Data in memory
    template <class Type = int8_t>
    class Data : public SomeData {
    private:
        /// Disables copy
        Data(Data const&);

    protected:
        Type* data = nullptr;
        size_t size = 0;

    public:
        using This = Data;

        std::function<Type*(size_t size)> allocator = [](size_t size) {
            return (Type*)malloc(size);
        };

        Data(
            std::function<Type*(size_t size)> allocator = [](size_t size
                                                          ) { return (Type*)malloc(size); }
        ) :
            allocator(allocator) {}

        Data(std::span<Type> span) {
            auto byteSize = span.size_bytes();
            auto resizeResult = ResizeBytes(byteSize);
            if (resizeResult == byteSize) {
                memcpy(data, span.data(), byteSize);
            }
        }

        Data(std::span<Type const> span) {
            auto byteSize = span.size_bytes();
            auto resizeResult = ResizeBytes(byteSize);
            if (resizeResult == byteSize) {
                memcpy(data, span.data(), byteSize);
            }
        }

        virtual ~Data() {
            if (data) {
                free(data);
            }
        }

        std::span<Type> Span() {
            GUARDR(data && size > 0, {})
            return std::span<Type>(data, Count());
        }

        bool CopyIn(Type* data, size_t size) {
            GUARDR(data != this->data, false)
            GUARDR(data && size > 0, false)

            GUARDR(ResizeBytes(size) == size, false)
            memcpy(this->data, data, size);

            return true;
        }

        UP<This> Copy() const {
            auto result = NEW<This>();

            if (size > 0 && nullptr != data) {
                result->CopyIn(data, size);
            }

            return result;
        }

        void ResizeCount(size_t count) {
            ResizeBytes(count * sizeof(Type));
        }

        size_t ResizeBytes(size_t size) {
            GUARDR(allocator, this->size)
            GUARDR(size != this->size, this->size)

            if (data) {
                Type* reallocResult = (Type*)realloc(data, size);
                if (reallocResult) {
                    data = reallocResult;
                    this->size = size;
                }
            } else {
                data = allocator(size);
                this->size = data ? size : 0;
            }

            return this->size;
        }

        void Flush() {
            GUARD(data)

            free(data);
            data = nullptr;
            size = 0;
        }

        Type* Pointer() const {
            return data;
        }

        size_t Size() const override {
            return size;
        }

        size_t Count() const {
            return size / sizeof(Type);
        }

        operator Type*() const {
            return data;
        }
    };
} // namespace PJ
