using Godot;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple extension to standard Node class
 * GODOT CODE REVIEW: 12/30/23
 */
namespace PJ
{
    /// <summary>
    /// Adds common patterns to the standard Node type
    /// Example: Start, OnUpdate, etc.
    /// For component based versions of Phase Jump, this is called WorldComponent
    /// </summary>
    public partial class WorldNode2D : Node2D, SomeWorldNode
    {
        /// <summary>
        /// Updatables are sent update events (for timers, animations, etc.)
        /// </summary>
        public HashSet<Updatable> updatables = new();

        public override void _Ready()
        {
            base._Ready();

            Awake();

            GD.Print("Ready");

            // TODO: unit test
            // Call Start for all nodes from root after all nodes have received Awake
            if (IsSceneRoot)
            {
                GD.Print("IsRoot");

                var nodes = new List<Node>();
                AddFromAndAllChildrenFrom(this, nodes);

                foreach (var child in nodes)
                {
                    if (child is SomeWorldNode worldNode)
                    {
                        worldNode.Start();
                    }
                }
            }
        }

        // TODO: unit test
        protected static void AddFromAndAllChildrenFrom(Node from, List<Node> result)
        {
            result.Add(from);

            foreach (var child in from.GetChildren())
            {
                AddFromAndAllChildrenFrom(child, result);
            }
        }

        public virtual void Start()
        {
            GD.Print("Start");
        }

        public bool IsSceneRoot
        {
            get
            {
                var sceneRoot = GetTree().CurrentScene;
                return this == sceneRoot;
            }
        }

        public override void _Process(double delta)
        {
            base._Process(delta);

            OnUpdate(new TimeSlice((float)delta));
        }

        public override void _PhysicsProcess(double delta)
        {
            base._PhysicsProcess(delta);

            OnFixedUpdate(new TimeSlice((float)delta));
        }

        protected virtual void Awake()
        {
        }

        public virtual void OnFixedUpdate(TimeSlice time)
        {
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            // Copy because collection can be modified during loop
            var iterUpdatables = new HashSet<Updatable>(updatables);

            foreach (Updatable updatable in iterUpdatables)
            {
                updatable.OnUpdate(time);
            }

            // Don't flush until update is finished (new updatables might have been added)
            FlushFinishedUpdatables();
        }

        protected void FlushFinishedUpdatables()
        {
            var unfinishedUpdatables = new HashSet<Updatable>();

            foreach (Updatable updatable in updatables)
            {
                if (!updatable.IsFinished)
                {
                    unfinishedUpdatables.Add(updatable);
                }
            }

            updatables = unfinishedUpdatables;
        }
    }
}
