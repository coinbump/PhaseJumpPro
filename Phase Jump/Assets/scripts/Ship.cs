using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class Ship : PJ.Node2D {

	// Use this for initialization
	void Start () {
		isKinematic = true;
	}
	
	// Update is called once per frame
	protected override void Update() {
		UpdateNode(UpdateType.Default);
		    
		if (Input.GetKey(KeyCode.RightArrow))
		{
			RotationAngle += 180.0f * Time.deltaTime;
		}
		else if (Input.GetKey(KeyCode.LeftArrow))
		{
			RotationAngle -= 180.0f * Time.deltaTime;
		}

		if (Input.GetKey(KeyCode.UpArrow))
		{
			directionVelocity = 2.0f;
		}
		else {
			directionVelocity = 0;
		}
	}
}
