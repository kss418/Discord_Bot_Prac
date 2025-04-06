#pragma once
#include <string>

class Character {
public:
    std::string Image_URL;
    std::string World_Name;
    std::string Class;
    int64_t Level;
    std::string EXP_Rate;
    std::string Guild_Name;
    int32_t Status_Code;
};

class Union {
public:
    int64_t Union_Level;
    std::string Union_Grade;
    int64_t Artifact_Level;
    int32_t Status_Code;
};
