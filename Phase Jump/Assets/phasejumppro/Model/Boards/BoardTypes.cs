using System.Collections;
using System.Collections.Generic;

/// <summary>
/// Direction of movement on a hexagonal board
/// </summary>
public enum HexBoardDirection
{
	UpLeft,
	Up,
	UpRight,
	DownRight,
	Down,
	DownLeft
}

/// <summary>
/// Axial direction of travel
/// </summary>
public enum AxialDirection {
	Right,
	Left
}

namespace EnumExtension
{
	public static class HexBoardDirectionExtensions
	{
		public static HexBoardDirection Opposite(this HexBoardDirection direction)
		{
			switch (direction)
			{
				case HexBoardDirection.UpLeft:
					return HexBoardDirection.DownRight;
				case HexBoardDirection.Up:
					return HexBoardDirection.Down;
				case HexBoardDirection.UpRight:
					return HexBoardDirection.DownLeft;
				case HexBoardDirection.DownRight:
					return HexBoardDirection.UpLeft;
				case HexBoardDirection.Down:
					return HexBoardDirection.Up;
				case HexBoardDirection.DownLeft:
					return HexBoardDirection.UpRight;
			}

			return HexBoardDirection.Up;
		}
	}
}
