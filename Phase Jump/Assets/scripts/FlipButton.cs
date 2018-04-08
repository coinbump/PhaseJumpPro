using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FlipButton : MonoBehaviour {
	public PJ.Broadcaster broadcaster = new PJ.Broadcaster();

	void Start()
	{
		Button btn = this.GetComponent<Button>();
		btn.onClick.AddListener(EvtTapped);
	}

	void TaskOnClick()
	{
		Debug.Log("You have clicked the button!");
	}

	// Update is called once per frame
	void Update () {
		
	}

	public void EvtTapped() {
		broadcaster.Broadcast(new PJ.Event("flip"));
	}
}
