#pragma once

#include "SpriteRenderer.hpp"
#include "TextRenderer.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <SFML/Audio.hpp>
#include "GameObject.hpp"
#include "GameLevel.hpp"
#include "SnakePiece.hpp"

namespace game {

    class Game
    {
        /* game state */
        GameState m_state;

        /* Renderers */
        Utility::SpriteRenderer* Renderer;
        Utility::TextRenderer* Text;

        /* Snake and Apple objects */
        GameObject apple;
        std::vector<SnakePiece> snake;

        /* Player score */
        int score;

        /* Current Level Time */
        float time;

        /* Sounds */
        sf::Music music;
        sf::SoundBuffer eatsfxBuffer;
        std::optional<sf::Sound> eatSfx;

        /* Levels */
        std::vector<GameLevel> Levels;
        unsigned int CurrentLevel;

        bool generateValidAppleCoord(int x, int y);

        int gamepadPresent = 0;

    public:

        bool Keys[1024];

        /* constructor/destructor */
        Game();
        ~Game();

        /* initialize game state(load all shaders / textures ) */
        void Init(const std::filesystem::path& shadersPath, const std::filesystem::path& texturePath, const std::filesystem::path& fontsPath, const std::filesystem::path& soundsPath, const std::filesystem::path& levelsPath);

        /* method used to process the inputs */
        bool ProcessInput(float dt);
        /* method used to update the movements */
        void Update(float dt);
        /* method used to render the game */
        void Render();

        /* method that check if two object collide */
        bool CheckCollision(const GameObject& one, const GameObject& two);

        /* method used to handle the collisions between the objects */
        void DoCollisions();

        /* method used to draw the objects */
        void drawSquare(int x, int y, std::string sprite);

        /* reset */
        void ResetGame();
        void ResetPlayer();

        /* Getters */
        GameState getGameState() const { return m_state; }

    };

}
