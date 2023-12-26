using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 2/18/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Run a series of Timed objects in sequence
    /// </summary>
    public class TimedSequence : Updatable
    {
        protected List<SomeTimed> sequence = new();
        protected int index;
        protected bool isFinished;

        public bool IsFinished => isFinished;

        public void Add(SomeTimed timed)
        {
            sequence.Add(timed);
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            if (isFinished) { return; }

            var timeDelta = time.delta;

            while (timeDelta > 0)
            {
                bool isValidIndex = (index >= 0 && index < sequence.Count);
                if (!isValidIndex) { break; }

                var timed = sequence[index];
                float timeSpent = timed.Progress * timed.duration;
                float timeLeft = timed.duration - timeSpent;
                float spendTime = Mathf.Min(timeLeft, timeDelta);
                timed.OnUpdate(new TimeSlice(spendTime));
                timeDelta -= spendTime;

                if (!timed.IsFinished) { break; }
                index++;
            }

            isFinished = index >= sequence.Count;
        }
    }
}
