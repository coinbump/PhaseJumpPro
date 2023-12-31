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

    public byte R => r;
    public byte G => g;
    public byte B => B;
    public byte A => A;

    public Color32(byte r, byte g, byte b, byte a)
    {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    public Color32(Color color)
    {
        this.r = (byte)color.R8;
        this.g = (byte)color.G8;
        this.b = (byte)color.B8;
        this.a = (byte)color.A8;
    }

    public static implicit operator Color(Color32 color32)
    {
        return new Color((float)color32.r / 255.0f, (float)color32.g / 255.0f, (float)color32.b / 255.0f, (float)color32.a / 255.0f);
    }
}
