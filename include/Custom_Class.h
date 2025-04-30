#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>

class Character {
public:
    std::string character_image;
    std::string world_name;
    std::string character_class;
    int64_t character_level;
    std::string character_exp_rate;
    std::string character_guild_name;
};

class Union {
public:
    int64_t union_level;
    std::string union_grade;
    int64_t union_artifact_level;
};

class Popularity {
public:
    int32_t popularity;
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
};

class Option {
public:
    std::string str;
    std::string dex;
    std::string int_;
    std::string luk;
    std::string max_hp;
    std::string attack_power;
    std::string magic_power;
    std::string boss_damage;
    std::string ignore_monster_armor;
    std::string all_stat;
    std::string max_hp_rate;
};

class Equipment_Info {
public:
    std::string item_equipment_part;
    std::string item_equipment_slot;
    std::string item_name;
    std::string item_icon;
    std::string scroll_upgrade;
    std::string cuttable_count;
    std::string golden_hammer_flag;
    std::string soul_name;
    std::string soul_option;
    std::string starforce;

    Option item_base_option;
    Option item_total_option;
    Option item_add_option;
    Option item_starforce_option;
    Option item_exceptional_option;
    Option item_etc_option;

    std::string potential_option_grade;
    std::string additional_potential_option_grade;
    std::string potential_option_1;
    std::string potential_option_2;
    std::string potential_option_3;
    std::string additional_potential_option_1;
    std::string additional_potential_option_2;
    std::string additional_potential_option_3;

    int64_t special_ring_level;
};

class Equipment_Set {
public:
    std::vector<Equipment_Info> item_equipment;
    std::vector<Equipment_Info> item_equipment_preset_1;
    std::vector<Equipment_Info> item_equipment_preset_2;
    std::vector<Equipment_Info> item_equipment_preset_3;
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
};

class Symbol {
public:
    class Info{
    public:
        std::string symbol_name;
        std::string symbol_icon;
        int32_t symbol_level;
        int32_t symbol_growth_count;
        int32_t symbol_require_growth_count;
    };
    
    std::vector <Info> symbol;
};

using API_Type = std::variant<
    Equipment_Set, Character_Skill, Hexa_Stat, 
    Symbol, Popularity, Union, Character
>;

class API_Result{
public:
    int32_t Status_Code;
    API_Type Data;
};