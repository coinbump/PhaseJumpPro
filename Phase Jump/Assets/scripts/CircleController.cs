﻿using System;
using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;

public class CircleController : PJ.MonoBehaviour {

	protected RedGreenStateMachine state = new RedGreenStateMachine();
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

		state.State = defaultState;
		UpdateAppearance();
	}

	public void Flip() {
		switch (state.state)
		{
			case RedGreenState.Red:
				state.State = RedGreenState.Green;
				break;
			case RedGreenState.Green:
				state.State = RedGreenState.Red;
				break;
			default:
				state.State = RedGreenState.Green;
				break;
		}

		UpdateAppearance();
	}

	protected void UpdateAppearance()
	{
		var sprite = Resources.Load<Sprite>("green_circle");

		switch (state.state)
		{
			case RedGreenState.Red:
				sprite = Resources.Load<Sprite>("red_circle");
				break;
		}

		GetComponent<SpriteRenderer>().sprite = sprite;
	}

	void OnValidate() {
		state.State = defaultState;
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
