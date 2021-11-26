using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class Ship : PJ.GoNode2D {

	protected override void Awake()
	{
		base.Awake();
	}

	// Use this for initialization
	protected override void Start () {
		base.Start();
	}
	
	// Update is called once per frame
	protected override void Update() {
		UpdateNode(UpdateType.Default);
		    
		if (Input.GetAxis("Horizontal") > 0)
		{
			RotationDegreeAngle += 180.0f * Time.deltaTime;
		}
		else if (Input.GetAxis("Horizontal") < 0)
		{
			RotationDegreeAngle -= 180.0f * Time.deltaTime;
		}

		if (Input.GetAxis("Vertical") > 0)
		{
			directionVelocity = 2.0f;
		}
		else {
			directionVelocity = 0;
		}
	}
}
