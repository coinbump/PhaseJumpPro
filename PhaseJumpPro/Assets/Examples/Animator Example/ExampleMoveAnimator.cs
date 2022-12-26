using UnityEngine;
using UnityEngine.EventSystems;
using PJ;

public class ExampleMoveAnimator : PJ.WorldComponent, IPointerClickHandler
{
    protected CycleAnimator<float> moveAnimator;
    protected bool isMoveDown = false;

    public void OnPointerClick(PointerEventData eventData)
    {
        if (null == moveAnimator || moveAnimator.IsFinished)
        {
            isMoveDown = !isMoveDown;
            updatables.Clear();

            var modify = 1.0f;
            if (!isMoveDown)
            {
                modify = -1.0f;
            }

            moveAnimator = new(
                new Interpolator<float>(transform.position.y, -3.0f * modify, new FloatValueInterpolator(), new EaseInOutBack()),
                new(1.0f, AnimationCycleType.Once),
                new Binding<float>(() => transform.position.y, UpdateTransformPosition)
            );
            updatables.Add(moveAnimator);
        }
    }

    public void UpdateTransformPosition(float value)
    {
        // Debug.Log("UpdateTransformPosition: " + value.ToString());
        transform.position = new Vector3(transform.position.x, value, transform.position.z);
    }
}
