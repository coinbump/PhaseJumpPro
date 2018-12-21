using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	public class FXHueShift : MonoBehaviour
	{
		public float cycleTime = 1.0f;

		// Start is called before the first frame update
		void Start()
		{

		}

		// Update is called once per frame
		void Update()
		{
			SpriteRenderer spriteRenderer = GetComponent<SpriteRenderer>();
			if (null == spriteRenderer) { return; }

			Color color = spriteRenderer.color;
			float H, S, V;
			Color.RGBToHSV(color, out H, out S, out V);
			H += Time.deltaTime / cycleTime;
			H = H % 1.0f;

			color = Color.HSVToRGB(H, S, V);

			spriteRenderer.color = color;
		}
	}

}
