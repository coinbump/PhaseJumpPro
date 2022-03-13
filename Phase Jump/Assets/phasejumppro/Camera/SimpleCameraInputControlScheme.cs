using UnityEngine;
using System.Collections;

namespace PJ
{
	/// <summary>
    /// Simple control scheme for previewing 3D scenes
    /// Attach to the main camera
    /// </summary>
	public class SimpleCameraInputControlScheme : SomeInputControlScheme
	{
		[Tooltip("Distance per second")]
		public float moveSpeed = 10.0f;

		[Tooltip("Angles per second")]
		public float rotateSpeed = 45.0f;

		[Tooltip("Snap rotation to fixed angles")]
		public bool isRotateSnap = false;

		public virtual void Start()
		{
		}

		public virtual void Update()
		{
			var xAxis = Input.GetAxis("Horizontal");
			var yAxis = Input.GetAxis("Vertical");

			var moveSpeed = this.moveSpeed * Time.deltaTime;
			gameObject.transform.Translate(new Vector3(xAxis * moveSpeed, 0, yAxis * moveSpeed));

			if (isRotateSnap)
			{
				if (Input.GetKeyDown("q"))
				{
					gameObject.transform.Rotate(new Vector3(0, 0, -rotateSpeed));
				}
				else if (Input.GetKeyDown("e"))
				{
					gameObject.transform.Rotate(new Vector3(0, 0, rotateSpeed));
				}
			}
			else
			{
				var rotateSpeed = this.rotateSpeed * Time.deltaTime;
				if (Input.GetKey("q"))
				{
					gameObject.transform.Rotate(new Vector3(0, 0, -rotateSpeed));
				}
				else if (Input.GetKey("e"))
				{
					gameObject.transform.Rotate(new Vector3(0, 0, rotateSpeed));
				}
			}

			if (Input.GetKey("r"))
			{
				gameObject.transform.Translate(moveSpeed * Vector3.up);
			}
			else if (Input.GetKey("f"))
			{
				gameObject.transform.Translate(moveSpeed * Vector3.down);
			}
		}
	}
}
