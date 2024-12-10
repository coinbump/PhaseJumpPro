#include "gtest/gtest.h"
#include "Theme.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"
#include "SpacerView.h"

using namespace std;
using namespace PJ;

namespace ThemeTests {
    Theme const testTheme{Theme::Config{
        .colors = {
            { "red", Color::red },
            { "green", Color::green },
        },
            .elements = {
                {"background", Theme::ElementConfig{.colorId = "red"}},
                {"2", Theme::ElementConfig{.colorId = "green"}},
                {"alias", Theme::ElementConfig{.alias = "background"}},
            }
    }};

}

using namespace ThemeTests;

TEST(Theme, GetColors) {
    auto sut = testTheme;
    
    EXPECT_EQ(Color::red, sut.ThemeColor("background"));
    EXPECT_EQ(Color::green, sut.ThemeColor("2"));
}

TEST(Theme, ElementValues) {
    auto sut = testTheme;
    
    EXPECT_EQ(Color::red, sut.ElementTagValue<Color>("test", "color", Color::red));
    sut.SetElementTag<Color>("test", "color", Color::yellow);
    EXPECT_EQ(Color::yellow, sut.ElementTagValue<Color>("test", "color", Color::red));
}

TEST(Theme, Alias) {
    auto sut = testTheme;
    
    EXPECT_EQ(Color::red, sut.ThemeColor("alias"));
}
