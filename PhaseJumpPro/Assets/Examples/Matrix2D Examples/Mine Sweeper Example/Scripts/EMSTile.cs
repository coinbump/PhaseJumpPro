using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class EMSTile : MatrixNode2D
{
    public enum TileType
    {
        Tile, Bomb, Clear, ClearFlag, BombFlag
    }

    public TileType type = TileType.Tile;

    public bool IsBomb
    {
        get
        {
            switch (type)
            {
                case EMSTile.TileType.Bomb:
                case EMSTile.TileType.BombFlag:
                    return true;
                default:
                    return false;
            }
        }
    }
}
