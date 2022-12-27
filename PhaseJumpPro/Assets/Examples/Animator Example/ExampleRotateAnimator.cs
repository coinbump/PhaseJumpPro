using UnityEngine;
using UnityEngine.EventSystems;
using PJ;

public class ExampleRotateAnimator : PJ.WorldComponent, IPointerClickHandler
{
    protected CycleAnimator<float> rotateAnimator;

    public void OnPointerClick(PointerEventData eventData)
    {
        if (null == rotateAnimator || rotateAnimator.IsFinished)
        {
            updatables.Clear();

            if (TryGetComponent(out GoNode2D node))
            {
                rotateAnimator = new(
                    new FloatInterpolator(0, 360),
                    new(0.3f, AnimationCycleType.Once),
                    new SetBinding<float>((float value) => node.Rotation = Angle.DegreesAngle(value))
                );
                updatables.Add(rotateAnimator);
            }
        }
    }
}
