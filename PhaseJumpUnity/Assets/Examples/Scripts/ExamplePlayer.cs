using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;
using TMPro;

public class ExamplePlayer : GoNode2D
{
    protected CannonModel burstWeapon = new CannonModel();
    protected CannonEmitter2D weaponEmitter;

    protected override void Awake()
    {
        base.Awake();

        burstWeapon.emitsModel.BuildSpread2D(3, Angle.DegreesAngle(2), Angle.DegreesAngle(2));
        burstWeapon.VelocityMagnitude = 20.0f;
        burstWeapon.limitTime = 0.4f;
        burstWeapon.AddBurstFire(2, 0.1f);

        weaponEmitter = GetComponent<CannonEmitter2D>();
        weaponEmitter.Model = burstWeapon;
    }

    public void OnInputMoveRight(InputValue inputValue)
    {
        var inputVector = inputValue.Get<Vector2>();

        var distance = AngleUtils.Hypotenuse(inputVector.x, inputVector.y);
        //Debug.Log("MoveRight Distance: " + distance.ToString());

        var minDistance = 0.3f;
        if (distance < minDistance)
        {
            return;
        }

        var angle = new Angle(inputVector);
        weaponEmitter.Fire(angle);
    }
}
