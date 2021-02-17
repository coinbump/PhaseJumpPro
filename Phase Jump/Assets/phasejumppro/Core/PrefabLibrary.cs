using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ {

	class PrefabLibrary : PJ.MonoBehaviour
	{
		public Dictionary<string, GameObject> library = new Dictionary<string, GameObject>();

		public GameObject GetPrefabByIdentifier(string identifier)
		{
			GameObject result = null;
			this.library.TryGetValue(identifier, out result);
			return result;
		}
	}
}
