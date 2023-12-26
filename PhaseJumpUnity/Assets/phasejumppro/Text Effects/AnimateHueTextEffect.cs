using TMPro;
using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/2/23
*/
namespace PJ
{
    /// <summary>
    /// Animates the hue color of a range of text
    /// </summary>
    public class AnimateHueTextEffect : SomeRangesTextEffect
    {
        /// <summary>
        /// Time for a full cyle across all hues
        /// </summary>
        public float cycleTime = 1.0f;

        [Range(0, 1.0f)]
        public float hue = 1.0f;

        [Range(0, 1.0f)]
        public float saturation = 1.0f;

        [Range(0, 1.0f)]
        public float value = 1.0f;

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }

            var newHue = hue;
            newHue += Time.deltaTime / cycleTime;
            newHue %= 1.0f;

            hue = newHue;
            UpdateEffectProperties();
        }

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            if (!TryGetComponent(out TextMeshPro textMesh)) { return; }
            var textInfo = textMesh.textInfo;

            textMesh.ForceMeshUpdate();
            Color32[] vertexColors = textInfo.meshInfo[0].colors32;
            var characterInfoList = new List<TMP_CharacterInfo>(textInfo.characterInfo);

            foreach (var range in ranges)
            {
                var start = range.Start;
                var end = range.End;

                for (int i = start; i <= end; i++)
                {
                    if (i < 0 || i >= characterInfoList.Count) { continue; }
                    int vertexIndex = textInfo.characterInfo[i].vertexIndex;

                    Color32 color = Color.HSVToRGB(hue, saturation, value);
                    vertexColors[vertexIndex] = color;
                    vertexColors[vertexIndex + 1] = color;
                    vertexColors[vertexIndex + 2] = color;
                    vertexColors[vertexIndex + 3] = color;
                }
            }

            textMesh.mesh.colors32 = vertexColors;
        }
    }
}
