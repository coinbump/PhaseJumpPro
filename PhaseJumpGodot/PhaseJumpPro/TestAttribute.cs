using System;

// TODO: temporary until we come up with a unit testing solution
public class TestAttribute : Attribute
{
}

public class RequireComponent : Attribute
{
    public RequireComponent(Type requiredComponent)
    {
    }
}

public class RangeAttribute : Attribute
{
    public RangeAttribute(float min, float max)
    {
    }
}

public class TooltipAttribute : Attribute
{
    public TooltipAttribute(string value)
    {
    }
}

public class HeaderAttribute : Attribute
{
    public HeaderAttribute(string value)
    {
    }
}