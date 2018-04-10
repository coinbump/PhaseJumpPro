using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ship : PJ.Node2D {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update() {
		UpdateNode();
		    
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
