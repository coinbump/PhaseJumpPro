#pragma once

#include "MultiFunction.h"
#include "SomePosition.h"
#include "SomeWorldComponent.h"
#include "StandardCore.h"
#include "Updatable.h"
#include <memory>

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    class World;
    template <class Core>
    class WorldComponent;

    /// No-op component core
    class VoidWorldComponentCore : public StandardCore {
    public:
        using Owner = WorldComponent<VoidWorldComponentCore>;

        String TypeName() const {
            return "VoidWorldComponentCore";
        }

        void Awake(Owner& component) {}

        void Start(Owner& component) {}

        void LateUpdate(Owner& component) {}

        void OnUpdate(Owner& component, TimeSlice time) {}
    };

    /// Component core that uses function
    class FuncWorldComponentCore : public StandardCore {
    public:
        using Owner = WorldComponent<FuncWorldComponentCore>;
        using VoidFunc = std::function<void(Owner&)>;
        using OnUpdateFunc = std::function<void(Owner&, TimeSlice)>;

        VoidFunc awakeFunc;
        VoidFunc startFunc;
        VoidFunc lateUpdateFunc;
        OnUpdateFunc onUpdateFunc;

        String TypeName() const {
            return "FuncWorldComponentCore";
        }

        void Awake(Owner& component) {
            GUARD(awakeFunc)
            awakeFunc(component);
        }

        void Start(Owner& component) {
            GUARD(startFunc)
            startFunc(component);
        }

        void LateUpdate(Owner& component) {
            GUARD(lateUpdateFunc)
            lateUpdateFunc(component);
        }

        void OnUpdate(Owner& component, TimeSlice time) {
            GUARD(onUpdateFunc)
            onUpdateFunc(component, time);
        }
    };

    /// Component core that uses MultiFunction
    class MultiFuncWorldComponentCore : public StandardCore {
    public:
        using Owner = WorldComponent<MultiFuncWorldComponentCore>;
        using VoidMultiFunc = MultiFunction<void(Owner&)>;
        using OnUpdateMultiFunc = MultiFunction<void(Owner&, TimeSlice)>;

    protected:
        VoidMultiFunc awakeFuncs;
        VoidMultiFunc startFuncs;
        VoidMultiFunc lateUpdateFuncs;
        OnUpdateMultiFunc onUpdateFuncs;

    public:
        String TypeName() const {
            return "MultiFuncWorldComponentCore";
        }

        VoidMultiFunc& AwakeFuncs() {
            return awakeFuncs;
        }

        VoidMultiFunc& StartFuncs() {
            return startFuncs;
        }

        VoidMultiFunc& LateUpdateFuncs() {
            return lateUpdateFuncs;
        }

        OnUpdateMultiFunc& OnUpdateFuncs() {
            return onUpdateFuncs;
        }

        void Awake(Owner& component) {
            awakeFuncs.Run(component);
        }

        void Start(Owner& component) {
            startFuncs.Run(component);
        }

        void LateUpdate(Owner& component) {
            lateUpdateFuncs.Run(component);
        }

        void OnUpdate(Owner& component, TimeSlice time) {
            onUpdateFuncs.Run(component, time);
        }
    };
} // namespace PJ
