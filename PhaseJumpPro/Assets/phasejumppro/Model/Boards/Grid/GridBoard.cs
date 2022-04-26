using UnityEngine;
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

        protected Vector3Int size = new Vector3Int(0, 0, 0);
        protected List<GridLayer<Tile>> layers = new List<GridLayer<Tile>>();
        protected Broadcaster broadcaster = new Broadcaster();

        public int Width => size.x;
        public int Height => size.y;
        public int Depth => size.z;

        public bool PutTile(Tile tile, Vector3Int loc)
        {
            if (!IsValidLoc(loc)) { return false; }
            if (null == tile) { return false; }

            if (tile.size.x < 1 || tile.size.y < 1)
            {
                Debug.LogWarningFormat("ERROR. Invalid tile size {0}, {1}", tile.size.x, tile.size.y);
                return false;
            }

            Rect2Int tileBounds = DestTileBounds(tile, loc);
            if (IsBlocked(tileBounds, loc.z))
            {
                return false;
            }

            var layer = layers[loc.z];

            // Store all tiles here for easy iteration.
            layer.tiles.Add(tile);

            tile.board = new WeakReference<SomeGridBoard>(this);
            tile.position = new Vector2Int(loc.x, loc.y);

            // A tile might extend across several cells, fill them all
            for (int x = tileBounds.position.x; x < (tileBounds.position.x + tileBounds.size.x); x++)
            {
                for (int y = tileBounds.position.y; y < (tileBounds.position.y + tileBounds.size.y); y++)
                {
                    CellAt(new Vector3Int(x, y, loc.z)).tile = tile;
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

            Vector3Int loc = new Vector3Int(tile.position.x, tile.position.y, layer);
            Rect2Int tileBounds = DestTileBounds(tile, loc);
            tile.board = null;

            int depth = loc.z;

            for (int x = tileBounds.position.x; x < (tileBounds.position.x + tileBounds.size.x); x++)
            {
                for (int y = tileBounds.position.y; y < (tileBounds.position.y + tileBounds.size.y); y++)
                {
                    CellAt(new Vector3Int(x, y, depth)).tile = null;
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
        public Rect2Int DestTileBounds(Tile tile, Vector3Int loc)
        {
            Rect2Int result = new Rect2Int(new Vector2Int(loc.x, loc.y), new Vector2Int(tile.size.x, tile.size.y));
            return result;
        }

        public virtual GridCell<Tile> NewCell() { return new GridCell<Tile>(); }
        public virtual GridLayer<Tile> NewLayer(Vector2Int size) { return new GridLayer<Tile>(size); }

        public GridCell<Tile> CellAt(Vector3Int loc)
        {
            if (!IsValidLoc(loc)) { return null; }

            GridCell<Tile> result;
            GridLayer<Tile> layer = layers[loc.z];

            Vector2Int position = new Vector2Int(loc.x, loc.y);

            result = layer.CellAt(position);
            if (null == result)
            {
                result = NewCell();
                result.position = position;
                layer.SetCell(position, result);
            }

            return result;
        }

        public bool IsCellBlocked(Vector3Int loc)
        {
            if (!IsValidLoc(loc)) { return true; }
            return layers[loc.z].IsCellBlocked(new Vector2Int(loc.x, loc.y));
        }

        public bool IsBlocked(Rect2Int bounds, int depth)
        {
            if (!IsValidLoc(new Vector3Int(0, 0, depth))) { return true; }
            return layers[depth].IsBlocked(bounds);
        }

        public Tile TileAt(Vector3Int loc)
        {
            GridCell<Tile> cell = CellAt(loc);
            if (null != cell)
            {
                return cell.tile;
            }
            return null;
        }

        public void Resize(Vector3Int newSize)
        {
            var gridSize = new Vector2Int(newSize.x, newSize.y);
            var newLayerCount = System.Math.Max(newSize.z, 1);

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

        public bool IsValidLoc(Vector3Int loc)
        {
            if (loc.x < 0 || loc.y < 0 || loc.x >= size.x || loc.y >= size.y)
            {
                return false;
            }
            if (loc.z < 0 || loc.z >= Depth)
            {
                return false;
            }

            return true;
        }

        public bool IsRowEmpty(Vector3Int _loc)
        {
            /* HAS UNIT TEST */
            for (int x = 0; x < Width; x++)
            {
                Vector3Int loc = new Vector3Int(x, _loc.y, _loc.z);
                if (null != TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public bool IsColumnEmpty(Vector3Int col)
        {
            /* HAS UNIT TEST */
            for (int y = 0; y < Height; y++)
            {
                Vector3Int loc = col;
                loc.y = y;
                if (null != TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public bool IsRowFull(Vector3Int row)
        {
            /* HAS UNIT TEST */
            for (int x = 0; x < Width; x++)
            {
                Vector3Int loc = row;
                loc.x = x;
                if (null == TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public bool IsColumnFull(Vector3Int col)
        {
            /* HAS UNIT TEST */
            for (int y = 0; y < Height; y++)
            {
                Vector3Int loc = col;
                loc.y = y;
                if (null == TileAt(loc))
                {
                    return false;
                }
            }
            return true;
        }

        public int ColumnTilesCount(Vector3Int loc)
        {
            /* HAS UNIT TEST */
            HashSet<Tile> foundTiles = new HashSet<Tile>();

            for (int y = 0; y < Height; y++)
            {
                Tile tile = TileAt(new Vector3Int(loc.x, y, loc.z));
                if (null == tile)
                {
                    continue;
                }
                foundTiles.Add(tile);
                y += tile.size.y - 1;
            }

            return foundTiles.Count;
        }

        public int RowTilesCount(Vector3Int loc)
        {
            /* HAS UNIT TEST */
            HashSet<Tile> foundTiles = new HashSet<Tile>();

            for (int x = 0; x < Width; x++)
            {
                Tile tile = TileAt(new Vector3Int(x, loc.y, loc.z));
                if (null == tile)
                {
                    continue;
                }
                foundTiles.Add(tile);
                x += tile.size.x - 1;
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

        public HashSet<Tile> CollectNeighbors(Vector3Int loc)
        {
            var neighbors = new HashSet<Tile>();
            var matrixAxial = new MatrixAxial();

            for (int i = 0; i < matrixAxial.AxialDirectionsCount; i++)
            {
                Vector2Int axialOffset = MatrixAxial.NeighborAxialLocs[i];

                var loc2 = matrixAxial.OffsetLoc(new Vector2Int(loc.x, loc.y), axialOffset);
                Vector3Int neighborLoc = new Vector3Int(loc2.x, loc2.y, loc.z);

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
