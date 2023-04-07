using UnityEngine;
using System;
using UnityEditor;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using System.Linq;

/*
 * RATING: 4 stars
 * Needs unit tests, better episolon default values?
 * CODE REVIEW: 4/8/22
 */
namespace PJ
{
    public static class Utils
    {
        public enum RaycastType
        {
            All, First
        }

        public const float FloatEpsilon = 1.192092896e-07F;

        public static bool Equals(float a, float b, float epsilon = FloatEpsilon)
        {
            return Mathf.Abs(Mathf.Abs(a) - Mathf.Abs(b)) < epsilon;
        }

        // FUTURE: need better epsilon default here
        public static bool Equals(double a, double b, double epsilon = .00001)
        {
            return Math.Abs(Math.Abs(a) - Math.Abs(b)) < epsilon;
        }

        public static List<RaycastHit2D> RaycastHits2D(Camera camera, ScreenPosition screenPosition, RaycastType raycastType)
        {
            var result = new List<RaycastHit2D>();

            switch (raycastType)
            {
                case RaycastType.All:
                    return new(AllRaycastHits2D(camera, screenPosition));
                case RaycastType.First:
                    var hit = FirstRaycastHit2D(camera, screenPosition);
                    if (hit)
                    {
                        result.Add(hit);
                    }
                    break;
            }

            return result;
        }

        public static RaycastHit2D[] AllRaycastHits2D(Camera camera, ScreenPosition screenPosition)
        {
            return Physics2D.RaycastAll(camera.ScreenToWorldPoint(screenPosition.position), Vector2.zero);
        }

        public static RaycastHit2D FirstRaycastHit2D(Camera camera, ScreenPosition screenPosition)
        {
            RaycastHit2D hit = Physics2D.Raycast(camera.ScreenToWorldPoint(screenPosition.position), Vector2.zero);
            return hit;
        }

        public static Vector2Int ScreenPixelSize()
        {
#if UNITY_EDITOR
            // The unity editor does not return correct results via Screen.width/.height
            string[] res = UnityStats.screenRes.Split('x');
            return new Vector2Int(int.Parse(res[0]), int.Parse(res[1]));
            // Screen.currentResolution returns the *entire* resolution of the monitor
#else
			return new Vector2Int(Screen.width, Screen.height);
#endif
        }

        public static Vector2 ScreenWorldSize(Camera camera)
        {
            var halfScreenHeight = camera.orthographicSize;
            var screenHeight = halfScreenHeight * 2.0f;

            var ratio = (float)Screen.width / (float)Screen.height;

#if UNITY_EDITOR
            // Screen.width/height aren't useful in the editor, try this
            // TBD: is there a better way?
            if (Application.isEditor && !Application.isPlaying) { ratio = 1920.0f / 1080.0f; }
#endif

            var xSize = screenHeight * ratio;
            var ySize = screenHeight;

            return new Vector2(xSize, ySize);
        }

        public static List<Type> FindObjectsOfTypeIn<Type>(Scene scene)
        {
            List<Type> result = new();

            result = scene.GetRootGameObjects()
                .SelectMany(gameObject => gameObject.GetComponentsInChildren<Type>())
                .ToList();

            return result;
        }
    }
}
