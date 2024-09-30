#pragma once

#include <optional>
#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/18/24
 */
namespace PJ {
    enum class ResultType { Success, Failure };

    template <class Success, class Failure>
    class Result {
    protected:
        std::variant<Success, Failure> value;

    public:
        Result(Success success) {
            value = success;
        }

        Result(Failure failure) {
            value = failure;
        }

        ResultType Type() const {
            if (std::holds_alternative<Success>(value)) {
                return ResultType::Success;
            }
            return ResultType::Failure;
        }

        bool IsSuccess() const {
            return Type() == ResultType::Success;
        }

        bool IsFailure() const {
            return Type() == ResultType::Failure;
        }

        std::optional<Success> SuccessValue() const {
            try {
                return std::get<Success>(value);
            } catch (...) {
                return std::optional<Success>();
            }
        }

        std::optional<Failure> FailureValue() const {
            try {
                return std::get<Failure>(value);
            } catch (const std::bad_variant_access& exception) {
                return std::optional<Failure>();
            }
        }
    };
} // namespace PJ
