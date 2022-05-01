using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class SSEScanner : Node2D, SensorDelegate, SomeSensorListener
{
    public GameObject sensorObject;

    protected override void Awake()
    {
        base.Awake();

        var sensor = sensorObject.GetComponent<SomeSensor>();
        sensor.sensorDelegate = new WeakReference<SensorDelegate>(this);
        sensor.AddListener(this);
    }

    public bool IsSenseTarget(GameObject targetObject)
    {
        return targetObject.GetComponent<SomeNode>().HasTypeTag("target");
    }

    public void OnSense(List<GameObject> objectList, SenseState senseState)
    {
    }
}
