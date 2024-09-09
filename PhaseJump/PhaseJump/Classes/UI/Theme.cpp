#include "Theme.h"

using namespace std;
using namespace PJ;

// FUTURE: support theme files (CSV or JSON)
// FUTURE: support dark mode
Theme const Themes::fruit{ {
                               { "red", Color32(255, 59, 48) },
                               { "orange", Color32(255, 149, 0) },
                               { "yellow", Color32(255, 204, 0) },
                               { "green", Color32(52, 199, 89) },
                               { "mint", Color32(0, 199, 190) },
                               { "teal", Color32(48, 176, 199) },
                               { "cyan", Color32(50, 173, 230) },
                               { "blue", Color32(0, 122, 255) },
                               { "indigo", Color32(88, 86, 214) },
                               { "purple", Color32(175, 82, 222) },
                               { "pink", Color32(255, 45, 85) },
                               { "brown", Color32(162, 132, 94) },
                               { "themeGray", Color32(142, 142, 147) },
                               { "themeGray2", Color32(174, 174, 178) },
                               { "themeGray3", Color32(199, 199, 204) },
                               { "themeGray4", Color32(209, 209, 214) },
                               { "themeGray5", Color32(229, 229, 234) },
                               { "themeGray6", Color32(242, 242, 247) },
                           },
                           // TODO: these semantic colors are just guesses for now
                           {
                               { "label", "themeGray" },
                               { "secondaryLabel", "themeGray2" },
                               { "tertiaryLabel", "themeGray3" },
                               { "quaternaryLabel", "themeGray4" },
                               { "themeBackground", "themeGray6" },
                               { "secondaryThemeBackground", "themeGray5" },
                               { "tertiaryThemeBackground", "themeGray4" },
                               { "themeGroupedBackground", "themeGray4" },
                               { "secondaryThemeGroupedBackground", "themeGray5" },
                               { "tertiaryThemeGroupedBackground", "themeGray6" },
                               { "lightText", "themeGray6" },
                               { "darkText", "themeGray" },
                               { "themeFill", "themeGray4" },
                               { "secondaryThemeFill", "themeGray3" },
                               { "tertiaryThemeFill", "themeGray2" },
                               { "quaternaryThemeFill", "themeGray" },
                           },
                           // TODO: these are just placeholders for now
                           {
                               { "handle.fill", { "teal" } },
                               { "handle.frame", { "purple" } },
                           } };
