#include "MediaTrackView.h"
#include "MediaTrackViewLayout.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

MediaTrackView::MediaTrackView() {
    UP<SomeViewLayout> layout = NEW<MediaTrackViewLayout>();
    SetLayout(layout);
}

void MediaTrackView::RegisterClipView(SP<MediaClipModel> model, WorldNode* node) {
    GUARD(model && node)
    clipViewItems.push_back({ .model = model, .node = node });
    SetNeedsLayout();
}

void MediaTrackView::UnregisterClipView(MediaClipModel const& model) {
    for (auto iter = clipViewItems.begin(); iter != clipViewItems.end(); ++iter) {
        if (iter->model.get() == &model) {
            if (iter->node) {
                iter->node->Destroy();
            }
            clipViewItems.erase(iter);
            SetNeedsLayout();
            return;
        }
    }
}

MediaTrackView::ClipViewItem const* MediaTrackView::EntryForNode(WorldNode const* node) const {
    for (auto& entry : clipViewItems) {
        if (entry.node == node) {
            return &entry;
        }
    }
    return nullptr;
}

MediaTrackView::ClipViewItem const* MediaTrackView::EntryForModel(MediaClipModel const& model
) const {
    for (auto& entry : clipViewItems) {
        if (entry.model.get() == &model) {
            return &entry;
        }
    }
    return nullptr;
}
