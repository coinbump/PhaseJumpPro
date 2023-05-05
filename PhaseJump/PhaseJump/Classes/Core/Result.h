#ifndef PJRESULT_H
#define PJRESULT_H

#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    enum class ResultType {
        Success, Failure
    };

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

        Success SuccessValue() const {
            return std::get<Success>(value);
        }

        Failure FailureValue() const {
            return std::get<Failure>(value);
        }
    };
}

#endif
