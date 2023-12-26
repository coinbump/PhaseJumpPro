using UnityEngine;
using System.Collections;

/*
 * RATING: 3 stars
 * Works, but functionality is basic
 * FUTURE: support advanced camera control in 3D space
 * CODE REVIEW: 4/8/22
 */
namespace PJ
{
    /// <summary>
    /// Simple control scheme for previewing 3D scenes
    /// Attach to the main camera
    /// </summary>
    public class SimpleCameraInputHandler : SomeInputHandler
    {
        [Tooltip("Distance per second")]
        public float moveSpeed = 10.0f;

        [Tooltip("Angles per second")]
        public float rotateSpeed = 45.0f;

        [Tooltip("Snap rotation to fixed angles")]
        public bool isRotateSnap = false;

        protected override void Start()
        {
        }

        protected override void Update()
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
