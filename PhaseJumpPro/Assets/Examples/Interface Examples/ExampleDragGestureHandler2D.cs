using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using TMPro;

public class ExampleDragGestureHandler2D : SomeDragGestureHandler2D
{
    public TextMeshPro textMesh;

    public override void OnDragGestureUpdate(Vector2 delta)
    {
        base.OnDragGestureUpdate(delta);

        textMesh.text = delta.x.ToString("0.00") + "," + delta.y.ToString("0.00");
    }
}
