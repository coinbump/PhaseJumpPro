#ifndef PJRANGE_H_
#define PJRANGE_H_

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Specifies a range of values
    template <class T>
        requires std::integral<T> || std::floating_point<T>
    struct Range {
    public:
        enum class Evaluate { Inclusive, Inside, Open };

    protected:
        T start;
        T end;
        Evaluate startEvaluate;
        Evaluate endEvaluate;

    public:
        Range(T start, T end, Evaluate startEvaluate, Evaluate endEvaluate) :
            start(start),
            end(end),
            startEvaluate(startEvaluate),
            endEvaluate(endEvaluate) {}

        static Range<T> InclusiveRange(T start, T end) {
            return Range<T>(start, end, Evaluate::Inclusive, Evaluate::Inclusive);
        }

        static Range<T> InsideRange(T start, T end) {
            return Range<T>(start, end, Evaluate::Inside, Evaluate::Inside);
        }

        bool IsInside(T value) {
            auto startResult = false;
            auto endResult = false;

            if (end >= start) {
                switch (startEvaluate) {
                case Evaluate::Inclusive:
                    startResult = value >= start;
                    break;
                case Evaluate::Inside:
                    startResult = value > start;
                    break;
                case Evaluate::Open:
                    startResult = true;
                    break;
                }

                switch (endEvaluate) {
                case Evaluate::Inclusive:
                    endResult = value <= end;
                    break;
                case Evaluate::Inside:
                    endResult = value < end;
                    break;
                case Evaluate::Open:
                    endResult = true;
                    break;
                }
            } else {
                switch (startEvaluate) {
                case Evaluate::Inclusive:
                    startResult = value <= start;
                    break;
                case Evaluate::Inside:
                    startResult = value < start;
                    break;
                case Evaluate::Open:
                    startResult = true;
                    break;
                }

                switch (endEvaluate) {
                case Evaluate::Inclusive:
                    endResult = value >= end;
                    break;
                case Evaluate::Inside:
                    endResult = value > end;
                    break;
                case Evaluate::Open:
                    endResult = true;
                    break;
                }
            }

            return startResult && endResult;
        }

        T Start() const {
            return start;
        }

        T End() const {
            return end;
        }
    };
} // namespace PJ

#endif
