#pragma once

#include "Color.h"
#include "Rect.h"
#include "Utils.h"
#include "Vector2.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/9/23
 */
// TODO: update using std::span, std::copy
// TODO: should this be a template class for the data type inside?
namespace PJ {
    struct MallocDataAllocator {
        void* operator()(size_t size) const {
            // May return null if it fails
            return malloc(size);
        }
    };

    struct CallocDataAllocator {
        void* operator()(size_t size) const {
            // May return null if it fails
            return calloc(1, size);
        }
    };

    /// Non templated data for storing in collections
    class SomeData {
    public:
        virtual ~SomeData() {}

        virtual size_t Size() const = 0;
    };

    /// Data in memory
    template <class Type = void>
    class Data : public SomeData {
    private:
        /// Disable copy
        Data(Data const&);

    protected:
        Type* data = nullptr;
        size_t size = 0;
        std::function<Type*(size_t size)> allocator;

    public:
        using This = Data;

        Data(
            std::function<Type*(size_t size)> allocator = [](size_t size
                                                          ) { return (Type*)malloc(size); }
        ) :
            allocator(allocator) {}

        virtual ~Data() {
            if (data) {
                free(data);
            }
        }

        bool CopyIn(Type* data, size_t size) {
            GUARDR(data != this->data, false)
            GUARDR(data && size > 0, false)

            if (ResizeBytes(size) == size) {
                memcpy(this->data, data, size);
                return true;
            }

            return false;
        }

        SP<This> Copy() const {
            auto result = MAKE<This>();

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
