#include "include.h"

enum StageState {
    NORMAL_STATE,
    STAGE_CLEAR,
    GAME_OVER,
    PLAYER_DEAD
};

class StageStateHandler {   //To handle player's dead, stage clear, game over, etc.
private:
    static StageState state;
    StageStateHandler();
public:
    StageStateHandler(const StageStateHandler&) = delete;
    StageStateHandler& operator=(const StageStateHandler&) = delete;
    static StageStateHandler& GetInstance();
    static void SetState(StageState s);
    static StageState GetState();
};