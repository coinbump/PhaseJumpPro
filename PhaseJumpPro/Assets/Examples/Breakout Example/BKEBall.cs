using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// A breakout ball that removes blocks that it hits and reflects when hitting blocks
/// </summary>
public class BKEBall : Node2D
{
    public AudioClip hitBlockClip;

    protected Vector3 initialPosition;
    protected Vector2 initialVelocity;

    protected override void Awake()
    {
        base.Awake();

        initialPosition = transform.position;
        initialVelocity = GetComponent<Node2D>().Velocity;
    }

    protected void OnCollisionEnter2D(Collision2D collision)
    {
        var myCollider = GetComponent<CircleCollider2D>();
        var colliderList = new List<Collider2D>();
        myCollider.OverlapCollider(new ContactFilter2D().NoFilter(), colliderList);

        var isFirst = true;
        foreach (Collider2D collider in colliderList)
        {
            var node = collider.gameObject.GetComponent<Node2D>();
            if (null == node) { continue; }

            if (!node.HasTypeTag("block")) { continue; }

            if (isFirst)
            {
                GetComponent<AudioSource>().PlayOneShot(hitBlockClip);

                isFirst = false;

                var contacts = new List<ContactPoint2D>();
                collider.GetContacts(contacts);
                Velocity = Vector2.Reflect(Velocity, contacts[0].normal);
            }

            Destroy(collider.gameObject);
        }


    }

    protected override void OnBecameInvisible()
    {
        transform.position = initialPosition;
        GetComponent<Node2D>().Velocity = initialVelocity;
    }
}
