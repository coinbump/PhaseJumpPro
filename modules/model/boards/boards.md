# Module: Model.Boards

**Parent:** [Model](../model.md)

Boards arrange game objects according to a format/rule. The most common board type is the grid: checkers, chess, Tetris, tile-based games, etc.

## Module: BoardTypes

**Link:** [BoardTypes](BoardTypes.md)


## Class: Grid

### Description

The most common board type for games, can be a hex or square grid.

### Public Methods

**Grid(Vector2Int size)**: Initalize the grid with a specific size.

**Row GetRow(int index)**: Grids are stored as rows. Eeach row is a list with X elements.

**Resize(Vector2Int newSize)**: Resize the grid and its underlying storage.

**bool IsValidLoc(Vector2Int loc)**: Returns true if the grid location is valid.

**T GetCell(Vector2Int loc)**: Returns type `T` at the specified grid location, or null if it doesn't exist.

**SetCell(Vector2Int loc, T content)**: Set the T value for the cell at the specified location.


## Class: GridCell

### Description

A grid board consists of cells, each cell records which tile is assigned to that cell. Large tiles can exist in multiple grid cells simultaneously. For overlapping tiles, use layers (z axis).

### Stability

Stable.


## Class: GridTile

### Description

A grid tile can be placed in a grid board, and will occupy 1 or more grid cells.

### Limitations

Currently only square shaped tiles are supported.

### Future Improvements

Support non-square tiles, such as Tetronimos.

### Stability

Stable.
