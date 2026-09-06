#ifndef TYPES_H
#define TYPES_H

namespace game
{
    /* Represents the current state of the game */
    enum GameState {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN,
        GAME_LOOSE,
        GAME_PAUSE
    };

    /* The Width of the screen */
    const unsigned int SCREEN_WIDTH = 1000;
    /* The height of the screen */
    const unsigned int SCREEN_HEIGHT = 1000;

    const int NUM_COLUMN = 25;
    const int NUM_ROWS = 25;

    const unsigned int SQUARE_SIZE = SCREEN_WIDTH / NUM_COLUMN;

    /* Represents the four possible (collision) directions */
    enum class Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };
}

#endif