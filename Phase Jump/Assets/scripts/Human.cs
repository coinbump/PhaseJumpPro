using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class Human : PJ.Node2D {

	public enum State
	{
		Invalid,
		WalkLeft,
		WalkRight
	}

	class Core : PJ.Core
	{
		WeakReference owner;

		public GenericStateMachine<State> state = new GenericStateMachine<State>();

		public Core(WeakReference owner)
		{
			this.owner = owner;
			state.State = State.WalkLeft;
			SetStateMachine(state);
		}

		protected override void EvtStateChanged(AbstractStateMachine state)
		{
			base.EvtStateChanged(state);

			if (!owner.IsAlive)
			{
				return;
			}

			var human = owner.Target as Human;
			human.EvtStateChanged();
		}
	}
	Core core;

	void EvtStateChanged() {
		UpdateVelocity();

		switch (core.state.State)
		{
			case State.WalkLeft:
				GetComponent<SpriteRenderer>().flipX = false;
				break;
			case State.WalkRight:
				GetComponent<SpriteRenderer>().flipX = true;
				break;
		}
	}

	void Start() {
		core = new Core(new WeakReference(this));

		UpdateVelocity();
	}

	private void OnCollisionEnter2D(Collision2D coll)
	{
		if (coll.gameObject.tag == "wall")
		{
			switch (core.state.State)
			{
				case State.WalkLeft:
					core.state.State = State.WalkRight;
					break;
				case State.WalkRight:
					core.state.State = State.WalkLeft;
					break;
			}
		}
	}

	void UpdateVelocity() {
		var walkSpeed = 2.0f;

		switch (core.state.State) {
			case State.WalkLeft:
				velocity = new Vector2(-walkSpeed, 0);
				break;
			case State.WalkRight:
				velocity = new Vector2(walkSpeed, 0);
				break;
		}
	}
	
	void FixedUpdate() {
	}

	void Update()
	{
		base.UpdateNode();

		var r = GetComponent<Rigidbody2D>();
		r.velocity = velocity;
	}
}
