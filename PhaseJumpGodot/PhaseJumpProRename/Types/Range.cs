using System;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has Unit tests
 * CODE REVIEW: 12/17/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// A range of values, which tests for inclusiveness
    /// </summary>
    public struct Range<T> where T : IComparable<T>
    {
        public enum Evaluate
        {
            Inclusive,
            Inside,
            Open
        }

        private T start;
        private T end;
        private Evaluate startEvaluate;
        private Evaluate endEvaluate;

        public Range(T start, T end, Evaluate startEvaluate, Evaluate endEvaluate)
        {
            this.start = start;
            this.end = end;
            this.startEvaluate = startEvaluate;
            this.endEvaluate = endEvaluate;
        }

        public static Range<T> InclusiveRange(T start, T end)
        {
            return new Range<T>(start, end, Evaluate.Inclusive, Evaluate.Inclusive);
        }

        public static Range<T> InsideRange(T start, T end)
        {
            return new Range<T>(start, end, Evaluate.Inside, Evaluate.Inside);
        }

        public bool IsInside(T value)
        {
            var startResult = false;
            var endResult = false;

            if (end.CompareTo(start) >= 0)
            {
                switch (startEvaluate)
                {
                    case Evaluate.Inclusive:
                        startResult = value.CompareTo(start) >= 0;
                        break;
                    case Evaluate.Inside:
                        startResult = value.CompareTo(start) > 0;
                        break;
                    case Evaluate.Open:
                        startResult = true;
                        break;
                }

                switch (endEvaluate)
                {
                    case Evaluate.Inclusive:
                        endResult = value.CompareTo(end) <= 0;
                        break;
                    case Evaluate.Inside:
                        endResult = value.CompareTo(end) < 0;
                        break;
                    case Evaluate.Open:
                        endResult = true;
                        break;
                }
            }
            else
            {
                switch (startEvaluate)
                {
                    case Evaluate.Inclusive:
                        startResult = value.CompareTo(start) <= 0;
                        break;
                    case Evaluate.Inside:
                        startResult = value.CompareTo(start) < 0;
                        break;
                    case Evaluate.Open:
                        startResult = true;
                        break;
                }

                switch (endEvaluate)
                {
                    case Evaluate.Inclusive:
                        endResult = value.CompareTo(end) >= 0;
                        break;
                    case Evaluate.Inside:
                        endResult = value.CompareTo(end) > 0;
                        break;
                    case Evaluate.Open:
                        endResult = true;
                        break;
                }
            }

            return startResult && endResult;
        }

        public T Start => start;
        public T End => end;
    }
}
