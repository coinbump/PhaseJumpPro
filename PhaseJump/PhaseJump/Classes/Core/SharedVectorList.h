#pragma once

#include "Dev.h"
#include "VectorList.h"
#include <span>
#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /**
     Stores either a shared vector via span or a modifiable vector copy

     When dealing with large vectors,  we want to reduce the # of
     times the data gets copied. This allows a span to be shared
     until the point in time where we need to copy the vector in order to modify it

     Example: large mesh triangle lists
     Important: span is just a view into the vector and doesn't hold on to it, make sure you aren't
     letting the shared vector deallocate.
     */
    template <class ValueType>
    class SharedVectorList {
    protected:
        using SpanType = std::span<ValueType const>;
        using Vector = VectorList<ValueType>;

        // FUTURE: support shared_ptr if needed
        std::variant<SpanType, Vector> value;

    public:
        SharedVectorList() {}

        SharedVectorList(SpanType span) :
            value(span) {
            // PJ::Log("SPAN triangles")
        }

        SharedVectorList(Vector const& vector) :
            value(vector) {
            // PJ::Log("COPY triangles");
        }

        /// @return Returns a span view into the vector
        /// Can either be a span a shared vector stored somewhere else or a span for this object's
        /// vector
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

        /// @return Returns true if the stored value is shared
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

        operator std::span<ValueType const>() const {
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
