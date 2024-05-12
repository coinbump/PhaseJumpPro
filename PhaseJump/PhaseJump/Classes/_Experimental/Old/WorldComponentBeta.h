#ifndef PJWORLDCOMPONENT_H
#define PJWORLDCOMPONENT_H

namespace PJ
{
    /// <summary>
    /// Modifiers can be attached to components to modify behavior
    /// Example: offset modifier attached to a layout component
    /// </summary>
    class Modifier
    {
    };

    /// <summary>
    /// Components can be attached to objects to provide behavior
    /// </summary>
    class Component
    {
    public:
        VectorList<SP<Modifier>> modifiers;
    };

    /// <summary>
    /// </summary>
    class WorldComponentBeta : public Component
    {
        /// <summary>
        /// Updatables are sent update events (for timers, animations, etc.)
        /// </summary>
        HashSet<Updatable> updatables = new HashSet<Updatable>();

//        T ParentComponent<T>() where T : class {
//            var parent = transform.parent.gameObject;
//            if (null == parent) { return null; }
//            return parent.TypeComponent<T>();
//        }

        virtual void OnUpdate(TimeSlice time) {
            // Copy because collection can be modified during loop
            // TODO: should this be List or Set?
            var iterUpdatables = new HashSet<Updatable>(updatables);

            for (auto updatable : iterUpdatables)
            {
                updatable.OnUpdate(time);
            }

            // Don't flush until update is finished (new updatables might have been added)
            FlushFinishedUpdatables();
        }

        virtual void FlushFinishedUpdatables() {
            var unfinishedUpdatables = new HashSet<Updatable>();

            for (auto updatable : updatables)
            {
                if (!updatable.IsFinished) {
                    unfinishedUpdatables.Add(updatable);
                }
            }

            updatables = unfinishedUpdatables;
        }
    };
}

#endif
