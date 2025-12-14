using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using Cinemachine;

/// <summary>
/// Contains walls, hero, and enemies
/// </summary>
public class MRBoardView : MatrixBoardView2D
{
    protected SceneObjectRegistry sceneObjectRegistry;

    protected override void Start()
    {
        base.Start();

        sceneObjectRegistry = FindObjectOfType<SceneObjectRegistry>();

        AddHero();
        PopulateOuterWalls();
        PopulateNoiseWalls();
        PopulateEnemies();
    }

    void AddHero()
    {
        var hero = sceneObjectRegistry.InstantiateGameObject("hero", Vector3.zero, Quaternion.identity);
        Put(hero.GetComponent<MatrixNode2D>(), new Vector2Int((int)(matrixSize.x / 2.0f), (int)(matrixSize.y / 2.0f)));

        // Make camera follow the hero
        var cinemachineCamera = FindObjectOfType<CinemachineVirtualCamera>();
        cinemachineCamera.Follow = hero.transform;
    }

    void PopulateOuterWalls()
    {
        for (int x = 0; x < matrixSize.x; x++)
        {
            var wall = sceneObjectRegistry.InstantiateGameObject("tile.wall", Vector3.zero, Quaternion.identity);
            Put(wall.GetComponent<MatrixNode2D>(), new Vector2Int(x, 0));
            wall = sceneObjectRegistry.InstantiateGameObject("tile.wall", Vector3.zero, Quaternion.identity);
            Put(wall.GetComponent<MatrixNode2D>(), new Vector2Int(x, matrixSize.y - 1));
        }

        for (int y = 1; y < matrixSize.y - 1; y++)
        {
            var wall = sceneObjectRegistry.InstantiateGameObject("tile.wall", Vector3.zero, Quaternion.identity);
            Put(wall.GetComponent<MatrixNode2D>(), new Vector2Int(0, y));
            wall = sceneObjectRegistry.InstantiateGameObject("tile.wall", Vector3.zero, Quaternion.identity);
            Put(wall.GetComponent<MatrixNode2D>(), new Vector2Int(matrixSize.x - 1, y));
        }
    }

    void PopulateNoiseWalls()
    {
        var noise = new PJ.Noise2D.Perlin(Vector2.zero, new Vector2(20.0f, 20.0f));
        for (int x = 1; x < matrixSize.x - 1; x++)
        {
            for (int y = 1; y < matrixSize.y - 1; y++)
            {
                var noiseValue = noise.ValueFor(new Vector2((float)x / (float)matrixSize.x, (float)y / (float)matrixSize.y));
                if (noiseValue > 0.8f)
                {
                    var wall = sceneObjectRegistry.InstantiateGameObject("tile.wall", Vector3.zero, Quaternion.identity);
                    Put(wall.GetComponent<MatrixNode2D>(), new Vector2Int(x, y));
                }
            }
        }
    }

    void PopulateEnemies()
    {
        var noise = new PJ.Noise2D.Perlin(Vector2.zero, new Vector2(20.0f, 20.0f));
        for (int x = 1; x < matrixSize.x - 1; x++)
        {
            for (int y = 1; y < matrixSize.y - 1; y++)
            {
                var noiseValue = noise.ValueFor(new Vector2((float)x / (float)matrixSize.x, (float)y / (float)matrixSize.y));
                if (noiseValue < 0.1f)
                {
                    var enemy = sceneObjectRegistry.InstantiateGameObject("enemy", Vector3.zero, Quaternion.identity);
                    Put(enemy.GetComponent<MatrixNode2D>(), new Vector2Int(x, y));
                }
            }
        }
    }
}
