#include "stage_state_handler.h"

StageState StageStateHandler::state = NORMAL_STATE;

StageStateHandler::StageStateHandler() {}

StageStateHandler& StageStateHandler::GetInstance() {
    static StageStateHandler instance;
    return instance;
}

void StageStateHandler::SetState(StageState s) {
    state = s;
}

StageState StageStateHandler::GetState() {
    return state;
}