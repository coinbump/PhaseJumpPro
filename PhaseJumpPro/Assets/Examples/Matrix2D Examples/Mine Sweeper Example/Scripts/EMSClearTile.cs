using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class EMSClearTile : EMSTile
{
    public GameObject text;

    EMSClearTile()
    {
        this.type = EMSClearTile.TileType.Clear;
    }
}
