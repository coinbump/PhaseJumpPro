using UnityEngine;
using UnityEngine.EventSystems;
using PJ;

public class ExampleRotateAnimator : PJ.MonoBehaviour, IPointerClickHandler
{
    protected Animator<float> rotateAnimator;

    public void OnPointerClick(PointerEventData eventData)
    {
        if (null == rotateAnimator || rotateAnimator.IsFinished) {
            updatables.Clear();

            if (TryGetComponent(out Node2D node))
            {
                rotateAnimator = new(
                    new AnimationCurve<float>(0, 360, new FloatValueAnimator()),
                    new(0.3f, AnimationCycleType.Once),
                    new Binding<float>(() => node.Rotation, (float value) => node.Rotation = value)
                );
                updatables.Add(rotateAnimator);
            }
        }
    }
}
