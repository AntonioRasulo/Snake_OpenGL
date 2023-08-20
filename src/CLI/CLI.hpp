#pragma once

#include <string>

namespace CLI {

    std::string checkAndGetCmdOption(char** begin, char** end, const std::string& option);

}
