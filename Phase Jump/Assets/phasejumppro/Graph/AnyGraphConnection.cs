namespace PJ
{
    /// <summary>
    /// Somes implementation of graph connection
    /// </summary>
    public interface AnyGraphConnection
    {
        AnyGraphEdge Edge { get; }
        AnyGraphNode Target { get; }
    }
}
