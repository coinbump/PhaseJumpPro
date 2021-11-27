# Module: Model.Boards

Boards arrange game objects according to a format/rule. The most common board type is the grid: checkers, chess, Tetris, tile-based games, etc.

## Module: BoardTypes

**Link:** [BoardTypes](BoardTypes.md)

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
