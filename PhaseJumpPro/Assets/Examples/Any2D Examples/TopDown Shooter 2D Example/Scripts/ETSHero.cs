using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class ETSHero : GoNode2D
{
    protected CannonModel weapon = new();
    protected CannonEmitter2D weaponEmitter;

    protected override void Awake()
    {
        base.Awake();

        weapon.emitsModel.BuildSpread2D(3, Angle.DegreesAngle(10.0f), Angle.DegreesAngle(0));
        weapon.VelocityMagnitude = 50.0f;
        weapon.limitTime = 0.4f;

        weaponEmitter = GetComponent<CannonEmitter2D>();
        weaponEmitter.Model = weapon;
    }

    public override void OnUpdate(TimeSlice time)
    {
        base.OnUpdate(time);

        weaponEmitter.Fire(Angle.DegreesAngle(0));
    }
}
