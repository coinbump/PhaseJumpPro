/*
 * RATING: 5 stars.
 * CODE REVIEW: 3.30.18.
 */
namespace PJ {

	/// <summary>
	/// Standard property names for encoding.
	/// </summary>
    /// 
    /// Code style:
    /// https://docs.microsoft.com/en-us/dotnet/standard/design-guidelines/capitalization-conventions
	/// FUTURE: evaluate Google protocol buffers vs XML
	public class TagNames {

		/*
			Standard property names for objects.
	 
			Used in XML files, and when identifying tags attached to objects.
			
	 	*/
		public const string Some = "a";
		public const string Anchor = "anchor";
		public const string Alpha = "alpha";
		public const string Art = "art";
		public const string Bounds = "b";			// Concat for smaller files.
		public const string BrowseName = "name_browse";	// Browse name for classes (sq -> "Squirrel")
		public const string Class = "class";
		public const string ClassName = "name_class";
		public const string Color = "color";
		public const string Count = "count";		// For storing the count # of objects
		public const string Currency = "currency";
		public const string CurrencyGame = "currency_g";
		public const string CurrencyPaid = "currency_p";
		public const string Description = "desc";		// Verbose description.
		public const string DescriptionShort = "sdesc";		// Short description.
		public const string Duration = "dur";		// Duration (time)
		public const string RealWorldStart = "rw_start";	// Start time (encoded time stamp in UTC time).
		public const string RealWorldDuration = "rw_dur";	// Duration (real-world time)
		public const string X = "x";
		public const string Y = "y";
		public const string Width = "width";
		public const string Height = "height";
		public const string PercentWidth = "p_width";	// % of some other value
		public const string PercentHeight = "p_height";	// % of some other value
		public const string CellWidth = "c_width";
		public const string CellHeight = "c_height";
		public const string Command = "cmd";
		public const string Distance = "dist";
		public const string Label = "l";			// Label (esp graphs). Concat for lots of nodes.
		public const string DrawableName = "name_draw";		// Name of the drawable to represent an entity visually
		public const string LogicOp = "op";
		public const string Number = "num";		// To specify an amount
		public const string Type = "type";
		public const string Position = "pos";
		public const string Rotate = "rot";
		public const string Scale = "scale";
		public const string Size = "size";
		public const string State = "state";
		public const string Parent = "parent";
		public const string EaseIn = "ease_in";
		public const string EaseOut = "ease_out";
		public const string Flags = "flags";		// Comma-delimited flag keys.
		public const string FPS = "fps";
		public const string Interpolate = "interp";
		public const string Key = "key";
		public const string Listener = "listener";
		public const string Message = "msg";
		public const string Metadata = "meta";
		public const string Param = "param";
		public const string Category = "cat";
		public const string InsetLeft = "inset_l";
		public const string InsetTop = "inset_t";
		public const string InsetRight = "inset_r";
		public const string InsetBottom = "inset_b";
		public const string InsetX = "inset_x";
		public const string InsetY = "inset_y";
		public const string Path = "path";
		public const string Price = "price";
		public const string Direction = "dir";
		public const string Magnitude = "mag";
		public const string Multiplier = "mult";
		public const string Name = "name";
		public const string Velocity = "vel";
		public const string Version = "version";
		public const string MinVersion = "min_version";
		public const string Index = "i";
		public const string ID = "id";
		public const string VolumeMusic = "vol_music";
		public const string VolumeSound = "vol_sound";
		public const string Speed = "speed";
		public const string StrokeWidth = "stroke_width";
		public const string StrokeColor = "stroke_color";
		public const string TileWidth = "t_width";
		public const string TileHeight = "t_height";
		public const string Title = "title";
		public const string EastOf = "eof";
		public const string WestOf = "wof";
		public const string NorthOf = "nof";
		public const string SouthOf = "sof";
		public const string Value = "v";
		public const string UDID = "udid";	// DEVICE UDID (iOS)
		public const string UUID = "uuid";	// OBJECT UUID

		// LAYOUT:
		public const string LayoutDir = "l_dir";
		public const string LayoutNumCells = "l_numcell";
		public const string LayoutCell = "l_cell";
		public const string LayoutWidth = "l_width";
		public const string LayoutHeight = "l_height";

		// RPG:
		public const string RPGXP = "xp";
		public const string RPGXPThis = "xp_this";
		public const string RPGLEvel = "level";

		public const string AppVersion = "app_version";

	}

}

