#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 10/27/24
 */
namespace PJ {
    /// Used to turn on extra "diagnose" logging for objects
    class DiagnoseModel {
    protected:
        bool isEnabled{};

    public:
        DiagnoseModel(bool isEnabled = false) :
            isEnabled(isEnabled) {}

        operator bool() const {
            return isEnabled;
        }

        void Enable(bool value) {
            isEnabled = value;
        }

        /// Turns on diagnose flag if we're in DEBUG mode
        void EnableIfDebug() {
#ifdef DEBUG
            Enable(true);
#else
            Enable(false);
#endif
        }
    };
} // namespace PJ
