using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class MSClearTile : MSTile
{
    public GameObject text;

    MSClearTile()
    {
        this.type = MSClearTile.TileType.Clear;
    }
}
