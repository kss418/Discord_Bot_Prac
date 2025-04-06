#pragma once
#include <string>
#include <vector>

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

class Popularity {
public:
    int32_t Popularity;
    int32_t Status_Code;
};

class Option {
public:
    std::string Str;
    std::string Dex;
    std::string Int;
    std::string Luk;
    std::string Max_HP;
    std::string Max_MP;
    std::string Attack_Power;
    std::string Magic_Power;
    std::string Boss_Damage;
    std::string Ignore_Armor;
    std::string All_Stat;
    std::string Damage;
    std::string Max_HP_Rate;
    std::string Max_MP_Rate;
};

class Potential_Option {
public:
    std::string Grade;
    std::string Additional_Grade;
    std::string Option[3];
    std::string Additional_Option[3];
};

class Equipment_Info {
public:
    std::string Part_Name;
    std::string Slot_Name;
    std::string Item_Name;
    std::string Item_Icon;
    std::string Uprage_Count;
    std::string Cuttable_Count;
    std::string Golden_Hammer_Flag;
    std::string Soul_Name;
    std::string Soul_Option;
    std::string Starforce;

    Option Base_Option;
    Option Total_Option;
    Option Additional_Option;
    Option Starforce_Option;
    Option Exceptional_Option;

    std::string Flag;
    std::string Additional_Flag;
    Potential_Option Basic_Potential_Option;
    Potential_Option Additional_Potential_Option;

    int64_t Special_Ring_Level;
};

class Equipment_Set {
public:
    std::vector <Equipment_Info> Info[4];
    int32_t Status_Code;
};