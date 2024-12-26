#pragma once

#include "include.h"

class MouseClickToEmptySpaceHandler{
private:
    static MouseClickToEmptySpaceHandler* instance;

    static bool isTrue;

    MouseClickToEmptySpaceHandler();
    MouseClickToEmptySpaceHandler(const MouseClickToEmptySpaceHandler&) = delete;
    MouseClickToEmptySpaceHandler& operator=(const MouseClickToEmptySpaceHandler&) = delete;
public:
    static MouseClickToEmptySpaceHandler& GetInstance();

    static void SetIsTrue(bool value);
    static bool GetIsTrue();
};