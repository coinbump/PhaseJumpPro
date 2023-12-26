using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using TMPro;

public class ExampleGoalsButton : PJ.ButtonControl
{
    public TextMeshPro countText;
    public GameObject goalMesh;

    protected ValueTargetGoal goal = new(id: "example", 3);

    protected override void OnPress()
    {
        base.OnPress();

        if (goal.IsComplete) { return; }

        goal.Increment();
        countText.text = goal.Value.ToString();

        if (goal.IsComplete)
        {
            goalMesh.SetActive(true);
            goalMesh.GetComponent<SomeEffect>().IsOn = true;
        }
    }
}
