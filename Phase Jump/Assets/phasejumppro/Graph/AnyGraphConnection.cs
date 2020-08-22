namespace PJ
{
    /// <summary>
    /// Abstracts implementation of graph connection
    /// </summary>
    public interface AnyGraphConnection
    {
        AnyGraphEdge Edge { get; }
        AnyGraphNode Target { get; }
    }
}
