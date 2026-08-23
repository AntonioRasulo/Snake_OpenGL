#include "GameLevel.hpp"
#include <fstream>

namespace game {

    void GameLevel::Load(const char *file)
    {
        // clear old data
        Bricks.clear();

        unsigned int tileCode;

        std::string line;
        std::ifstream fstream(file);

        std::vector<std::pair<int, int>> tileData;

        if(fstream)
        {
            // Read each line from level file
            while(std::getline(fstream, line))
            {

            }
        }
    }

}