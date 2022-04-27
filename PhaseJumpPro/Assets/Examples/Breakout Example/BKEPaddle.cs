using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class BKEPaddle : Node2D
{
    public AudioClip hitBallClip;

    protected void OnCollisionEnter2D(Collision2D collision)
    {
        var node = collision.collider.gameObject.GetComponent<Node2D>();
        if (null == node) { return; }

        if (!node.HasTypeTag("ball")) { return; }

        //Debug.Log("Ball hit paddle");

        ContactPoint2D contactPoint = collision.contacts[0];

        var myCollider = GetComponent<CapsuleCollider2D>();
        var factor = (contactPoint.point.x - myCollider.bounds.center.x) / (myCollider.bounds.size.x / 2.0f);

        var reflectDegreeAngle = factor * 45.0f;
        //Debug.Log("Paddle factor: " + factor.ToString() + "Reflect angle: " + reflectDegreeAngle);

        node.Velocity = AngleUtils.DegreeAngleToVector2(reflectDegreeAngle, 10);

        GetComponent<AudioSource>().PlayOneShot(hitBallClip);
    }
}
