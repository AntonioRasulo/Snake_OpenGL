#include "GameLevel.hpp"
#include <fstream>
#include <sstream>

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
                std::istringstream sstream(line);
                std::vector<unsigned int> row;
                int xCoord = -1;
                // Read each word separated by spaces
                while(sstream >> tileCode)
                {
                    if(xCoord == -1)
                    {
                        xCoord = tileCode;
                    }
                    else
                    {
                        std::pair<int, int> coordPair(xCoord, tileCode);
                        tileData.push_back(coordPair);
                        xCoord = -1;
                    }
                }
                if(tileData.empty() == false)
                {
                    Init(tileData);
                }
            }
        }
    }

    void GameLevel::Init(std::vector<std::pair<int, int>> tileData)
    {
        //unsigned int lvlWidth =
        // const int NUM_COLUMN = 50;
        // const int NUM_ROWS = 50;
        for(auto coords : tileData)
        {
            
        }
    }

}