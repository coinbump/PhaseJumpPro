#include "AttributedString.h"
#include <stack>

using namespace std;
using namespace PJ;

using This = AttributedString;

AttributedString::AttributedString(Config const& config) {
    parts.push_back({ .text = config.text });
}

void AttributedString::InsertPart(TextPart value, int pos) {
    String text;

    auto check = [&](int index) {
        auto& part = parts[index];
        auto partSize = part.text.size();
        GUARDR(partSize > 0, false)

        if (pos == 0) {
            parts.insert(parts.begin() + index, value);
            return true;
        } else if (pos < partSize) {
            auto leftSplit = part;
            auto rightSplit = part;
            leftSplit.text = part.text.substr(0, pos);
            rightSplit.text = part.text.substr(pos, part.text.size() - pos);

            parts[index] = leftSplit;
            parts.insert(parts.begin() + index + 1, rightSplit);
            parts.insert(parts.begin() + index + 1, value);
            return true;
        } else if (pos == partSize) {
            parts.insert(parts.begin() + index + 1, value);
            return true;
        }

        pos -= part.text.size();

        return false;
    };

    for (int i = 0; i < parts.size(); i++) {
        GUARD(!check(i))
    }
}

void AttributedString::ApplyAttribute(TextPart part, int start, int length) {
    GUARD(length > 0)

    TextPart startPart = part;
    startPart.attributeLength = length;

    InsertPart(startPart, start);
}

This& AttributedString::ApplyColor(Color value, int start, int length) {
    ApplyAttribute({ .type = TextPartType::AttributeColor, .color = value }, start, length);

    return *this;
}

This& AttributedString::RemoveAllAttributes() {
    SetPlainText(PlainText());
    return *this;
}

String AttributedString::PlainText() const {
    String result;

    for (auto& part : parts) {
        result += part.text;
    }

    return result;
}

void AttributedString::SetPlainText(String value) {
    parts = { TextPart{ .type = TextPartType::Text, .text = value } };
}

VectorList<TextPart> AttributedString::Attributes(int pos) const {
    std::stack<TextPart> attributeStack;

    VectorList<TextPart> activeParts;

    for (auto& part : parts) {
        if (part.IsAttribute()) {
            activeParts.push_back(part);
            continue;
        } else if (pos < part.text.size()) {
            VectorList<TextPart> result;

            for (auto& active : activeParts) {
                GUARD_CONTINUE(active.attributeLength > pos)
                result.push_back(active);
            }

            return result;
        }

        auto partTextSize = part.text.size();
        pos -= partTextSize;

        for (auto& active : activeParts) {
            active.attributeLength -= partTextSize;
        }
    }

    return {};
}

Color AttributedString::GetColor(int pos, Color defaultValue) const {
    auto attributes = Attributes(pos);
    auto colorAttributes =
        Filter(attributes, [](auto& part) { return part.type == TextPartType::AttributeColor; });

    return IsEmpty(colorAttributes) ? defaultValue : colorAttributes[0].color;
}
