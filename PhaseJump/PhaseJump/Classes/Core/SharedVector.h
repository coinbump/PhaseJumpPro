#pragma once

#include "Dev.h"
#include "VectorList.h"
#include <span>
#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/10/24
 */
namespace PJ {
    /**
     Stores either a shared vector via span or a modifiable vector copy

     When dealing with large vectors,  we want to reduce the # of
     times the data gets copied. This allows a span to be shared
     until the point in time where we need to copy the vector in order to modify it

     Example: large vertex lists
     Important: span is just a view into the vector and doesn't hold on to it, make sure you aren't
     letting the shared vector deallocate.
     */
    template <class ValueType>
    class SharedVector {
    protected:
        bool isModifiable = false;

        // FUTURE: evaluate using std::monostate if needed
        using Span = std::span<ValueType const>;
        using Vector = VectorList<ValueType>;

        // FUTURE: support shared_ptr if needed
        std::variant<Span, Vector> value;

    public:
        SharedVector() :
            isModifiable(false) {}

        SharedVector(Span span) :
            value(span),
            isModifiable(false) {} // PJ::Log("SPAN triangles") }

        SharedVector(Vector const& vector) :
            value(vector),
            isModifiable(true) {
            //            PJ::Log("COPY triangles");
        }

        /// @return Returns a span view into the vector
        /// Can either be a span a shared vector stored somewhere else or a span for this object's
        /// vector
        constexpr Span Value() const {
            auto span = std::get_if<Span>(&value);
            if (span) {
                return *span;
            }

            auto vector = std::get_if<Vector>(&value);
            if (vector) {
                return Span{ *vector };
            }

            return {};
        }

        /// @return Returns true if the stored value is inherantly modifiable (not shared)
        constexpr bool IsModifiable() const {
            return isModifiable;
        }

        /// @return Returns true if the stored value is shared
        constexpr bool IsShared() const {
            auto span = std::get_if<Span>(&value);
            return nullptr != span && nullptr != span->data();
        }

        /// Shared data can't be modified, so if we are using shared data, we'll need
        /// to copy it into the local value
        VectorList<ValueType>& Modifiable() {
            if (!isModifiable) {
                isModifiable = true;

                auto vector = std::get_if<Vector>(&value);
                if (vector) {
                    PJ::Log("Error. Storing Vector with isModifiable set to false");
                }

                auto span = std::get_if<Span>(&value);
                if (span) {
                    Vector vector;
                    vector.assign(span->begin(), span->end());
                    this->value = vector;
                }
            }

            return std::get<Vector>(value);
        }

        size_t size() const {
            return Value().size();
        }

        operator std::span<ValueType const>() const {
            return Value();
        }

        /// @return Returns the span element at index
        /// No range checking is done
        ValueType operator[](size_t index) {
            Assert(index >= 0 && index < Value().size());

            return Value()[index];
        }

        /// @return Returns the span element at index
        /// Throws an error if index is invalid
        ValueType at(size_t index) {
            return Value().at(index);
        }
    };
} // namespace PJ
