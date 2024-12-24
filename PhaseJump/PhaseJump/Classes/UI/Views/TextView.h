#pragma once

#include "AttributedString.h"
#include "Color.h"
#include "Font.h"
#include "StringUtils.h"
#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 11/29/24
 */
namespace PJ {
    class TextRenderer;

    /// Text view with ideal size logic
    class TextView : public View2D {
    public:
        using Base = View2D;
        using This = TextView;

        struct Config {
            Color color = Color::black;
            FontSpec fontSpec = { .size = 12 };
            AttributedString text;
        };

        void SetText(String value);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "TextView";
        }

    protected:
        WP<TextRenderer> renderer;
        Config config;

        TextRenderer* Renderer();

    public:
        TextView();
        TextView(Config config);

        void Configure(Config value);
    };
} // namespace PJ
