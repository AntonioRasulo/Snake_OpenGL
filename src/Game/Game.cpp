#include "Game.hpp"
#include "ResourceManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <random>
#include <sstream>

namespace game {

    int generateRandomCoordinate(int max);
    glm::vec2 generatePosition(int x, int y);
    bool checkAppleCoordinateValidity(int x, int y, const std::vector<snakePiece>& snakePieces);

    snakePiece::snakePiece(int xPos, int yPos, Direction dirObject) {
        x = xPos;
        y = yPos;
        dir = dirObject;
    }

    void snakePiece::move() {

        switch (dir) {
        case(Direction::UP):
            y--;
            break;
        case(Direction::RIGHT):
            x++;
            break;
        case(Direction::LEFT):
            x--;
            break;
        case(Direction::DOWN):
            y++;
            break;
        }

        if (y < 0) {
            y = NUM_ROWS - 1;
        }
        else if (y > NUM_ROWS - 1) {
            y = 0;
        }
        else if (x > NUM_COLUMN - 1) {
            x = 0;
        }
        else if (x < 0) {
            x = NUM_COLUMN - 1;
        }

    }

    Game::Game()
        : m_state(GAME_MENU), Keys(), Renderer(nullptr), Text(nullptr), score(0), apple(), snake()
    {

    }

    Game::~Game()
    {
        delete Renderer;
        delete Text;
    }
    
    void Game::Init(const std::string& shadersPath, const std::string& texturePath, const std::string& fontsPath)
    {
        const std::string vertexCode = shadersPath + "\\sprite.vs";
        const std::string fragmentCode = shadersPath + "\\sprite.frag";

        std::string applePNG = texturePath + "\\apple.png";
        std::string snakePNG = texturePath + "\\snake.png";
        std::string background = texturePath + "\\background.jpg";
        std::string ocra = fontsPath + "\\OCRAEXT.TTF";

        /* Text renderer */
        Text = new Utility::TextRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, shadersPath);
        try {
            Text->Load(ocra, 24);
        }
        catch (std::runtime_error& ex) {
            throw ex;
        }

        /* load shaders */
        Utility::ResourceManager::LoadShader(vertexCode.c_str(), fragmentCode.c_str(), nullptr, "sprite");
        /* configure shaders */
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
        Utility::ResourceManager::GetShader("sprite").Use().setInt("image", 0);
        Utility::ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
        /* set render-specific controls */
        Renderer = new Utility::SpriteRenderer(Utility::ResourceManager::GetShader("sprite"));
        /* load textures */
        Utility::ResourceManager::LoadTexture(applePNG.c_str(), true, "apple");
        Utility::ResourceManager::LoadTexture(snakePNG.c_str(), true, "snake");
        Utility::ResourceManager::LoadTexture(background.c_str(), false, "background");

