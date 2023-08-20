#include "CLI.hpp"
#include <exception>
#include <sstream>

namespace CLI {

    namespace {
        void cmdOptionExists(char** begin, char** end, const std::string& option)
        {
            if (!(std::find(begin, end, option) != end)) {
                std::stringstream ss;
                ss << "Option " << option << " is not present as argument of the executable\n";
                std::runtime_error ex(ss.str().c_str());
                throw ex;
            }
        }

        char* getCmdOption(char** begin, char** end, const std::string& option)
        {
            char** itr = std::find(begin, end, option);
            if (itr != end && ++itr != end)
            {
                return *itr;
            }
            else {
                std::stringstream ss;
                ss << "Command of option " << option << " is not present as argument of the executable\n";
                std::runtime_error ex(ss.str().c_str());
                throw ex;
            }
            return nullptr;
        }
    }

    std::string checkAndGetCmdOption(char** begin, char** end, const std::string& option)
    {
        try {
            cmdOptionExists(begin, end, option);
        }
        catch (std::runtime_error& ex) {
            throw ex;
        }

        try {
            std::string return_str = getCmdOption(begin, end, option);
            return return_str;
        }
        catch (std::runtime_error& ex) {
            throw ex;
        }
    }

}
