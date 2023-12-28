using Godot;
using System;
using System.Collections.Generic;

/*
 * RATING: 4 stars
 * Has unit tests. Could use more
 * CODE REVIEW: 4/16/22
 */
// FUTURE: add more unit tests as needed
namespace PJ
{
    public interface SomeGridBoard
    {
    }

    /// <summary>
    /// A grid board consists of N layers of X by Y grid cells
    /// </summary>
    public partial class GridBoard<Tile> : SomeGridBoard, Updatable where Tile : GridTile
    {
        struct Events
        {
            public const string TileAdded = "tile_added";
            public const string WillRemoveTile = "tile_will_remove";
            public const string TileRemoved = "tile_removed";
        }

        public bool suspendEvents;   // If true, don't broadcast events

        protected Vector3I size = new Vector3I(0, 0, 0);
        protected List<GridLayer<Tile>> layers = new List<GridLayer<Tile>>();
        protected Broadcaster broadcaster = new Broadcaster();

        public int Width => size.X;
        public int Height => size.Y;
        public int Depth => size.Z;

        public bool IsFinished { get => false; }

        public bool PutTile(Tile tile, Vector3I loc)
        {
            if (!IsValidLoc(loc)) { return false; }
            if (null == tile) { return false; }

            if (tile.size.X < 1 || tile.size.Y < 1)
            {
                GD.PrintErr("ERROR. Invalid tile size {0}, {1}", tile.size.X, tile.size.Y);
                return false;
            }

            Rect2I tileBounds = DestTileBounds(tile, loc);
            if (IsBlocked(tileBounds, loc.Z))
            {
                return false;
            }

            var layer = layers[loc.Z];

            // Store all tiles here for easy iteration.
            layer.tiles.Add(tile);

            tile.board = new WeakReference<SomeGridBoard>(this);
            tile.position = new Vector2I(loc.X, loc.Y);

            // A tile might extend across several cells, fill them all
            for (int x = tileBounds.position.X; x < (tileBounds.position.X + tileBounds.size.X); x++)
            {
                for (int y = tileBounds.position.Y; y < (tileBounds.position.Y + tileBounds.size.Y); y++)
                {
                    CellAt(new Vector3I(x, y, loc.Z)).tile = tile;
                }
            }
            layer.OnCellsBlocked(tileBounds);

            // Don't notify until tile's state is set (or wrong state will be encoded).
            if (!suspendEvents)
            {
                OnAddTile(tile);
            }

            return true;
        }

        public virtual void RemoveTile(Tile tile, int layer)
        {
            if (null == tile) { return; }
            if (!tile.board.TryGetTarget(out SomeGridBoard owner)) { return; }
            if (owner != this) { return; }

            if (!suspendEvents)
            {
                OnWillRemoveTile(tile);
            }

            Vector3I loc = new Vector3I(tile.position.X, tile.position.Y, layer);
            Rect2I tileBounds = DestTileBounds(tile, loc);
            tile.board = null;

            int depth = loc.Z;

            for (int x = tileBounds.position.X; x < (tileBounds.position.X + tileBounds.size.X); x++)
            {
                for (int y = tileBounds.position.Y; y < (tileBounds.position.Y + tileBounds.size.Y); y++)
                {
                    CellAt(new Vector3I(x, y, depth)).tile = null;
                }
            }
            layers[depth].tiles.Remove(tile);
            layers[depth].OnCellsUnblocked(tileBounds);

            if (!suspendEvents)
            {
                OnRemoveTile(tile);
            }
        }

        public virtual void RemoveAllTiles()
        {
            int layerIndex = 0;
            foreach (GridLayer<Tile> layer in layers)
            {
                // Avoid mutation error while iterating
                HashSet<Tile> iterTiles = new HashSet<Tile>(layer.tiles);
                foreach (Tile tile in iterTiles)
                {
                    RemoveTile(tile, layerIndex);
                }

                layerIndex++;
            }
        }
    }

    // Utilities

    public partial class GridBoard<Tile> : SomeGridBoard where Tile : GridTile
    {
        public Rect2I DestTileBounds(Tile tile, Vector3I loc)
        {
            Rect2I result = new Rect2I(new Vector2I(loc.X, loc.Y), new Vector2I(tile.size.X, tile.size.Y));
            return result;
        }

        public virtual GridCell<Tile> NewCell() { return new GridCell<Tile>(); }
        public virtual GridLayer<Tile> NewLayer(Vector2I size) { return new GridLayer<Tile>(size); }

        public GridCell<Tile> CellAt(Vector3I loc)
        {
            if (!IsValidLoc(loc)) { return null; }

            GridCell<Tile> result;
            GridLayer<Tile> layer = layers[loc.Z];

            Vector2I position = new Vector2I(loc.X, loc.Y);

            result = layer.CellAt(position);
            if (null == result)
            {
                result = NewCell();
                result.position = position;
                layer.SetCell(position, result);
            }

            return result;
        }

