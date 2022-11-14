using System;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// Each child object added to this GameObject specifies a position to populate a template game object
    /// in the formation.
    /// 
    /// EXAMPLE: line of hearts to show player health (N hearts)
    /// EXAMPLE: V formation of enemies approaching
    /// </summary>
    public class Formation : MonoBehaviour
    {
        public enum Method
        {
            // Adds objects in order
            InOrder,

            // Adds objects at random
            Random
        }

        public Method method;

        public SomeRandom random = new UnityRandom();

        /// <summary>
        /// Objects to populate in formation
        /// </summary>
        public List<GameObject> objectList;

        /// <summary>
        /// Populate the formation
        /// </summary>
        public void Build()
        {
            PopulateToCount(transform.childCount, true);
        }

        /// <summary>
        /// Return a list of objects that have been populated into the formation parents
        /// </summary>
        public List<GameObject> PopulatedObjects()
        {
            List<GameObject> result = new List<GameObject>();
            foreach (Transform child in transform)
            {
                foreach (Transform ct in child)
                {
                    result.Add(ct.gameObject);
                }
            }

            return result;
        }

        protected override void Awake()
        {
        }

        protected override void Start()
        {
            Build();
        }

        protected GameObject TemplateAt(int index)
        {
            int chooseIndex = index % objectList.Count;

            switch (method)
            {
                case Method.Random:
                    chooseIndex = Mathf.RoundToInt(random.Value * (float)(objectList.Count - 1));
                    break;
            }

            return objectList.Count > 0 ? objectList[chooseIndex] : null;
        }

        /// <summary>
        /// Fill out the formation with up to N template objects
        /// </summary>
        public void PopulateToCount(int count, bool removeExtra = true)
        {
            var populatedObjects = PopulatedObjects();

            // Already have enough populated
            if (populatedObjects.Count == count)
            {
                return;
            }
            // Have more than enough, remove some
            else if (populatedObjects.Count > count)
            {
                if (removeExtra)
                {
                    for (int i = 0; i < (populatedObjects.Count - count); i++)
                    {
                        Destroy(populatedObjects[populatedObjects.Count - 1 - i]);
                    }
                }
            }
            // Not enough, add more
            else
            {
                int i = -1;
                int populatedCount = 0;
                foreach (Transform childTransform in transform) {
                    if (populatedCount >= count)
                    {
                        break;
                    }

                    i++;

                    var childObject = childTransform.gameObject;
                    if (childTransform.childCount > 0)
                    {
                        populatedCount++;
                        continue;
                    }

                    GameObject template = TemplateAt(i);
                    if (null == template)
                    {
                        Debug.Log("Error. Missing template for formation");
                        return;
                    }

                    GameObject newObject;

#if UNITY_EDITOR
                    var prefabType = PrefabUtility.GetPrefabAssetType(template);
                    if (prefabType == PrefabAssetType.NotAPrefab)
                    {
                        newObject = Instantiate(template, childObject.transform);
                    }
                    else
                    {
                        // This will return null if the prefab you linked to is in the scene instead of Assets
                        newObject = (UnityEngine.GameObject)PrefabUtility.InstantiatePrefab(template);
                    }
                    newObject.transform.SetParent(childObject.transform);
#else
				    newObject = Instantiate(template, childObject.transform);
#endif

                    newObject.transform.localPosition = Vector3.zero;
                }
            }
        }

        /// <summary>
        /// Remove all populated objects
        /// </summary>
        public void RemoveAll()
        {
            var populatedObjects = PopulatedObjects();

            foreach (GameObject populatedObject in populatedObjects)
            {
#if UNITY_EDITOR
                DestroyImmediate(populatedObject);
#else
					Destroy(populatedObject);
#endif
            }
        }

        protected virtual void OnValidate()
        {
        }

        /// <summary>
        /// Flip the x position of each node that specifies a location for the formation
        /// </summary>
        public void FlipX()
        {
            foreach (Transform child in transform)
            {
                Vector3 position = child.localPosition;
                position.x = -position.x;
                child.localPosition = position;
            }
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            var childObjects = new List<GameObject>();
            foreach (Transform childTransform in transform)
            {
                childObjects.Add(childTransform.gameObject);
            }

            EditorUtils.DrawConnectingLines(childObjects, renderState);
        }
#endif
    }

#if UNITY_EDITOR
    [CustomEditor(typeof(Formation))]
    public class FormationEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            DrawDefaultInspector();

            Formation formation = (Formation)target;
            if (GUILayout.Button("Build"))
            {
                formation.Build();
            }

            if (GUILayout.Button("Remove All"))
            {
                formation.RemoveAll();
            }

            if (GUILayout.Button("Flip X"))
            {
                formation.FlipX();
            }
        }
    }
#endif
}
