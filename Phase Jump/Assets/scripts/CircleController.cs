using System;
using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;

public class CircleController : MonoBehaviour {

	protected RedGreenStateMachine stateMachine = new RedGreenStateMachine();
	public RedGreenState defaultState = RedGreenState.Green;
	protected FlipButton flipButton;

	class Listener : PJ.Listener {
		WeakReference owner;

		public Listener(CircleController owner) {
			this.owner = new WeakReference(owner);
		}

		public override void EvtListen(PJ.Event theEvent)
		{
			base.EvtListen(theEvent);

			var owner = this.owner.Target as CircleController;
			if (owner == null) { return; }
			owner.Flip();
		}
	}
	Listener listener;

	// Use this for initialization
	void Start()
	{
		listener = new Listener(this);

		var go = GameObject.Find("flip_button");
		if (go != null) {
			flipButton = go.GetComponent<FlipButton>();
			if (flipButton != null)
			{
				flipButton.broadcaster.AddListener(listener);
			}
		}

		stateMachine.State = defaultState;
		UpdateAppearance();
	}

	public void Flip() {
		switch (stateMachine.state)
		{
			case RedGreenState.Red:
				stateMachine.State = RedGreenState.Green;
				break;
			case RedGreenState.Green:
				stateMachine.State = RedGreenState.Red;
				break;
			default:
				stateMachine.State = RedGreenState.Green;
				break;
		}

		UpdateAppearance();
	}

	protected void UpdateAppearance()
	{
		var sprite = Resources.Load<Sprite>("green_circle");

		switch (stateMachine.state)
		{
			case RedGreenState.Red:
				sprite = Resources.Load<Sprite>("red_circle");
				break;
		}

		GetComponent<SpriteRenderer>().sprite = sprite;
	}

	void OnValidate() {
		stateMachine.State = defaultState;
		UpdateAppearance();
	}

	// Update is called once per frame
	void Update () {

		if (Input.GetMouseButtonDown(0)) {
			var c = Camera.main;
			var hit = Physics2D.Raycast(new Vector2(c.ScreenToWorldPoint(Input.mousePosition).x, c.ScreenToWorldPoint(Input.mousePosition).y), Vector2.zero, 0f);

			if (hit) {
				if (hit.transform.gameObject == this.transform.gameObject) {
					Flip();
				}
			}
		}
	}
}
