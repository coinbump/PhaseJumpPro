using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using System;

/// <summary>
/// Moves in a random direction every N seconds
/// </summary>
public class MREnemy : MatrixNode2D
{
    protected class MoveTimer : Timer
    {
        WeakReference<MREnemy> owner;

        public MoveTimer(MREnemy owner) : base(3.0f, SomeRunner.RunType.KeepRunning)
        {
            this.owner = new WeakReference<MREnemy>(owner);
        }

        protected override void OnFinish()
        {
            base.OnFinish();

            if (owner.TryGetTarget(out MREnemy enemy))
            {
                enemy.OnMoveTimerFinish();
            }
        }
    }

    protected override void Awake()
    {
        base.Awake();

        var timer = new MoveTimer(this);
        updatables.Add(timer);
    }

    public void OnMoveTimerFinish()
    {
        List<MapDirection> directions = new();
        directions.Add(MapDirection.North);
        directions.Add(MapDirection.Northeast);
        directions.Add(MapDirection.East);
        directions.Add(MapDirection.Southeast);
        directions.Add(MapDirection.South);
        directions.Add(MapDirection.Southwest);
        directions.Add(MapDirection.West);
        directions.Add(MapDirection.Northwest);

        var tryDirection = RandomUtils.ChooseFrom(directions);
        transform.parent.GetComponent<MatrixBoardView2D>().MovePiece(piece, tryDirection, 0.2f);
    }
}
