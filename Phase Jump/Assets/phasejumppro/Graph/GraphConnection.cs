namespace PJ
{
    /// <summary>
    /// Simple graph connection
    /// </summary>
    public struct GraphConnection : AnyGraphConnection
    {
        public AnyGraphEdge Edge { get; set; }
        public AnyGraphNode Target { get; set; }
    }
}
