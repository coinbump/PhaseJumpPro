using Godot;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 12/17/22
PORTED TO: C++
*/
namespace PJ
{
    public abstract class SomeKeyframe
    {
        public float time;

        public SomeKeyframe(float time)
        {
            this.time = time;
        }
    }
}
