#pragma once

#include <PhaseJump/PhaseJump.h>

class TestInput : public WorldComponent<>, public SomeKeyUIEventsResponder, public SomeInputActionEventResponder {
    void OnKeyDown(KeyDownUIEvent event) override {
        std::cout << "Key: " << (char)event.keyCode.value << std::endl;

        GUARD(owner)
        owner->transform.value.position.x += 10;
    }

    void OnInputAction(InputActionEvent const& event) override {
        std::cout << "Action: " << event.action << std::endl;

//        if (event.action == "left") {
//            owner->transform.value.position.x -= 10;
//        } else if (event.action == "right") {
//            owner->transform.value.position.x += 10;
//        }
    }

    void OnUpdate(TimeSlice time) override {
        // TODO: support reading from any controller
        {
            auto value = Input::FirstAxisValue(ControllerAxisId::LeftX);
            if (value < -0.5f) {
                owner->transform.value.position.x -= 600.0f * time.delta;
                owner->TypeComponent<AnimatedSpriteRenderer>()->flipX = false;
            } else if (value > 0.5f) {
                owner->transform.value.position.x += 600.0f * time.delta;
                owner->TypeComponent<AnimatedSpriteRenderer>()->flipX = true;
            }
        }
        {
            auto value = Input::FirstAxisValue(ControllerAxisId::LeftY);
            if (value < -0.5f) {
                owner->transform.value.position.y += 600.0f * time.delta;
                owner->TypeComponent<AnimatedSpriteRenderer>()->flipX = false;
            } else if (value > 0.5f) {
                owner->transform.value.position.y -= 600.0f * time.delta;
                owner->TypeComponent<AnimatedSpriteRenderer>()->flipX = true;
            }
        }
    }
};
