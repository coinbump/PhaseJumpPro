#ifndef PJDATA_H
#define PJDATA_H

#include "Color.h"
#include "Vector2.h"
#include "Rect.h"
#include "Macros.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    struct MallocDataAllocator {
        void* Allocate(uint32_t size) const {
            return malloc(size);
        }
    };

    struct CallocDataAllocator {
        void* Allocate(uint32_t size) const {
            return calloc(1, size);
        }
    };

    /// Some data in memory
    class SomeData {
    public:
        virtual void* DataPointer() const = 0;
        virtual uint32_t DataSize() const = 0;
    };

    /// Data in memory
    template<class Allocator = MallocDataAllocator>
    class Data : public SomeData {
    private:
        /// Disable copy
        Data(Data const&);

    protected:
        void* data = nullptr;

    public:
        using This = Data<Allocator>;

        uint32_t size = 0;

        Data() {
        }

        virtual ~Data()
        {
            if (data) {
                free(data);
            }
        }

        SP<This> Copy() const {
            auto result = MAKE<This>();

            if (size > 0 && nullptr != data) {
                result->Resize(size);
                memcpy(result->data, data, size);
            }

            return result;
        }

        void Resize(uint32_t size) {
            if (data) {
                auto reallocResult = realloc(data, size);
                if (reallocResult) {
                    data = reallocResult;
                    this->size = size;
                }
            } else {
                data = Allocator().Allocate(size);
                this->size = size;
            }
        }

        void Flush() {
            if (nullptr == data) { return; }

            free(data);
            data = nullptr;
            size = 0;
        }

        void* DataPointer() const override { return data; }
        uint32_t DataSize() const override { return size; }

        operator void*() const { return data; }
    };
}

#endif
