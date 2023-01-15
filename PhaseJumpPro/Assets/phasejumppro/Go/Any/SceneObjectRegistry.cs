using UnityEngine;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine.UIElements;

/*
 * RATING: 5 stars
 * It works
 * CODE REVIEW: 4/9/22
 */
namespace PJ
{
    public class SceneObjectRegistry : PJ.WorldComponent
    {
        [System.Serializable]
        public struct Item<T>
        {
            public string id;
            public T @object;
        }

        public List<Item<GameObject>> gameObjectItems;
        public List<Item<AudioClip>> audioClipItems;

        /// <summary>
        /// Internal registry for id -> GameObject (or Prefab) map
        /// </summary>
        protected Dictionary<string, GameObject> gameObjectRegistry = new Dictionary<string, GameObject>();
        protected Dictionary<string, AudioClip> audioClipRegistry = new Dictionary<string, AudioClip>();

        public Dictionary<string, GameObject> GameObjectRegistry => gameObjectRegistry;
        public Dictionary<string, AudioClip> AudioClipRegistry => audioClipRegistry;

        /// <summary>
        /// Instantiate a game object if a prefab exists in the registry
        /// </summary>
        public GameObject InstantiateGameObject(string id, Vector3 position, Quaternion rotation)
        {
            if (TryGetRegisteredGameObject(id, out GameObject gameObject))
            {
                return Instantiate(gameObject, position, rotation);
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Return a registered game object prefab if it exists in the registry
        /// </summary>
        public GameObject RegisteredGameObject(string id)
        {
            if (TryGetRegisteredGameObject(id, out GameObject gameObject))
            {
                return gameObject;
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Send out a registered game object prefab if it exists in the registry
        /// </summary>
        public bool TryGetRegisteredGameObject(string id, out GameObject gameObject)
        {
            var result = gameObjectRegistry.TryGetValue(id, out gameObject);

            if (!result)
            {
                Debug.Log("Missing from registry: " + id);
            }

            return result;
        }

        protected override void Awake()
        {
            RegisterItems(gameObjectItems, gameObjectRegistry);
            RegisterItems(audioClipItems, audioClipRegistry);
        }

        protected void RegisterItems<T>(List<Item<T>> items, Dictionary<string, T> registry)
        {
            var anyMissingIds = false;
            foreach (Item<T> item in items)
            {
                if (item.id.Length == 0)
                {
                    anyMissingIds = true;
                    continue;
                }

                registry[item.id] = item.@object;
            }

            if (anyMissingIds)
            {
                Debug.Log("SceneObjectRegistry: Missing id");
            }
        }
    }
}
