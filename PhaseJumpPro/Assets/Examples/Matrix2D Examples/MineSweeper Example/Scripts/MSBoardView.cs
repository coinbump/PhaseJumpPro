using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using TMPro;

/// <summary>
/// Uses Matrix Board View to play MineSweeper
/// How to Play: Left click to guess at an empty space, right click to flag a potential bomb location
/// Failure: when you tap a bomb -> Game Over.
/// </summary>
public class MSBoardView : MatrixBoardView2D, SomeGoStateListener<MSBoardView.StateType>
{
    enum StateType
    {
        Running, GameOver
    }

    GoCore<StateType> core;

    public class BombCountComparer : IComparer<Vector2Int>
    {
        MSBoardView boardView;

        public BombCountComparer(MSBoardView boardView)
        {
            this.boardView = boardView;
        }

        public int Compare(Vector2Int lhs, Vector2Int rhs)
        {
            return boardView.BombSurroundCountAt(lhs, false) < boardView.BombSurroundCountAt(rhs, false) ? -1 : 1;
        }
    }

    SceneObjectRegistry sceneObjectRegistry;

    public MSBoardView()
    {
        core = new(this);
    }

    protected override void Awake()
    {
        base.Awake();

        sceneObjectRegistry = FindObjectOfType<SceneObjectRegistry>();
    }

    protected override void Start()
    {
        base.Start();
        PopulateBoard();
    }

    protected void PopulateBoard()
    {
        var cellCount = matrixSize.x * matrixSize.y;
        var bombPercentage = 0.15f;
        var bombCount = bombPercentage * cellCount;

        List<Vector2Int> emptyCells = new();

        for (int x = 0; x < matrixSize.x; x++)
        {
            for (int y = 0; y < matrixSize.y; y++)
            {
                emptyCells.Add(new Vector2Int(x, y));
            }
        }

        for (int i = 0; i < bombCount; i++)
        {
            if (emptyCells.Count == 0)
            {
                break;
            }
            var choiceIndex = Random.Range(0, emptyCells.Count);

            var choice = emptyCells[choiceIndex];
            emptyCells.RemoveAt(choiceIndex);

            var bombTileObject = sceneObjectRegistry.InstantiateGameObject("tile", Vector3.zero, Quaternion.identity);
            var bombTile = bombTileObject.GetComponent<MSTile>();
            bombTile.type = MSTile.TileType.Bomb;
            Put(bombTile, choice);
        }

        foreach (var cell in emptyCells)
        {
            var tileObject = sceneObjectRegistry.InstantiateGameObject("tile", Vector3.zero, Quaternion.identity);
            var tile = tileObject.GetComponent<MSTile>();
            Put(tile, cell);
        }

        // Give the player a hint by clearing something for them
        emptyCells.Sort(new BombCountComparer(this));
        ClearTilesAt(emptyCells[0]);
    }

    public override void OnPointerClickEvent(PointerClickUIEvent _event)
    {
        var localPressPosition = _event.pressPosition;
        var cellHit = LocationAtLocalPosition(localPressPosition.Position);

        if (cellHit.value == null) { return; }

        var cellGameObject = WorldNodeAt(cellHit.value);
        var hitTile = cellGameObject.GetComponent<MSTile>();

        switch (_event.button)
        {
            case PointerClickUIEvent.InputButton.Left:
                {
                    switch (core.State)
                    {
                        case StateType.Running:
                            if (hitTile.type == MSTile.TileType.Bomb)
                            {
                                RevealBombs();
                            }
                            else
                            {
                                ClearTilesAt(cellHit.value);
                            }
                            break;
                        case StateType.GameOver:
                            Restart();
                            break;
                    }
                    break;
                }
            case PointerClickUIEvent.InputButton.Right:
                {
                    switch (hitTile.type)
                    {
                        case MSTile.TileType.Tile:
                            {
                                var flagTileObject = sceneObjectRegistry.InstantiateGameObject("tile-flag", Vector3.zero, Quaternion.identity);
                                var flagTile = flagTileObject.GetComponent<MSTile>();
                                Replace(flagTile, cellHit.value);
                                break;
                            }
                        case MSTile.TileType.Bomb:
                            {
                                var flagTileObject = sceneObjectRegistry.InstantiateGameObject("tile-flag", Vector3.zero, Quaternion.identity);
                                var flagTile = flagTileObject.GetComponent<MSTile>();
                                flagTile.type = MSTile.TileType.BombFlag;
                                Replace(flagTile, cellHit.value);
                                break;
                            }
                        case MSTile.TileType.ClearFlag:
                            {
                                var tileObject = sceneObjectRegistry.InstantiateGameObject("tile", Vector3.zero, Quaternion.identity);
                                var tile = tileObject.GetComponent<MSTile>();
                                Replace(tile, cellHit.value);
                                break;
                            }
                        case MSTile.TileType.BombFlag:
                            {
                                var tileObject = sceneObjectRegistry.InstantiateGameObject("tile", Vector3.zero, Quaternion.identity);
                                var tile = tileObject.GetComponent<MSTile>();
                                tile.type = MSTile.TileType.Bomb;
                                Replace(tile, cellHit.value);
                                break;
                            }
                        default:
                            break;
                    }
                }
                break;
        }
    }

