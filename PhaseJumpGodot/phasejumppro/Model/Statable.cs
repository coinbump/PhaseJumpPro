namespace PJ
{
    /// <summary>
    /// An object with a state
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public interface Statable<StateType>
    {
        public StateType State { get; set; }
    }
}
