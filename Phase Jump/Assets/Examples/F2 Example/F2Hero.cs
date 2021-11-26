using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class F2Hero : PJ.GoNode2D
{
    public enum InputType
    {
        Mouse, Joystick
    }

    public GameObject bulletObject;
    public InputType inputType;

    protected override void Start()
    {
        base.Start();

        var mouseInputScheme = GetComponent<PJ.MoveToMouseInputControlScheme2D>();
        mouseInputScheme.enabled = null != mouseInputScheme ? inputType == InputType.Mouse : false;
    }

    // Update is called once per frame
    protected override void Update()
    {
        base.Update();

        if (Input.GetButtonDown("Fire1")) {
            Instantiate(bulletObject, this.transform.position, Quaternion.identity);
        }

        switch (inputType)
        {
            case InputType.Joystick:
                //var xAxis = Input.GetAxis("Horizontal");
                //var yAxis = Input.GetAxis("Vertical");

                //var deadZone = 0.3f;
                //var xMove = 0.0f;
                //var yMove = 0.0f;
                //var baseMoveSpeed = 10.0f;

                //if (Math.Abs(xAxis) >= deadZone)
                //{
                //    var interpolate = new PJ.InterpolateSquared();
                //    var factor = (Math.Abs(xAxis) - deadZone) / (1.0f - deadZone);
                //    var interpolatedFactor = interpolate.Evaluate(factor);
                //    var moveSpeed = baseMoveSpeed * interpolatedFactor;
                //    xMove = xAxis < 0 ? -moveSpeed : moveSpeed;
                //}
                //if (Math.Abs(yAxis) >= deadZone)
                //{
                //    var moveSpeed = baseMoveSpeed;
                //    yMove = yAxis < 0 ? -moveSpeed : moveSpeed;
                //}

                //velocity = new Vector2(xMove, yMove);
                break;
            default:
                break;
        }
    }
}
