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
                    new Interpolator<float>(0, 360, new FloatValueInterpolator()),
                    new(0.3f, AnimationCycleType.Once),
                    new Binding<float>(() => node.Rotation.Degrees, (float value) => node.Rotation = Angle.DegreesAngle(value))
                );
                updatables.Add(rotateAnimator);
            }
        }
    }
}
