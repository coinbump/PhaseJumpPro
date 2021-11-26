using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class F2Enemy1 : PJ.GoNode2D
{
    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
    }

    // Update is called once per frame
    protected override void Update()
    {
        base.Update();
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "hero_bullet")
        {
            Destroy(this.gameObject);
        }
    }
}
