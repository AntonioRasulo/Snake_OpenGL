#include "SnakePiece.hpp"
#include "ResourceManager.hpp"

namespace game
{
    SnakePiece::SnakePiece(int xPos, int yPos, Direction dirObject, bool is_head)
    {
        Position.x = xPos;
        Position.y = yPos;
        dir = dirObject;
        Size = glm::vec2(SQUARE_SIZE, SQUARE_SIZE);
        if (is_head)
        {
            isHead = true;
            Sprite = Utility::ResourceManager::GetTexture("snake_head");
        }
        else
        {
            Sprite = Utility::ResourceManager::GetTexture("snake");
        }
    }

    void SnakePiece::move()
    {

        switch (dir)
        {
        case (Direction::UP):
            Position.y -= SQUARE_SIZE;
            if (isHead)
                Rotation = 0.0f;
            break;
        case (Direction::RIGHT):
            Position.x += SQUARE_SIZE;
            if (isHead)
                Rotation = 90.0f;
            break;
        case (Direction::LEFT):
            Position.x -= SQUARE_SIZE;
            if (isHead)
                Rotation = -90.0f;
            break;
        case (Direction::DOWN):
            Position.y += SQUARE_SIZE;
            if (isHead)
                Rotation = 180.0f;
            break;
        }

        if (Position.y < 0)
        {
            Position.y = (NUM_ROWS - 1) * SQUARE_SIZE;
        }
        else if (Position.y > (NUM_ROWS - 1) * SQUARE_SIZE)
        {
            Position.y = 0;
        }
        else if (Position.x > (NUM_COLUMN - 1) * SQUARE_SIZE)
        {
            Position.x = 0;
        }
        else if (Position.x < 0)
        {
            Position.x = (NUM_COLUMN - 1) * SQUARE_SIZE;
        }
    }
}