# Module: Model.Boards

**Parent:** [Model](../model.md)

Boards arrange game objects according to a format/rule. The most common board type is the grid: checkers, chess, Tetris, tile-based games, etc.

## Module: BoardTypes

**Link:** [BoardTypes](BoardTypes.md)


## Template Class: Grid\<T>

### Description

A model object for storing objects in grids. Each grid is composed of Y List rows, each of which contains X elements.

### Public Methods

**Grid(Vector2Int size)**: Initalize the grid with a specific size.

**Row GetRow(int index)**: Grids are stored as rows. Eeach row is a list with X elements.

**Resize(Vector2Int newSize)**: Resize the grid and its underlying storage.

**bool IsValidLoc(Vector2Int loc)**: Returns true if the grid location is valid.

**T GetCell(Vector2Int loc)**: Returns type `T` at the specified grid location, or null if it doesn't exist.

**SetCell(Vector2Int loc, T content)**: Set the T value for the cell at the specified location.

### Stability

Stable, with Unit Tests.


## Class: GridBoard

Model for a grid with multiple layers.

**Link:** [GridBoard](GridBoard.md)


## Template Class: GridCell\<Tile>

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



## Template Class: GridLayer\<Tile>

### Description

Specialization of Grid that uses `GridCell` and `GridTile`. Used by `GridBoard` to define the layers of a multi-layer grid.

### Public Methods

**bool IsCellBlocked(Vector2Int loc)**: Returns true if the cell at the specified location is occupied by a `GridTile`.

**bool IsBlocked(Rect2Int bounds)**: Returns true if any of the cells covered by the Rectangle are occupied by a `GridTile`.

### Stability

Stable.