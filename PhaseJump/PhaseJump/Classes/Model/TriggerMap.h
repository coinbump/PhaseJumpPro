#pragma once

#include "List.h"
#include "StringUtils.h"
#include "Transformer.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/6/24
 */
namespace PJ {
    class SomeUIEvent;

    /// Abstract interface for object that produces N output results for a given input
    template <class Input, class Output>
    class SomeMultiMap : public SomeTransformer<Input, std::optional<Output>> {
    public:
        using InputType = Input;
        using OutputType = Output;

        virtual ~SomeMultiMap() {}

        virtual std::optional<Output> FirstOutputFor(Input const& input) = 0;

        virtual List<Output> OutputsFor(Input const& input) {
            List<Output> result;

            auto first = FirstOutputFor(input);
            if (first) {
                result.push_back(first.value());
            }

            return result;
        }

        // MARK: SomeTransformer

        std::optional<Output> Transform(Input input) override {
            return FirstOutputFor(input);
        }
    };

    /// A map of actions and  filter funcs that trigger those actions
    template <class Trigger, class Action = String>
    class TriggerMap : public SomeMultiMap<Trigger, Action> {
    public:
        using FilterFunc = std::function<bool(Trigger const&)>;
        using FilterList = List<FilterFunc>;

        struct Item {
            Action action;
            FilterList filters;

            Item(Action action, FilterList filters) :
                action(action),
                filters(filters) {}
        };

        using ItemList = List<Item>;

    protected:
        ItemList items;

    public:
        virtual ~TriggerMap() {}

        size_t ActionsCount() const {
            return items.size();
        }

        /// Adds an action
        ItemList::iterator AddAction(Action action) {
            // FUTURE: this is inefficient. Optimize with hash/map if needed
            typename ItemList::iterator found =
                FirstIterator(items, [=](Item const& item) { return item.action == action; });
            GUARDR(found == items.end(), found)

            return items.emplace(items.end(), Item{ action, FilterList{} });
        }

        /// Adds a trigger filter for the action
        void AddTrigger(Action action, FilterFunc filterFunc) {
            AddAction(action)->filters.push_back(filterFunc);
        }

        /// Returns the first action for this trigger
        std::optional<Action> FirstActionFor(Trigger const& event) {
            for (auto& item : items) {
                for (auto& filter : item.filters) {
                    if (filter(event)) {
                        return item.action;
                    }
                }
            }

            return std::nullopt;
        }

        // TODO: Unit test
        //        FilterList FiltersForAction(Action action) const {
        //            auto& item = Find(items, [=](Item& item) { return item.action == action; });
        //            GUARDR(item != items.end(), FilterList())
        //
        //            return item.filters;
        //        }

        /// Returns a list of all actions that are activated by this trigger
        List<Action> ActionsFor(Trigger const& event) {
            List<Action> result;

            for (auto& item : items) {
                for (auto& filter : item.filters) {
                    if (filter(event)) {
                        result.push_back(item.action);
                    }
                }
            }

            return result;
        }

        // MARK: SomeMultiMap

        std::optional<Action> FirstOutputFor(Trigger const& input) override {
            return FirstActionFor(input);
        }

        List<Action> OutputsFor(Trigger const& input) override {
            return ActionsFor(input);
        }
    };
} // namespace PJ
