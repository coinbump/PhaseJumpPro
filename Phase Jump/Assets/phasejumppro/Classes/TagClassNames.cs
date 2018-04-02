
/*
 * RATING: 5 stars. Simple constants.
 * CODE REVIEW: 3.31.18
 */
namespace PJ {

	/// <summary>
	/// Standard property class names, used for encoding
	/// 
	/// </summary>
	/// <example>
	/// float v="1.4"
	/// v2 x="1.3" y="3.4"
	/// </example>
	public class TagClassNames {

        // NOTE: const implies static.
		public const string Tag = "tag";
		public const string Int = "int";
		public const string Int64 = "int64";
		public const string Float = "float";
		public const string Bitfield = "bits";
		public const string Bool = "bool";
		public const string Color = "color";
		public const string Enum = "enum";
		public const string NFloat = "nfloat";	// Normalized float (0-1.0)
		public const string String = "str";
		public const string Vector2D = "v2";
		public const string Vector2DInt = "v2i";
		public const string Object = "obj";
		public const string Class = "class";
	}

}
