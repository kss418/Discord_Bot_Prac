#include "Discord_BOT.h"
#include <iostream>

std::string Discord_BOT::Get_Equipment_Detail_Option(const Equipment_Info& Equipment, const std::string& Key){
    std::string Msg;
    Msg += Get_Map_By_Key(Equipment.Total_Option.Map, Key);
    if(Is_Percentage(Key)) Msg += "%";

    if(Get_Map_By_Key(Equipment.Total_Option.Map, Key) == Get_Map_By_Key(Equipment.Base_Option.Map, Key)){
        return Msg;
    }
    
    Msg += " (";
    Msg += Get_Map_By_Key(Equipment.Base_Option.Map, Key);
    if(Is_Percentage(Key)) Msg += "%";

    if(Is_Additional_Option(Equipment)){
        Msg += "+" + Get_Map_By_Key(Equipment.Additional_Option.Map, Key);
        if(Is_Percentage(Key)) Msg += "%";
    }
    
    if(Is_Scroll(Equipment)){
        Msg += (std::stoi(Get_Map_By_Key(Equipment.Etc_Option.Map, Key)) < 0 ? "-" : "+");
        Msg += Get_Map_By_Key(Equipment.Etc_Option.Map, Key);
        if(Is_Percentage(Key)) Msg += "%";
    }

    if(Is_Starforce(Equipment)){
        Msg += "+" + Get_Map_By_Key(Equipment.Starforce_Option.Map, Key);
        if(Is_Percentage(Key)) Msg += "%";
    }
    Msg += ")";
    return Msg;
}

dpp::message Discord_BOT::Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page){
    dpp::component menu;
    menu.set_type(dpp::cot_selectmenu)
        .set_placeholder("장비 선택")
        .set_id("select_equipment");

    for(int i = 0;i < Info.size();i++){
        Equipment_Info Current_Equipment = Info[i];
        std::string description = (Is_Starforce(Current_Equipment) ? "★" + Current_Equipment.Starforce + " | " : "");
        if(!Current_Equipment.Potential_Option_Info.Grade.empty()) description += Current_Equipment.Potential_Option_Info.Grade;
        if(!Current_Equipment.Potential_Option_Info.Additional_Grade.empty()) description += " | " + Current_Equipment.Potential_Option_Info.Additional_Grade;
        if(Current_Equipment.Special_Ring_Level && Current_Equipment.Part_Name == "반지"){
            description += " Lv." + std::to_string(Current_Equipment.Special_Ring_Level);
        }

        menu.add_select_option(
            dpp::select_option(Current_Equipment.Item_Name, std::to_string(i))
                .set_description(description)
        );
    }

    dpp::component row;
    row.add_component(menu);

    dpp::message msg(page ? std::to_string(page) + "번 프리셋 장비" : "현재 장착 장비");
    msg.add_component(row);
    msg.add_component(dpp::component()
        .add_component(dpp::component()
            .set_label("◀️")
            .set_id("prev_equipment_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("▶️")
            .set_id("next_equipment_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("❌")
            .set_id("delete_command_message")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
    );
    
    return msg;
}

dpp::message Discord_BOT::Generate_Hexa_Skill_Embed(const Character_Skill& Skill, int Page){
    std::vector <Character_Skill::Skill_Info> Skill_List = Skill.character_skill;
    dpp::message Msg;
    dpp::embed Embed[4], Main_Embed;

    int32_t Start = Page * Skill_Per_Page;
    Main_Embed.set_title("헥사 스킬 목록\n" + std::to_string(Page + 1) + " / " + 
        std::to_string((Skill_List.size() - 1) / Skill_Per_Page + 1) + "페이지")
        .set_color(0xFFFF);
    
    for(int i = 0;i < Skill_Per_Page && Start + i < Skill_List.size();i++){
        const Character_Skill::Skill_Info& Skill = Skill_List[Start + i];
        Embed[i].set_thumbnail(Skill.skill_icon);
        Embed[i].add_field(Skill.skill_name, std::to_string(Skill.skill_level) + "레벨", false);
    }
    Msg.add_embed(Main_Embed);
    for(int i = 0;i < Skill_Per_Page;i++) Msg.add_embed(Embed[i]);

    Msg.add_component(dpp::component()
        .add_component(dpp::component()
            .set_label("◀️")
            .set_id("prev_hexa_skill_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("▶️")
            .set_id("next_hexa_skill_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("❌")
            .set_id("delete_command_message")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
    );

    return Msg;
}

dpp::message Discord_BOT::Generate_Hexa_Stat_Embed(const Hexa_Stat& Stat, int page){
    int32_t Hexa_Core_Count = 3;
    std::vector <std::vector<Hexa_Stat::Core>> Core_List;
    Core_List.push_back(Stat.character_hexa_stat_core);
    Core_List.push_back(Stat.character_hexa_stat_core_2);
    Core_List.push_back(Stat.character_hexa_stat_core_3);
    Hexa_Stat::Core Current_Core = Core_List[page][0];

    dpp::message Msg(std::to_string(page + 1) + "번 헥사 스탯\n" +
        "메인 레벨 : " + std::to_string(Current_Core.main_stat_level) + "\n" +
        "메인 스탯 : " + Current_Core.main_stat_name + "\n" +
        "에디셔널1 레벨 : " + std::to_string(Current_Core.sub_stat_level_1) + "\n" +
        "에디셔널1 스탯 : " + Current_Core.sub_stat_name_1 + "\n" +
        "에디셔널2 레벨 : " + std::to_string(Current_Core.sub_stat_level_2) + "\n" +
        "에디셔널2 스탯 : " + Current_Core.sub_stat_name_2 + "\n"
    );

    Msg.add_component(dpp::component()
        .add_component(dpp::component()
            .set_label("◀️")
            .set_id("prev_hexa_stat_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("▶️")
            .set_id("next_hexa_stat_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("❌")
            .set_id("delete_command_message")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
    );

    return Msg;
}