#include "SDKIncludes.h"

class KaijuImGuiRenderProcessor;

class KaijuWorldSystem : public EditorWorldSystem {
public:
    // FUTURE: Each scene can have its own menus
    /// Default menus for app, independent of current scene
    VectorList<UP<Menu>> defaultMenus;

    KaijuWorldSystem();

    void RegisterSceneClasses();

    // MARK: SomeWorldSystem
    void ProcessUIEvents(UIEventList const& uiEvents) override;
};
