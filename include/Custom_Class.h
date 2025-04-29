#pragma once
#include <string>
#include <vector>
#include <unordered_map>

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

class Character_Skill {
public:
    class Skill_Info{
    public:
        std::string skill_name;
        std::string skill_description;
        int64_t skill_level;
        std::string skill_icon;
    };
    
    std::string character_class;  
    std::string character_skill_grade;
    std::vector <Skill_Info> character_skill;
    int32_t Status_Code;
};

const std::vector <std::pair<std::string, std::string>> Option_List = {
    {"str", "STR"},
    {"dex", "DEX"},
    {"int", "INT"},
    {"luk", "LUK"},
    {"max_hp", "최대 HP"},
    {"attack_power", "공격력"},
    {"magic_power", "마력"},
    {"boss_damage", "보스 몬스터 공격 시 데미지"},
    {"ignore_monster_armor", "몬스터 방어율 무시"},
    {"all_stat", "올스탯"},
    {"max_hp_rate", "최대 HP"},
};

class Option {
public:
    std::unordered_map <std::string, std::string> Map;
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
    std::string Uprade_Count;
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
    Option Etc_Option;

    Potential_Option Potential_Option_Info;
    int64_t Special_Ring_Level;
};

class Equipment_Set {
public:
    std::vector <Equipment_Info> Info[4];
    int32_t Status_Code;
};

class Hexa_Stat {
public:
    class Core{
    public:
        std::string slot_id;
        std::string main_stat_name;
        std::string sub_stat_name_1;
        std::string sub_stat_name_2;
        int64_t main_stat_level;
        int64_t sub_stat_level_1;
        int64_t sub_stat_level_2;
        int64_t stat_grade;
    };
    std::string character_class;
    std::vector <Core> character_hexa_stat_core;
    std::vector <Core> character_hexa_stat_core_2;
    std::vector <Core> character_hexa_stat_core_3;
    
    int32_t Status_Code;
};