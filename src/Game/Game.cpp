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

    snakePiece::snakePiece(int xPos, int yPos, Direction dirObject) {
        Position.x = xPos;
        Position.y = yPos;
        dir = dirObject;
        Size = glm::vec2(SQUARE_SIZE, SQUARE_SIZE);
        Sprite = Utility::ResourceManager::GetTexture("snake");
    }

    void snakePiece::move() {

        switch (dir) {
        case(Direction::UP):
            Position.y-=SQUARE_SIZE;
            break;
        case(Direction::RIGHT):
            Position.x+=SQUARE_SIZE;
            break;
        case(Direction::LEFT):
            Position.x-=SQUARE_SIZE;
            break;
        case(Direction::DOWN):
            Position.y+=SQUARE_SIZE;
            break;
        }

        if (Position.y < 0) {
            Position.y = (NUM_ROWS - 1) * SQUARE_SIZE;
        }
        else if (Position.y > (NUM_ROWS - 1) * SQUARE_SIZE) {
            Position.y = 0;
        }
        else if (Position.x > (NUM_COLUMN - 1) * SQUARE_SIZE) {
            Position.x = 0;
        }
        else if (Position.x < 0) {
            Position.x = (NUM_COLUMN - 1) * SQUARE_SIZE;
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
    
    void Game::Init(const std::filesystem::path& shadersPath, const std::filesystem::path& texturePath, const std::filesystem::path& fontsPath, const std::filesystem::path& soundsPath, const std::filesystem::path& levelsPath)
    {
        const std::string vertexCode = (shadersPath / "sprite.vs").string();
        const std::string fragmentCode = (shadersPath / "sprite.frag").string();

        std::string applePNG = (texturePath / "apple.png").string();
        std::string snakePNG = (texturePath / "snake.png").string();
        std::string background = (texturePath / "background.jpg").string();
        std::string bricksJPG = (texturePath / "bricks.jpg").string();
        std::string ocra = (fontsPath / "OCRAEXT.TTF").string();
        std::string eatingsfx = (soundsPath / "eating.mp3").string();
        std::string musicFilePath = (soundsPath / "Music.mp3").string();

        /* Music configuration */
        music.openFromFile(musicFilePath);
        music.setLooping(true);
        music.play();

        /* Setting sfx */
        eatsfxBuffer.loadFromFile(eatingsfx);
        eatSfx.emplace(eatsfxBuffer);

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
        Utility::ResourceManager::LoadTexture(bricksJPG.c_str(), false, "bricks");

        // Load Levels
        GameLevel zero; zero.Load((levelsPath / "zero.txt").string().c_str());
        Levels.push_back(zero);

        /* Reset Snake and Apple position */
        ResetPlayer();

    }

    void Game::Update(float dt)
    {

        if (GAME_ACTIVE == m_state)
        {
            time += dt;

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
            if ((Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP]) && (Direction::DOWN != snake[0].dir) && (Direction::UP != snake[0].dir)) {
                snake[0].dir = Direction::UP;
                return true;
            }
            else if ((Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) && (Direction::RIGHT != snake[0].dir) && (Direction::LEFT != snake[0].dir)) {
                snake[0].dir = Direction::LEFT;
                return true;
            }
            else if ((Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN]) && (Direction::UP != snake[0].dir) && (Direction::DOWN != snake[0].dir)) {
                snake[0].dir = Direction::DOWN;
                return true;
            }
            else if ((Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) && (Direction::LEFT != snake[0].dir) && (Direction::RIGHT != snake[0].dir)) {
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

    bool Game::CheckCollision(const GameObject& one, const GameObject& two)
    {
        glm::vec2 onePos = one.Position;
        glm::vec2 twoPos = two.Position;

        bool collisionX = onePos.x == twoPos.x;
        bool collisionY = onePos.y == twoPos.y;

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
            apple.Draw(*Renderer);
            for (snakePiece& piece : snake) {
                piece.Draw(*Renderer);
            }
            /* Draw the score */
            std::stringstream ss; ss << score;
            Text->RenderText("Score: " + ss.str(), 5.0f, 5.0f, 1.0f);

            /* Draw the time */
            ss<<(int)time;
            Text->RenderText("Time: " + ss.str(), 800.0f, 5.0f, 1.0f);

            // Draw level
            Levels[CurrentLevel].Draw(*Renderer);

            break;
        }
        case(GAME_MENU):
            Text->RenderText("Snëk!", SCREEN_WIDTH * 0.32, SCREEN_HEIGHT * 0.4, 5.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            Text->RenderText("Press Spacebar to start", SCREEN_WIDTH * 0.32, SCREEN_HEIGHT * 0.6, 1.0f);
            break;

        case(GAME_WIN):
            Text->RenderText("Congratulation! You Win!", SCREEN_WIDTH * 0.32, SCREEN_HEIGHT * 0.4, 1.0f);
            Text->RenderText("Press Spacebar to start a new game", SCREEN_WIDTH * 0.32, SCREEN_HEIGHT * 0.6, 1.0f);
            break;
        case(GAME_LOOSE):

            Text->RenderText("You have lost", 400.0f, SCREEN_HEIGHT / 2, 1.0f);
            std::stringstream ss; ss << score;
            Text->RenderText("Final Score: " + ss.str(), 400.0f, (SCREEN_HEIGHT +50.0f )/ 2, 1.0f);
            Text->RenderText("Press Spacebar to start a new game", SCREEN_WIDTH * 0.32, SCREEN_HEIGHT * 0.6, 1.0f);
            
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
        snake.push_back(snakePiece(0, 0, Direction::RIGHT));

        /* Calculate new coordinates for the apple */
        int apple_x;
        int apple_y;
        do {
            apple_x = generateRandomCoordinate(NUM_COLUMN - 1);
            apple_y = generateRandomCoordinate(NUM_ROWS - 1);
        } while (generateValidAppleCoord(apple_x, apple_y));

        apple.Init(
            glm::vec2(SQUARE_SIZE * apple_x, SQUARE_SIZE * apple_y),
            glm::vec2(SQUARE_SIZE, SQUARE_SIZE),
            Utility::ResourceManager::GetTexture("apple")
        );

        score = 0;
        time = 0;

    }

    void Game::DoCollisions()
    {

        /* Check collision snake - snake */
        for (int i = snake.size() - 1; i != 0; i--)
        {
            if(CheckCollision(snake[i], snake[0]))
            {
                m_state = GAME_LOOSE;
                break;
            }
        }

        /* If the snake eat the apple */
        if (CheckCollision(apple, snake[0]))
        {

            /* Play eating sfx */
            eatSfx->play();

            /* Calculate new coordinates for the apple */
            int appleX;
            int appleY;
            do {
                appleX = generateRandomCoordinate(NUM_COLUMN - 1);
                appleY = generateRandomCoordinate(NUM_ROWS - 1);
            } while (generateValidAppleCoord(appleX, appleY));

            apple.Position = glm::vec2(SQUARE_SIZE * appleX, SQUARE_SIZE * appleY);

            Direction tailDir = snake.rbegin()->dir;
            
            int newX;
            int newY;
            Direction newDir = tailDir;

            switch (tailDir) {
            case(Direction::UP):
                newX = snake.rbegin()->Position.x;
                newY = snake.rbegin()->Position.y + SQUARE_SIZE;
                break;
            case(Direction::DOWN):
                newX = snake.rbegin()->Position.x;
                newY = snake.rbegin()->Position.y - SQUARE_SIZE;
                break;
            case(Direction::LEFT):
                newY = snake.rbegin()->Position.y;
                newX = snake.rbegin()->Position.x + SQUARE_SIZE;
                break;
            case(Direction::RIGHT):
                newY = snake.rbegin()->Position.y;
                newX = snake.rbegin()->Position.x - SQUARE_SIZE;
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

        /* Check collision snake - brick */
        for(const auto& brick : Levels[CurrentLevel].Bricks)
        {
            if(CheckCollision(brick, snake[0]))
            {
                m_state = GAME_LOOSE;
                break;
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

    bool Game::generateValidAppleCoord(int x, int y)
    {

        for (const snakePiece& piece: snake)
        {
            if (x == piece.Position.x && y == piece.Position.y)
            {
                return true;
            }
        }

        for (const auto& brick: Levels[CurrentLevel].Bricks)
        {
            if (x == brick.Position.x && y == brick.Position.y)
            {
                return true;
            }
        }

        return false;
    }

}