        /* Reset Snake and Apple position */
        ResetPlayer();

    }

    void Game::Update(float dt)
    {

        if (GAME_ACTIVE == m_state)
        {
            /* Check for collisions */
            DoCollisions();

            /* Move every piece of the snake */
            for (snakePiece& piece : snake) {
                piece.move();
            }

            /* Every piece follow the one in front */
            for (int i = snake.size() - 1; i != 0; i--) {
                snake[i].dir = snake[i - 1].dir;
            }
        }

    }

    bool Game::ProcessInput(float dt)
    {

        switch (m_state) {
        case(GAME_ACTIVE):
            if (Keys[GLFW_KEY_W] && Direction::DOWN != snake[0].dir && Direction::UP != snake[0].dir) {
                snake[0].dir = Direction::UP;
                return true;
            }
            else if (Keys[GLFW_KEY_A] && Direction::RIGHT != snake[0].dir && Direction::LEFT != snake[0].dir) {
                snake[0].dir = Direction::LEFT;
                return true;
            }
            else if (Keys[GLFW_KEY_S] && Direction::UP != snake[0].dir && Direction::DOWN != snake[0].dir) {
                snake[0].dir = Direction::DOWN;
                return true;
            }
            else if (Keys[GLFW_KEY_D] && Direction::LEFT != snake[0].dir && Direction::RIGHT != snake[0].dir) {
                snake[0].dir = Direction::RIGHT;
                return true;
            }

            break;
        case(GAME_MENU):
            if (Keys[GLFW_KEY_SPACE]) {
                m_state = GAME_ACTIVE;
            }
            break;

        case(GAME_LOOSE):
        case(GAME_WIN):
            if (Keys[GLFW_KEY_SPACE]) {
                ResetLevel();
            }
            break;
        }

        return false;

    }

    bool Game::CheckCollision(const gameObject& one, const gameObject& two)
    {

        bool collisionX = one.x == two.x;
        bool collisionY = one.y == two.y;

        return collisionX && collisionY;
    }

    void Game::Render()
    {

        /* draw background */
        Renderer->DrawSprite(Utility::ResourceManager::GetTexture("background"),
            glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.0f
        );

        switch (m_state)
        {
        case(GAME_ACTIVE): {
            /* Draw apple and snake */
            drawSquare(apple.x, apple.y, "apple");
            for (snakePiece& piece : snake) {
                drawSquare(piece.x, piece.y, "snake");
            }
            /* Draw the score */
            std::stringstream ss; ss << score;
            Text->RenderText("Score: " + ss.str(), 5.0f, 5.0f, 1.0f);

            break;
        }
        case(GAME_MENU):
            Text->RenderText("Press Spacebar to start", 400.0f, SCREEN_HEIGHT / 2, 1.0f);
            break;

        case(GAME_WIN):
            Text->RenderText("Congratulation! You Win!", 400.0f, SCREEN_HEIGHT / 2, 1.0f);
            Text->RenderText("Press Spacebar to start a new game", 400.0f, (SCREEN_HEIGHT + 50.0f) / 2, 1.0f);
            break;
        case(GAME_LOOSE):

            Text->RenderText("You have lost", 400.0f, SCREEN_HEIGHT / 2, 1.0f);
            std::stringstream ss; ss << score;
            Text->RenderText("Final Score: " + ss.str(), 400.0f, (SCREEN_HEIGHT +50.0f )/ 2, 1.0f);
            Text->RenderText("Press Spacebar to start a new game", 400.0f, (SCREEN_HEIGHT + 100.0f) / 2, 1.0f);
            
            break;
        }

    }

    void Game::drawSquare(int x, int y, std::string sprite) {

        glm::vec2 position = generatePosition(x, y);
        glm::vec2 size = glm::vec2(SQUARE_SIZE, SQUARE_SIZE);

        Renderer->DrawSprite(Utility::ResourceManager::GetTexture(sprite.c_str()),
            position, size);

    }

    void Game::ResetLevel()
    {
        m_state = GameState::GAME_MENU;
        ResetPlayer();
    }

    void Game::ResetPlayer()
    {
        /* reset player stats */
        snake.clear();

        apple.x = generateRandomCoordinate(NUM_COLUMN - 1);
        apple.y = generateRandomCoordinate(NUM_ROWS - 1);

        snake.push_back(snakePiece(0, 0, Direction::RIGHT));

        score = 0;

    }

    void Game::DoCollisions()
    {

        for (int i = snake.size() - 1; i != 0; i--) {

            if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {

                m_state = GAME_LOOSE;
                return;
            }

        }

        bool result = CheckCollision(apple, snake[0]);

        /* If the snake eat the apple */
        if (result)
        {
            /* Calculate new coordinates for the apple */
            int appleX;
            int appleY;
            do {
                appleX = generateRandomCoordinate(NUM_COLUMN - 1);
                appleY = generateRandomCoordinate(NUM_ROWS - 1);
            } while (checkAppleCoordinateValidity(appleX, appleY, snake));

            apple.x = appleX;
            apple.y = appleY;

            Direction tailDir = snake.rbegin()->dir;
            
            int newX;
            int newY;
            Direction newDir = tailDir;

            switch (tailDir) {
            case(Direction::UP):
                newX = snake.rbegin()->x;
                newY = snake.rbegin()->y + 1;
                break;
            case(Direction::DOWN):
                newX = snake.rbegin()->x;
                newY = snake.rbegin()->y - 1;
                break;
            case(Direction::LEFT):
                newY = snake.rbegin()->y;
                newX = snake.rbegin()->x + 1;
                break;
            case(Direction::RIGHT):
                newY = snake.rbegin()->y;
                newX = snake.rbegin()->x - 1;
                break;
            }

            snakePiece newPiece(newX, newY, newDir);

            snake.push_back(newPiece);

            score++;

            /* Check for victory */
            if (score == SCREEN_HEIGHT * SCREEN_HEIGHT - 1) {
                m_state = GAME_WIN;
            }

        }

    }

    int generateRandomCoordinate(int max)
    {
        std::random_device rd;  /* obtain a random number from hardware */
        std::mt19937 gen(rd()); /* seed the generator */
        std::uniform_int_distribution<> distr(0, max);  /* define the range */

        return distr(gen);
    }

    glm::vec2 generatePosition(int x, int y)
    {

        glm::vec2 returnVec = glm::vec2(SQUARE_SIZE * x, SQUARE_SIZE * y);

        return returnVec;

    }

    bool checkAppleCoordinateValidity(int x, int y, const std::vector<snakePiece>& snakePieces)
    {
        for (const snakePiece& piece: snakePieces) {

            if (x == piece.x && y == piece.y)
                return true;

        }

        return false;
    }

}
