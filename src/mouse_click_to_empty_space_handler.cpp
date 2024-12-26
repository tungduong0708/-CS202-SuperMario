#include "mouse_click_to_empty_space_handler.h"

MouseClickToEmptySpaceHandler* MouseClickToEmptySpaceHandler::instance = nullptr;
bool MouseClickToEmptySpaceHandler::isTrue = false;

MouseClickToEmptySpaceHandler::MouseClickToEmptySpaceHandler() {
}

MouseClickToEmptySpaceHandler& MouseClickToEmptySpaceHandler::GetInstance() {
    if (instance == nullptr) {
        instance = new MouseClickToEmptySpaceHandler();
    }
    return *instance;
}

void MouseClickToEmptySpaceHandler::SetIsTrue(bool value) {
    isTrue = value;
}

bool MouseClickToEmptySpaceHandler::GetIsTrue() {
    return isTrue;
}

