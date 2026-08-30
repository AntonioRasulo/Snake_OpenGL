#ifndef SNAKEPIECE_H
#define SNAKEPIECE_H

#include "GameObject.hpp"
#include "GameTypes.hpp"

namespace game
{
    class SnakePiece : public GameObject
    {

        bool isHead = false;

        public:

            /* Constructor */
            SnakePiece(int xPos, int yPos, Direction dirObject, bool is_head = false);

            /* Method used to increase/decrease x or y according to the direction */
            void move();

            /* Direction of the piece */
            Direction dir;

    };
}

#endif