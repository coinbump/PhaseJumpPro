#pragma once

#include "SomeSignal.h"
#include "StringUtils.h"
#include "UnorderedMap.h"
#include "Updatables.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Internal storage for signal handler
    template <class Owner>
    struct SignalHandler {
        using SignalFunc = std::function<void(Owner&, SomeSignal const&)>;

        String id;
        String name;
        SignalFunc func;

        void operator()(Owner& owner, SomeSignal const& signal) {
            GUARD(func)
            func(owner, signal);
        }
    };

    /// Standard core model for world attachments (component, system)
    template <class Owner>
    class WorldAttachmentCore {
    public:
        using SignalHandler = PJ::SignalHandler<Owner>;
        using SignalFunc = SignalHandler::SignalFunc;
        using SignalHandlerMap = UnorderedMap<String, VectorList<SignalHandler>>;

    protected:
        /// Signal handlers. Mapped by signal id
        SignalHandlerMap signalHandlers;

    public:
        Owner& owner;

        WorldAttachmentCore(Owner& owner) :
            owner(owner) {}

        /// Unique identifier
        String id;

        /// User facing name for display
        String name;

        /// Stores func for updates
        Updatable updatable;

        /// Stores objects that need time events
        Updatables updatables;

        SignalHandlerMap& SignalHandlers() {
            return signalHandlers;
        }

        VectorList<String> SignalHandlerIds() {
            VectorList<String> result;
            for (auto& item : signalHandlers) {
                VectorList<String> innerResult;
                std::transform(
                    item.second.begin(), item.second.end(), std::back_inserter(innerResult),
                    [](auto& item) { return item.id; }
                );

                AddRange(result, innerResult);
            }

            return result;
        }

        VectorList<String> SignalHandlerNames() {
            VectorList<String> result;
            for (auto& item : signalHandlers) {
                VectorList<String> innerResult;
                std::transform(
                    item.second.begin(), item.second.end(), std::back_inserter(innerResult),
                    [](auto& item) { return IsEmpty(item.name) ? item.id : item.name; }
                );

                AddRange(result, innerResult);
            }

            return result;
        }

        /// Called in main loop for time delta update events
        void OnUpdate(TimeSlice time) {
            updatable.OnUpdate(time);
            updatables.OnUpdate(time);
        }

        /// Adds a signal handler for the specified signal
        void AddSignalHandler(SignalHandler handler) {
            GUARD_LOG(!IsEmpty(handler.id), "ERROR. Signal handler is missing id")
            signalHandlers[handler.id].push_back(handler);
        }

        template <class Signal>
        struct SignalHandlerConfig {
            String id;
            std::function<void(Owner&, Signal const&)> func;
        };

        /// Adds a typesafe signal handler for the specified signal
        template <class Signal>
        void AddSignalHandler(SignalHandlerConfig<Signal> config) {
            GUARD_LOG(!IsEmpty(config.id), "ERROR. Signal handler is missing id")

            SignalFunc func = [=](auto& owner, auto& signal) {
                auto event = dynamic_cast<Signal const*>(&signal);
                GUARD(event);
                GUARD(config.func);
                config.func(owner, *event);
            };

            SignalHandler handler{ .id = config.id, .func = func };
            signalHandlers[handler.id].push_back(handler);
        }

        /// Sends a signal to registered signal handlers
        void Signal(String id, SomeSignal const& signal) {
            try {
                auto& handlers = signalHandlers.at(id);
                for (auto& handler : handlers) {
                    GUARD_CONTINUE(handler.func)
                    handler.func(owner, signal);
                }
            } catch (...) {}
        }
    };
} // namespace PJ
