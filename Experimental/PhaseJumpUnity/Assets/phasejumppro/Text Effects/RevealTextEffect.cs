using TMPro;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/2/23
*/
namespace PJ
{
    /// <summary>
    /// Reveals characters over time (for animated speech bubbles)
    /// </summary>
    public class RevealTextEffect : SomeTextEffect
    {
        public int revealPerSecond = 10;
        protected float revealCount = 0;
        protected bool isFinished = false;

        public bool IsFinished
        {
            get => isFinished;
            set
            {
                if (isFinished == value) { return; }
                isFinished = value;
                UpdateEffectProperties();
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn || isFinished) { return; }
            if (!TryGetComponent(out TextMeshPro textMesh)) { return; }

            revealCount += time.delta * revealPerSecond;
            if (revealCount >= textMesh.text.Length)
            {
                isFinished = true;
            }

            UpdateEffectProperties();
        }

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            if (!TryGetComponent(out TextMeshPro textMesh)) { return; }

            if (IsOn && !isFinished)
            {
                textMesh.maxVisibleCharacters = (int)revealCount;
            }
            else
            {
                textMesh.maxVisibleCharacters = int.MaxValue;
            }
        }

        public void Finish()
        {
            IsFinished = true;
        }

        public void Reset()
        {
            revealCount = 0;
            isFinished = false;
        }
    }
}
