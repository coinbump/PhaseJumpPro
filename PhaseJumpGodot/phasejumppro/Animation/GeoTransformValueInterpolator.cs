using System;
using Godot;

namespace PJ
{
    /// <summary>
    /// Interpolates a GeoTransform value
    /// </summary>
    public class GeoTransformValueInterpolator : SomeValueInterpolator<GeoTransform>
    {
        public override GeoTransform ValueAt(GeoTransform start, GeoTransform end, float progress)
        {
            var result = start;

            var startRotation = start.rotation;
            var endRotation = end.rotation;

            result.position = Vector3.Lerp(start.position, end.position, progress);
            result.rotation = Vector3.Lerp(startRotation, endRotation, progress);
            result.scale = Vector3.Lerp(start.scale, end.scale, progress);

            return result;
        }
    }

    public class GeoTransformInterpolator : Interpolator<GeoTransform>
    {
        public GeoTransformInterpolator(GeoTransform start, GeoTransform end, SomeValueTransform<float> transform = null) : base(start, end, new GeoTransformValueInterpolator(), transform)
        {
        }
    }
}
