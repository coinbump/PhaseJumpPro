﻿using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 5/1/22
 */
namespace PJ
{
    /// <summary>
    /// Builds a mesh that matches the angle range of the attached ScanSensor2D
    /// </summary>
    public class ScanSensorMeshBuilder2D : SomeMeshBuilder, SomeSensorListener
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public Color32 noSenseColor = Color.green;
        public Color32 senseColor = Color.red;

        public WeakGameObjectSet sensedObjects = new WeakGameObjectSet();

        protected override Vector2 WorldSizeInternal
        {
            get
            {
                var circleCollider = GetComponent<CircleCollider2D>();
                if (null == circleCollider) { return Vector2.zero; }

                return new Vector2(circleCollider.radius * 2.0f, circleCollider.radius * 2.0f);
            }
            set
            {
                var circleCollider = GetComponent<CircleCollider2D>();
                if (null == circleCollider) { return; }

                circleCollider.radius = Mathf.Min(value.x, value.y) / 2.0f;
            }
        }

        protected override void Awake()
        {
            var scanSensor = GetComponent<ScanSensor2D>();
            if (null == scanSensor)
            {
                //Debug.Log("Error. Scan sensor required for Scan Mesh");
                return;
            }

            GetComponent<MeshRenderer>().material.color = noSenseColor;
            scanSensor.AddListener(this);
        }

        public override Mesh BuildMesh()
        {
            var scanSensor = GetComponent<ScanSensor2D>();
            if (null == scanSensor)
            {
                return null;
            }

            var shapeMesh = new ArcMesh(Angle.DegreesAngle(-scanSensor.degreeAngleScan / 2.0f), Angle.DegreesAngle(scanSensor.degreeAngleScan / 2.0f), angleStep, WorldSizeInternal);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

        public void OnSense(List<GameObject> objectList, SenseState senseState)
        {
            if (objectList.Count <= 0) { return; }
            sensedObjects.Refresh();

            foreach (GameObject sensedObject in objectList)
            {
                switch (senseState)
                {
                    case SenseState.Enter:
                    // Add for stay because the object might have been occluded when it entered the sensor range
                    case SenseState.Stay:
                        sensedObjects.Add(sensedObject);
                        //Debug.Log("Scan Sensor Mesh Enter/Stay: " + sensedObject.name + " count: " + sensedObjects.Count);
                        break;
                    case SenseState.Exit:
                        sensedObjects.Remove(sensedObject);
                        //Debug.Log("Scan Sensor Mesh Exit: " + sensedObject.name + " count: " + sensedObjects.Count);
                        break;
                }
            }

            if (sensedObjects.Count > 0)
            {
                GetComponent<MeshRenderer>().material.color = senseColor;
            }
            else
            {
                GetComponent<MeshRenderer>().material.color = noSenseColor;
            }
        }
    }
}
