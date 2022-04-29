using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class BKEBoard : PJ.MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        GetComponent<SomeGameObjectsLayout>().ApplyLayout();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
