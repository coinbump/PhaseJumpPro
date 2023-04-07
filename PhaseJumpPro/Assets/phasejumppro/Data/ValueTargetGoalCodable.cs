using UnityEngine;
using PJ;
using System.Collections.Generic;

/// <summary>
/// Encodes a ValueTargetGoal
/// </summary>
public struct ValueTargetGoalCodable
{
    public string id;
    public int value;

    public ValueTargetGoalCodable(string id, int value)
    {
        this.id = id;
        this.value = value;
    }
}
