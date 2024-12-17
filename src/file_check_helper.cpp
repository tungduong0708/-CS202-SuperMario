#include "file_check_helper.h"

bool ifFileExists(const std::string& filePath) {
    struct stat checkFileExists;
    return stat(filePath.c_str(), &checkFileExists) == 0;
}