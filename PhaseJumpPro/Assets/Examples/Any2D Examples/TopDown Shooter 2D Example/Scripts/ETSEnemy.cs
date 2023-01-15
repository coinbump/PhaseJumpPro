using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class ETSEnemy : GoNode2D
{
    protected CannonModel weapon = new();
    protected CannonEmitter2D weaponEmitter;

    protected override void Awake()
    {
        base.Awake();

        weapon.emitsModel.BuildSpread2D(1, Angle.DegreesAngle(0.0f), Angle.DegreesAngle(0));
        weapon.VelocityMagnitude = 40.0f;
        weapon.limitTime = 0.6f;

        weaponEmitter = GetComponent<CannonEmitter2D>();
        weaponEmitter.Model = weapon;

        var angle = Angle.DegreesAngle(RandomUtils.VaryFloat(180.0f, 45.0f));
        var velocity = ((Vector2)angle).normalized * 30.0f;
        Velocity = velocity;
    }

    public override void OnUpdate(TimeSlice time)
    {
        base.OnUpdate(time);

        weaponEmitter.Fire(Angle.DegreesAngle(180.0f));
    }

    protected void OnTriggerEnter2D(Collider2D collider)
    {
        if (collider.TryGetComponent(out GoNode2D node))
        {
            if (node.HasTypeTag("bullet.hero"))
            {
                Destroy(gameObject);
            }
        }
    }
}
