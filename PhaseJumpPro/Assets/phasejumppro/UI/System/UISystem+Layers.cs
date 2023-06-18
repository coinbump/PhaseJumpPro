using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.SceneManagement;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    public enum OrderPosition
    {
        Before, After
    }

    public partial class UISystem
    {
        public struct SceneLayersPlan
        {
            public string sceneId;
            public OrderPosition orderPosition;
            public string layerId;

            public SceneLayersPlan(string sceneId, OrderPosition orderPosition, string layerId)
            {
                this.sceneId = sceneId;
                this.layerId = layerId;
                this.orderPosition = orderPosition;
            }
        }

        public void UpdateLayers()
        {
            float z = layerZStart;

            foreach (var layer in layers)
            {
                if (null == layer.parent)
                {
                    Debug.Log("ERROR. Missing parent for layer.");
                    return;
                }

                var position = layer.parent.transform.localPosition;
                layer.parent.transform.localPosition = new Vector3(position.x, position.y, z);

                float zStep = layer.zStep > 0 ? layer.zStep : layerZStep;
                z += zStep * Vector3.back.z;
            }
        }

        public Optional<int> IndexOfLayerId(string id)
        {
            for (int i = 0; i < layers.Count; i++)
            {
                var layer = layers[i];
                if (layer.id == id)
                {
                    return new(i);
                }
            }
            return null;
        }

        protected bool RemoveLayerIdInternal(string id)
        {
            var index = IndexOfLayerId(id);
            if (index == null) { return false; }

            var layer = layers[index.value];
            GameObject.Destroy(layer.parent);

            layers.RemoveAt(index.value);
            return true;
        }

        public void RemoveLayerId(string id)
        {
            if (RemoveLayerIdInternal(id))
            {
                UpdateLayers();
            }
        }

        public void InsertLayer(UILayer layer, OrderPosition orderPosition, string relativeLayerId)
        {
            var index = IndexOfLayerId(relativeLayerId);
            if (index != null)
            {
                switch (orderPosition)
                {
                    case OrderPosition.Before:
                        layers.Insert(index.value, layer);
                        break;
                    case OrderPosition.After:
                        layers.Insert(index.value + 1, layer);
                        break;
                }
            }
            else
            {
                layers.Add(layer);
            }

            UpdateLayers();
        }

        public void SetSceneIdByLayerId(string layerId, string sceneId)
        {
            var index = IndexOfLayerId(layerId);
            if (index == null) { return; }

            var layer = layers[index.value];
            layer.sceneId = sceneId;
            layers[index.value] = layer;
        }

        public void RemoveSceneLayers(string sceneId)
        {
            var layers = this.layers.Where(layer => layer.sceneId == sceneId).ToList();
            if (layers.Count == 0) { return; }

            foreach (var layer in layers)
            {
                RemoveLayerIdInternal(layer.id);
            }

            UpdateLayers();
        }

        public void LoadSceneLayers(SceneLayersPlan plan)
        {
            if (plan.sceneId.Length == 0) { return; }
            StartCoroutine(LoadSceneAsync(plan));
        }

        protected void OnSceneLayersLoad(SceneLayersPlan plan)
        {
            var newScene = SceneManager.GetSceneByName(plan.sceneId);
            var uiSystems = Utils.FindObjectsOfTypeIn<UISystem>(newScene);

            if (uiSystems.Count == 0)
            {
                Debug.Log("ERROR. Scene missing UISystem with layers");
                return;
            }

            var uiSystem = uiSystems[0];

            int insertIndex = 0;
            var relativeIndex = IndexOfLayerId(plan.layerId);
            if (null != relativeIndex)
            {
                switch (plan.orderPosition)
                {
                    case OrderPosition.After:
                        insertIndex = relativeIndex.value + 1;
                        break;
                    case OrderPosition.Before:
                        insertIndex = relativeIndex.value;
                        break;
                }
            }

            if (uiSystem.layers.Count == 0)
            {
                Debug.Log("ERROR. UISystem missing layers.");
            }

            foreach (var layer in uiSystem.layers)
            {
                SceneManager.MoveGameObjectToScene(layer.parent, this.gameObject.scene);
                var newLayer = layer;

                if (layersParent)
                {
                    newLayer.parent.transform.parent = layersParent.transform;
                }

                newLayer.sceneId = plan.sceneId;
                layers.Insert(insertIndex, newLayer);

                insertIndex++;
            }

            UpdateLayers();
            StartCoroutine(UnloadSceneAsync(newScene));
        }

        protected IEnumerator LoadSceneAsync(SceneLayersPlan plan)
        {
            AsyncOperation asyncLoad = SceneManager.LoadSceneAsync(plan.sceneId, LoadSceneMode.Additive);

            while (!asyncLoad.isDone)
            {
                yield return null;
            }

            OnSceneLayersLoad(plan);
        }

        protected IEnumerator UnloadSceneAsync(Scene scene)
        {
            AsyncOperation asyncLoad = SceneManager.UnloadSceneAsync(scene);

            while (!asyncLoad.isDone)
            {
                yield return null;
            }
        }
    }
}
