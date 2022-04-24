using System;
using PJ.Graph;

/*
 * RATING: 5 stars
 * Simple model for maps
 * CODE REVIEW: 4/23/22/
 */
namespace PJ
{
	/// <summary>
    /// Some room on a map
    /// </summary>
	public abstract class SomeRoom : CyclicNode<StandardEdgeModel>
    {

    }

	/// <summary>
    /// Some map divided into discrete rooms
    /// </summary>
	public class DiscreteMapGraph : CyclicGraph<StandardEdgeModel>
	{
        public void AddRoom(SomeRoom room)
        {
            nodes.Add(room);
        }

        public void ConnectRooms(SomeRoom fromRoom, SomeRoom toRoom)
        {
            AddEdge(fromRoom, new StandardEdgeModel(), toRoom);
        }
	}
}
