#pragma once

#include "SpriteRenderer.hpp"
#include "TextRenderer.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace game {

    /* Represents the current state of the game */
    enum GameState {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN,
        GAME_LOOSE
    };

    /* Represents the four possible (collision) directions */
    enum class Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    /* The Width of the screen */
    const unsigned int SCREEN_WIDTH = 1000;
    /* The height of the screen */
    const unsigned int SCREEN_HEIGHT = 1000;

    const int NUM_COLUMN = 50;
    const int NUM_ROWS = 50;

    const unsigned int SQUARE_SIZE = SCREEN_WIDTH / NUM_COLUMN;

    struct gameObject {

        int x;
        int y;

    };

    struct snakePiece : public gameObject {
        /* Constructor */
        snakePiece(int xPos, int yPos, Direction dirObject);

        /* Direction of the piece */
        Direction dir;

        /* Method used to increase/decrease x or y according to the direction */
        void move();

    };

    class Game
    {
        /* game state */
        GameState m_state;

        /* Renderers */
        Utility::SpriteRenderer* Renderer;
        Utility::TextRenderer* Text;

        /* Snake and Apple objects */
        gameObject apple;
        std::vector<snakePiece> snake;

        /* Player score */
        int score;

    public:

        bool Keys[1024];

        /* constructor/destructor */
        Game();
        ~Game();

        /* initialize game state(load all shaders / textures ) */
        void Init(const std::string& shadersPath, const std::string& texturePath, const std::string& fontsPath);

        /* method used to process the inputs */
        bool ProcessInput(float dt);
        /* method used to update the movements */
        void Update(float dt);
        /* method used to render the game */
        void Render();

        /* method that check if two object collide */
        bool CheckCollision(const gameObject& one, const gameObject& two);

        /* method used to handle the collisions between the objects */
        void DoCollisions();

        /* method used to draw the objects */
        void drawSquare(int x, int y, std::string sprite);

        /* reset */
        void ResetLevel();
        void ResetPlayer();

        /* Getters */
        GameState getGameState() const { return m_state; }

    };

}
