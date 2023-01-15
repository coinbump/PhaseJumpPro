using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class ETSStar : GoNode2D
{
    protected override void Awake()
    {
        base.Awake();

        float scale = RandomUtils.VaryFloat(0.7f, 0.15f);
        transform.localScale = new Vector3(scale, scale, 1.0f);
    }
}
