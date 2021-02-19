using UnityEngine;
using System.Collections;

namespace PJ
{
    /// <summary>
    /// Attach to camera for basic camera shake effect.
    ///
    /// NOTE: this might cause issues with a moving camera.
    /// </summary>
    public class CameraShake2D : MonoBehaviour
    {
        // FUTURE: support impulse modulation of camera using noise/oscillators/waves
        public IEnumerator Shake(float magnitude, float duration)
        {
            var position = transform.localPosition;
            float timer = 0;

            while (timer < duration)
            {
                float x = Random.Range(-1.0f, 1.0f) * magnitude;
                float y = Random.Range(-1.0f, 1.0f) * magnitude;

                transform.localPosition = new Vector3(x, y, position.z);
                timer += Time.deltaTime;

                yield return null;
            }
            transform.localPosition = position;
        }
    }

    public class Camera2D
    {
        public static void Shake(float magnitude, float duration = .3f)
        {
            var camera = Camera.main;
            var cameraShake = camera.GetComponent<CameraShake2D>();

            if (null == cameraShake)
            {
                Debug.Log("Main camera needs a CameraShake2D script");
                return;
            }

            cameraShake.StartCoroutine(cameraShake.Shake(magnitude, duration));
        }
    }
}
