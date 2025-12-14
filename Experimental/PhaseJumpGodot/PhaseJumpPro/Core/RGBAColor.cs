using System;

namespace PJ;

/// Specifies the mask and bitshift schema for an N byte-color
public struct ComponentColorSchema
{
    public UInt32 redMask;
    public UInt32 greenMask;
    public UInt32 blueMask;
    public UInt32 alphaMask;

    public UInt32 redShift;
    public UInt32 greenShift;
    public UInt32 blueShift;
    public UInt32 alphaShift;

    public ComponentColorSchema(
        UInt32 redMask,
        UInt32 greenMask,
        UInt32 blueMask,
        UInt32 alphaMask,

        UInt32 redShift,
        UInt32 greenShift,
        UInt32 blueShift,
        UInt32 alphaShift
    )
    {
        this.redMask = redMask;
        this.greenMask = greenMask;
        this.blueMask = blueMask;
        this.alphaMask = alphaMask;

        this.redShift = redShift;
        this.greenShift = greenShift;
        this.blueShift = blueShift;
        this.alphaShift = alphaShift;
    }

    private static ComponentColorSchema rgba = new(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 8, 16, 24);
    private static ComponentColorSchema bgra = new(0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000, 16, 8, 0, 24);

    public static ComponentColorSchema RGBA => rgba;
    public static ComponentColorSchema BGRA => bgra;
};

/// Stores RGBA as a 32-bit value, one byte for each color component
// C# can't use the same pattern as C++ due to language limitations:
// https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/differences-between-cpp-templates-and-csharp-generics
// TODO: Unit tests
public struct RGBAColor
{
    public UInt32 value;

    public static ComponentColorSchema ColorSchema => ComponentColorSchema.RGBA;

    public RGBAColor()
    {
        this.value = 0;
    }

    public RGBAColor(int red, int green, int blue, int alpha)
    {
        value = ValueFromRGBA(red, green, blue, alpha);
    }

    public RGBAColor(float red, float green, float blue, float alpha)
    {
        value = ValueFromRGBA((int)(red * 255.0f), (int)(green * 255.0f), (int)(blue * 255.0f), (int)(alpha * 255.0f));
    }

    static UInt32 ValueFromRGBA(int red, int green, int blue, int alpha)
    {
        return ((UInt32)red << (int)ColorSchema.redShift) | ((UInt32)green << (int)ColorSchema.greenShift) | ((UInt32)blue << (int)ColorSchema.blueShift) | ((UInt32)alpha << (int)ColorSchema.alphaShift);
    }

    public readonly int R => (int)((value & ColorSchema.redMask) >> (int)ColorSchema.redShift);
    public readonly int G => (int)((value & ColorSchema.greenMask) >> (int)ColorSchema.greenShift);
    public readonly int B => (int)((value & ColorSchema.blueMask) >> (int)ColorSchema.blueShift);
    public readonly int A => (int)((value & ColorSchema.alphaMask) >> (int)ColorSchema.alphaShift);

    public readonly byte R8 => (byte)R;
    public readonly byte G8 => (byte)G;
    public readonly byte B8 => (byte)B;
    public readonly byte A8 => (byte)A;

    public readonly float RedFloat => ((float)R) / 255.0f;
    public readonly float GreenFloat => ((float)G) / 255.0f;
    public readonly float BlueFloat => ((float)B) / 255.0f;
    public readonly float AlphaFloat => ((float)B) / 255.0f;
};
