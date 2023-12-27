using System.Data.Common;
using Godot;

namespace PJ;

// FUTURE: Evalute if [StructLayout] is needed: https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.structlayoutattribute?view=net-8.0&redirectedfrom=MSDN
/// <summary>
/// Bridge color type for moving from Unity game engine
/// </summary>
// TODO: Unit tests + test packing
public struct Color32
{
    public byte r;
    public byte g;
    public byte b;
    public byte a;

    public static implicit operator Color(Color32 color32)
    {
        return new Color((float)color32.r / 255.0f, (float)color32.g / 255.0f, (float)color32.b / 255.0f, (float)color32.a / 255.0f);
    }
}
