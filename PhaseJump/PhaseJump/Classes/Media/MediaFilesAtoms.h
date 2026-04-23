#pragma once

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/4/25
 */
namespace PJ {
    /// Utilities for dealing with ISOBMFF media files (.mov, .mp4, etc.)
    namespace MediaFiles {
        /// Default storage for a media atom
        struct AtomHeader32 {
            uint32_t size{};
            std::array<char, 4> type{};
        };

        /**
         Storage for a media atom that is > 4gb (past bounds of 32 bit size)

         The header size is always 1, followed by the type and actual 64 bit size
         */
        struct AtomHeader64 {
            // Always 1
            uint32_t size{};

            std::array<char, 4> type{};

            /// Actual size
            uint64_t largeSize{};
        };
    } // namespace MediaFiles

} // namespace PJ
