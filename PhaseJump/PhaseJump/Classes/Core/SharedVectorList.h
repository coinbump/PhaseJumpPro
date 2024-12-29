#pragma once

#include "Dev.h"
#include "VectorList.h"
#include <span>
#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/26/24
 */
namespace PJ {
    /**
     Stores either a shared list via span or a modifiable vector list

     Allows us to either share data from another object to avoid large copies of very large lists
     or store it ourselves if needed. Also allows us to convert the shared span into a modifiable
     copy if we need to modify it from the original source of truth.
     */
    template <class ValueType, class SpanValueType = ValueType const>
    class SharedVectorList {
    protected:
        using SpanType = std::span<SpanValueType>;
        using Vector = VectorList<ValueType>;

        std::variant<SpanType, Vector> value;

    public:
        SharedVectorList() {}

        SharedVectorList(SpanType span) :
            value(span) {}

        SharedVectorList(Vector const& vector) :
            value(vector) {}

        /// @return Returns a span view into the list
        constexpr SpanType Span() const {
            auto span = std::get_if<SpanType>(&value);
            if (span) {
                return *span;
            }

            auto vector = std::get_if<Vector>(&value);
            if (vector) {
                return SpanType{ *vector };
            }

            return {};
        }

        /// @return Returns true if the stored value is a shared span and doesn't hold the owning
        /// list object
        constexpr bool IsShared() const {
            auto span = std::get_if<SpanType>(&value);
            return nullptr != span && nullptr != span->data();
        }

        /// Shared data can't be modified, so if we are using shared data, we'll need
        /// to copy it into the local value
        VectorList<ValueType>& Modifiable() {
            auto span = std::get_if<SpanType>(&value);
            if (span) {
                Vector vector;
                vector.assign(span->begin(), span->end());
                this->value = vector;
            }

            return std::get<Vector>(value);
        }

        size_t size() const {
            return Span().size();
        }

        operator SpanType() const {
            return Span();
        }

        /// @return Returns the span element at index
        /// No range checking is done
        ValueType operator[](size_t index) {
            Assert(index >= 0 && index < Span().size());

            return Span()[index];
        }

        // Unavailable. at requires C++26
        /// @return Returns the span element at index
        /// @throw Throws an error if index is invalid
        //        ValueType At(size_t index) {
        //            return Value().at(index);
        //        }
    };
} // namespace PJ
