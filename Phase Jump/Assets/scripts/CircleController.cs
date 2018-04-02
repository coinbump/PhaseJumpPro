using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CircleController : MonoBehaviour {

	protected RedGreenStateMachine stateMachine = new RedGreenStateMachine();
	public RedGreenState defaultState = RedGreenState.Green;
	// Use this for initialization
	void Start () {
		stateMachine.SetState(defaultState);
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
		stateMachine.SetState(defaultState);
		UpdateAppearance();
	}

	// Update is called once per frame
	void Update () {

		if (Input.GetMouseButtonDown(0)) {
			var camera = Camera.main;
			var hit = Physics2D.Raycast(new Vector2(camera.ScreenToWorldPoint(Input.mousePosition).x, camera.ScreenToWorldPoint(Input.mousePosition).y), Vector2.zero, 0f);

			if (hit) {
				if (hit.transform.gameObject == this.transform.gameObject) {
					switch (stateMachine.state)
					{
						case RedGreenState.Red:
							stateMachine.SetState(RedGreenState.Green);
							break;
						case RedGreenState.Green:
							stateMachine.SetState(RedGreenState.Red);
							break;
						default:
							stateMachine.SetState(RedGreenState.Green);
							break;
					}

					UpdateAppearance();
				}
			}
		}
	}
}
