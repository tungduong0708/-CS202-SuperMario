#include "string_mode.h"

string StringMode::getMode(Mode mode)
{
    if (mode == Mode::SMALL) {
        return "small";
    }
    else {
        return "big";
    }
}