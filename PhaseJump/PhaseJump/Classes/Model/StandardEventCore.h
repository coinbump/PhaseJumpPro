#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Standard event model
    /// Use this or create your own
    struct StandardEventCore {
    public:
        // OPTIONAL:
        // TODO: seems weird. Can we get rid of this?
        WP<Base> sentFrom;

        /// Event id
        String id;

        StandardEventCore() {}

        StandardEventCore(String id) :
            id(id) {}

        StandardEventCore(String id, WP<Base> sentFrom) :
            id(id),
            sentFrom(sentFrom) {}
    };
} // namespace PJ