        public bool IsCellBlocked(Vector3I loc)
        {
            if (!IsValidLoc(loc)) { return true; }
            return layers[loc.Z].IsCellBlocked(new Vector2I(loc.X, loc.Y));
        }

        public bool IsBlocked(Rect2I bounds, int depth)
        {
            if (!IsValidLoc(new Vector3I(0, 0, depth))) { return true; }
            return layers[depth].IsBlocked(bounds);
        }

        public Tile TileAt(Vector3I loc)
        {
            GridCell<Tile> cell = CellAt(loc);
            if (null != cell)
            {
                return cell.tile;
            }
            return null;
        }

        public void Resize(Vector3I newSize)
        {
            var gridSize = new Vector2I(newSize.X, newSize.Y);
            var newLayerCount = System.Math.Max(newSize.Z, 1);

            // Add new layers
            while (layers.Count < newLayerCount)
            {
                GridLayer<Tile> layer = NewLayer(gridSize);
                layers.Add(layer);
            }

            // Remove old layers
            if (layers.Count > newLayerCount)
            {
                var diff = layers.Count - newLayerCount;
                layers.RemoveRange(layers.Count - diff, diff);
            }

            // Resize each layer
            foreach (GridLayer<Tile> layer in layers)
            {
                layer.Resize(gridSize);
            }

            size = newSize;
        }

        public bool IsValidLoc(Vector3I loc)
        {
            if (loc.X < 0 || loc.Y < 0 || loc.X >= size.X || loc.Y >= size.Y)
            {
                return false;
            }
            if (loc.Z < 0 || loc.Z >= Depth)
            {
                return false;
            }

            return true;
        }

        public bool IsRowEmpty(Vector3I _loc)
        {
            /* HAS UNIT TEST */
            for (int x = 0; x < Width; x++)
            {
                Vector3I loc = new Vector3I(x, _loc.Y, _loc.Z);
                if (null != TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public bool IsColumnEmpty(Vector3I col)
        {
            /* HAS UNIT TEST */
            for (int y = 0; y < Height; y++)
            {
                Vector3I loc = col;
                loc.Y = y;
                if (null != TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public bool IsRowFull(Vector3I row)
        {
            /* HAS UNIT TEST */
            for (int x = 0; x < Width; x++)
            {
                Vector3I loc = row;
                loc.X = x;
                if (null == TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public bool IsColumnFull(Vector3I col)
        {
            /* HAS UNIT TEST */
            for (int y = 0; y < Height; y++)
            {
                Vector3I loc = col;
                loc.Y = y;
                if (null == TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public int ColumnTilesCount(Vector3I loc)
        {
            /* HAS UNIT TEST */
            HashSet<Tile> foundTiles = new HashSet<Tile>();

            for (int y = 0; y < Height; y++)
            {
                Tile tile = TileAt(new Vector3I(loc.X, y, loc.Z));
                if (null == tile)
                {
                    continue;
                }
                foundTiles.Add(tile);
                y += tile.size.Y - 1;
            }

            return foundTiles.Count;
        }

        public int RowTilesCount(Vector3I loc)
        {
            /* HAS UNIT TEST */
            HashSet<Tile> foundTiles = new HashSet<Tile>();

            for (int x = 0; x < Width; x++)
            {
                Tile tile = TileAt(new Vector3I(x, loc.Y, loc.Z));
                if (null == tile)
                {
                    continue;
                }
                foundTiles.Add(tile);
                x += tile.size.X - 1;
            }

            return foundTiles.Count;
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            foreach (GridLayer<Tile> layer in layers)
            {
                HashSet<Tile> iterTiles = new HashSet<Tile>(layer.tiles);
                foreach (Tile tile in iterTiles)
                {
                    tile.OnUpdate(time);
                }
            }
        }

        protected virtual void OnAddTile(Tile tile)
        {
            if (null == broadcaster) { return; }
            broadcaster.Broadcast(new Event(Events.TileAdded, this));
        }

        protected virtual void OnWillRemoveTile(Tile tile)
        {
            if (null == broadcaster) { return; }
            broadcaster.Broadcast(new Event(Events.WillRemoveTile, this));
        }

        protected virtual void OnRemoveTile(Tile tile)
        {
            if (null == broadcaster) { return; }
            broadcaster.Broadcast(new Event(Events.TileRemoved, this));
        }

        public HashSet<Tile> CollectNeighbors(Vector3I loc)
        {
            var neighbors = new HashSet<Tile>();
            var matrixAxial = new MatrixAxial();

            for (int i = 0; i < matrixAxial.AxialDirectionsCount; i++)
            {
                Vector2I axialOffset = MatrixAxial.NeighborAxialLocs[i];

                var loc2 = matrixAxial.OffsetLoc(new Vector2I(loc.X, loc.Y), axialOffset);
                Vector3I neighborLoc = new Vector3I(loc2.X, loc2.Y, loc.Z);

                Tile neighbor = TileAt(neighborLoc);
                if (null != neighbor)
                {
                    neighbors.Add(neighbor);
                }
            }

            return neighbors;
        }
    }
}
