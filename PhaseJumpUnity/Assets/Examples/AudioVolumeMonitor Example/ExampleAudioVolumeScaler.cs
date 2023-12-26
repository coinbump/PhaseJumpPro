using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

public class ExampleAudioVolumeScaler : PJ.WorldComponent
{
    protected AudioVolumeMonitor audioVolumeMonitor;

    protected override void Awake()
    {
        base.Awake();

        audioVolumeMonitor = GetComponent<AudioVolumeMonitor>();
    }

    public override void OnUpdate(TimeSlice time)
    {
        base.OnUpdate(time);

        var scale = 1.0f + 2.0f * audioVolumeMonitor.Volume;

        transform.localScale = new Vector3(scale, scale, 1.0f);
    }
}
