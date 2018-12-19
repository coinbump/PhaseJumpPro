using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class Wheel : Node2D {

	class Valve : PJ.Valve {
		public Valve() {
			TimeOn = 2.0f;
			TimeOff = 2.0f;
			turnOnTimer = new InterpolateTimer(new InterpolateSquared(), 1.0f, AbstractTimed.Type.Persistent);
			turnOffTimer = new InterpolateTimer(new InterpolateLinear(), 2.0f, AbstractTimed.Type.Persistent);
		}
	}
	private Valve valve = new Valve();

	// Use this for initialization
	protected override void Start () {
		base.Start();

		valve.TurnOn(InputEffect.OverTime);
	}
	
	// Update is called once per frame
	protected override void Update () {
		base.Update();

		valve.EvtUpdate(new TimeSlice(Time.deltaTime));
		var valveState = valve.ValveState;
		//Debug.Log(string.Format("valveState: {0}, State: {1}", valveState, valve.state.state));
		RotationNormal = valveState;
	}
}