    protected void Restart()
    {
        RemoveAll();
        PopulateBoard();
        core.State = StateType.Running;
    }

    protected void RevealBombs()
    {
        core.State = StateType.GameOver;

        for (int x = 0; x < matrixSize.x; x++)
        {
            for (int y = 0; y < matrixSize.y; y++)
            {
                var cell = new Vector2Int(x, y);
                var gameObject = WorldNodeAt(cell);
                if (gameObject)
                {
                    var tile = gameObject.GetComponent<MSTile>();

                    if (tile.IsBomb)
                    {
                        var bombTileObject = sceneObjectRegistry.InstantiateGameObject("tile-bomb", Vector3.zero, Quaternion.identity);
                        var bombTile = bombTileObject.GetComponent<MSTile>();
                        Replace(bombTile, cell);
                    }
                }
            }
        }
    }

    protected void ClearTilesAt(Vector2Int cell)
    {
        ClearUnclearedTileAt(cell);
    }

    protected void ClearUnclearedTileAt(Vector2Int cell)
    {
        var gameObject = WorldNodeAt(cell);
        if (null == gameObject) { return; }

        var tile = gameObject.GetComponent<MSTile>();
        if (tile.type == MSTile.TileType.Tile)
        {
            var clearTileObject = sceneObjectRegistry.InstantiateGameObject("tile-clear", Vector3.zero, Quaternion.identity);
            var clearTile = clearTileObject.GetComponent<MSClearTile>();
            Replace(clearTile, cell);

            var bombSurroundCount = BombSurroundCountAt(cell, false);
            var tmp = clearTile.text.GetComponent<TextMeshPro>();

            Color color = Color.red;
            Color.RGBToHSV(color, out float h, out float s, out float v);
            h = 1.0f - (Mathf.Min(4.0f, (float)bombSurroundCount) / 4.0f);

            tmp.color = Color.HSVToRGB(h, s, v);

            switch (bombSurroundCount)
            {
                case 0:
                    Destroy(clearTile.text);
                    clearTile.text = null;
                    break;
                default:
                    tmp.text = bombSurroundCount.ToString();
                    break;
            }

            if (BombSurroundCountAt(cell, true) == 0)
            {
                ClearUnclearedTileAt(new Vector2Int(cell.x - 1, cell.y));
                ClearUnclearedTileAt(new Vector2Int(cell.x + 1, cell.y));
                ClearUnclearedTileAt(new Vector2Int(cell.x, cell.y - 1));
                ClearUnclearedTileAt(new Vector2Int(cell.x, cell.y + 1));
            }
        }
    }

    protected int BombSurroundCountAt(Vector2Int cell, bool skipDiagonals)
    {
        var result = 0;

        for (int x = cell.x - 1; x <= cell.x + 1; x++)
        {
            for (int y = cell.y - 1; y <= cell.y + 1; y++)
            {
                if (x == cell.x && y == cell.y) { continue; }
                if (skipDiagonals)
                {
                    if (x != cell.x && y != cell.y)
                    {
                        continue;
                    }
                }

                var tileObject = WorldNodeAt(new Vector2Int(x, y));
                if (null == tileObject) { continue; }

                var tile = tileObject.GetComponent<MSTile>();
                if (tile.IsBomb)
                {
                    result++;
                }
            }
        }

        return result;
    }

    void SomeGoStateListener<StateType>.OnStateChange(GoStateMachine<StateType> inStateMachine)
    {
    }

    void SomeGoStateListener<StateType>.OnStateFinish(GoStateMachine<StateType> inStateMachine)
    {
    }
}
