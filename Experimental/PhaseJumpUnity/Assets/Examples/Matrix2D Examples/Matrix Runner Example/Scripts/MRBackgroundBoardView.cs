using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// Populates the grass tiles into a matrix board behind the hero
/// For performance, background matrix cells are larger than foreground
/// </summary>
public class MRBackgroundBoardView : MatrixBoardView2D
{
    protected override void Start()
    {
        base.Start();

        var sceneObjectRegistry = FindObjectOfType<SceneObjectRegistry>();

        for (int x = 0; x < matrixSize.x; x++)
        {
            for (int y = 0; y < matrixSize.y; y++)
            {
                var tile = sceneObjectRegistry.InstantiateGameObject("tile.grass", Vector3.zero, Quaternion.identity);
                Put(tile.GetComponent<MatrixNode2D>(), new Vector2Int(x, y));
            }
        }
    }
}
