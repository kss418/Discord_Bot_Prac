#include "Discord_BOT.h"
#include <iostream>
#include <string_view>
const dpp::snowflake Guild_ID = 408601520951656448;

void Discord_BOT::Add_Command_Guild(const dpp::slashcommand& CMD){
    BOT.guild_command_create(CMD, Guild_ID,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Create_Command_Log(cb, CMD);
    });
}

void Discord_BOT::Add_Command_Global(const dpp::slashcommand& CMD){
    BOT.global_command_create(CMD,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Create_Command_Log(cb, CMD);
    });
}

std::string Discord_BOT::Get_Option_Value(const Option& option, const std::string& Key){
    static const std::string Empty = "0";
    for(auto& [Str, it] : Option_List){
        auto& [Name, Ptr] = it;
        if(Str == Key) return option.*Ptr;
    }
   
    return Empty;
}

void Discord_BOT::Move_Hexa_Skill_Page(const dpp::button_click_t& Event){
    dpp::snowflake UID = Event.command.get_issuing_user().id;
    dpp::snowflake MID = Event.command.message_id;
    size_t& Page = Message_Page[Event.command.message_id];
    Message_Info[UID] = { Event.command.message_id, Event.command.channel_id };

    auto it = Message_Map.find(MID);
    if(it == Message_Map.end()){
        dpp::message Msg("헥사 스킬 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    Event.reply("로딩 중 입니다...");
    const auto& Skill = std::get<Character_Skill>(it->second);

    int32_t Last_Page = (Skill.character_skill.size() - 1) / Skill_Per_Page;
    if(Event.custom_id == "next_hexa_skill_page" && Page < Last_Page) Page++;
    if(Event.custom_id == "prev_hexa_skill_page" && Page > 0) Page--;

    dpp::message Msg = Generate_Hexa_Skill_Embed(Skill, Page);
    Msg.id = Event.command.message_id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_edit(Msg);
    Event.delete_original_response();
}

void Discord_BOT::Edit_Prev_Message(dpp::message& Msg, const dpp::snowflake& UID){
    Msg.id = Message_Info[UID].first;
    Msg.channel_id = Message_Info[UID].second;
    BOT.message_edit(Msg);
}

void Discord_BOT::Move_Equipment_Page(const dpp::button_click_t& Event){
    dpp::snowflake UID = Event.command.get_issuing_user().id;
    dpp::snowflake MID = Event.command.message_id;
    size_t& Page = Message_Page[Event.command.message_id];
    Message_Info[UID] = { Event.command.message_id, Event.command.channel_id };

    if(Event.custom_id == "prev_equipment_page" && Page > 0) --Page;
    if(Event.custom_id == "next_equipment_page" && Page < 3) ++Page;

    auto it = Message_Map.find(MID);
    if(it == Message_Map.end()){
        dpp::message Msg("장비 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    const auto& Equipment = std::get<Equipment_Set>(it->second);
    const std::vector <Equipment_Info>& Current_Equipment = !Page ? Equipment.item_equipment : (
        Page == 1 ? Equipment.item_equipment_preset_1 : (
            Page == 2 ? Equipment.item_equipment_preset_2 : Equipment.item_equipment_preset_3
        )
    );

    Event.reply("로딩 중 입니다...");
    dpp::message Msg = Generate_Equipment_Embed(Current_Equipment, Page);
    Msg.id = Event.command.message_id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_edit(Msg);
    Event.delete_original_response();
}

void Discord_BOT::Show_Equipment_Detail(const dpp::select_click_t& Event){
    std::cout << "상세 정보 출력 함수 호출" << std::endl;
    Event.reply(dpp::message("로딩 중 입니다..."));
    const dpp::snowflake UID = Event.command.get_issuing_user().id;
    const dpp::snowflake MID = Event.command.message_id;
    const dpp::snowflake CID = Event.command.channel_id;

    int Index = std::stoi(Event.values[0]);
    auto Equipment_It = Message_Map.find(MID);
    auto Page_It = Message_Page.find(MID);
    if(Equipment_It == Message_Map.end() || Page_It == Message_Page.end()){
        dpp::message Msg("장비 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        Event.delete_original_response();
        return;
    }
    
    const Equipment_Set& Equipment_ = std::get<Equipment_Set>(Message_Map[MID]);
    const std::vector <Equipment_Info>& Equipment_List = !Message_Page[MID] ? Equipment_.item_equipment : (
        Message_Page[MID] == 1 ? Equipment_.item_equipment_preset_1 : (
            Message_Page[MID] == 2 ? Equipment_.item_equipment_preset_2 : Equipment_.item_equipment_preset_3
        )
    );

    if(Index < 0 || Index >= Equipment_List.size()){
        dpp::message Msg("잘못된 장비 선택입니다.");
        Edit_Prev_Message(Msg, UID);
        Event.delete_original_response();
        return;
    }

    const Equipment_Info& Equipment = Equipment_List[Index];
    dpp::message Msg;
    dpp::embed Image;
    Image.set_title(Get_Equipment_Name(Equipment))
        .set_thumbnail(Equipment.item_icon)
        .set_color(Get_Potential_Color(Equipment.potential_option_grade))
        .set_description(Get_Equipment_Detail_Message(Equipment));
        
    Msg.add_embed(Image);

    dpp::component Back_Button = dpp::component()
        .add_component(dpp::component()
            .set_type(dpp::cot_button)
            .set_style(dpp::cos_secondary)
            .set_label("🔙")
            .set_id("back_summary")
    );

    Msg.add_component(Back_Button);
    Msg.id = Event.command.message_id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_edit(Msg); 
    Event.delete_original_response();
}

uint32_t Discord_BOT::Get_Potential_Color(const std::string& Potential_Grade) const{
    if(Potential_Grade == "레전드리") return 0x33FF33;
    else if(Potential_Grade == "유니크") return 0xFFFF00;
    else if(Potential_Grade == "에픽") return 0x7F00FF;
    else if(Potential_Grade == "레어") return 0x99CCFF;
    return 0xFFFFFF;
}

std::string Discord_BOT::Get_Equipment_Detail_Message(const Equipment_Info& Equipment){
    std::string Msg;
    Msg += "장비분류: " + Equipment.item_equipment_part + "\n";

    for(const auto& [Key, it] : Option_List){
        auto[Name, Ptr] = it;
        if((Equipment.item_total_option).*Ptr == "0") continue;
        Msg += Name + ": +" + Get_Equipment_Detail_Option(Equipment, Key) + "\n";
    }

    if(Equipment.cuttable_count != "255"){
        Msg += "가위 사용 가능 횟수: " + Equipment.cuttable_count + "\n";
    }

    if(!Equipment.potential_option_grade.empty()){
        Msg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        Msg += "\n" + Equipment.potential_option_grade + " 잠재옵션\n";
        Msg += Equipment.potential_option_1 + "\n";
        Msg += Equipment.potential_option_2 + "\n";
        if(!Equipment.potential_option_3.empty()){
            Msg += Equipment.potential_option_3 + "\n";
        }
    }

    if(!Equipment.additional_potential_option_grade.empty()){
        Msg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        Msg += "\n" + Equipment.additional_potential_option_grade + " 에디셔널 잠재옵션\n";
        Msg += Equipment.additional_potential_option_1 + "\n";
        Msg += Equipment.additional_potential_option_2+ "\n";
        if(!Equipment.additional_potential_option_3.empty()){
            Msg += Equipment.additional_potential_option_3 + "\n";
        }
    }

    if(Get_Option_Value(Equipment.item_exceptional_option, "exceptional_upgrade") != "0"){
        Msg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n익셉셔널\n";
        for(const auto& [Key, it] : Option_List){
            auto [Name, Ptr] = it;
            if(Get_Option_Value(Equipment.item_exceptional_option, Key) == "0") continue;
            Msg += Name + ": +" + Get_Option_Value(Equipment.item_exceptional_option, Key) + "\n";
        }
        Msg += "익셉셔널 강화 " + Get_Option_Value(Equipment.item_exceptional_option, "exceptional_upgrade") + "회 적용\n";
    }

    return Msg;
}

bool Discord_BOT::Is_Starforce(const Equipment_Info& Equipment) const{
    const std::string& slot = Equipment.item_equipment_slot;
    if(slot == "보조무기") return 0;
    if(slot == "뱃지") return 0;
    if(slot == "훈장") return 0;
    if(slot == "포켓 아이템") return 0;
    if(slot == "엠블렘") return 0;
    if(slot == "반지" && Equipment.special_ring_level) return 0;
    return 1;
}

bool Discord_BOT::Is_Scroll(const Equipment_Info& Equipment) const{
    const std::string& slot = Equipment.item_equipment_slot;
    if(slot == "보조무기") return 0;
    if(slot == "뱃지") return 0;
    if(slot == "훈장") return 0;
    if(slot == "포켓 아이템") return 0;
    if(slot == "엠블렘") return 0;
    if(slot == "반지" && Equipment.special_ring_level) return 0;
    return 1;
}

bool Discord_BOT::Is_Additional_Option(const Equipment_Info& Equipment) const{
    const std::string& slot = Equipment.item_equipment_slot;
    const std::string& part = Equipment.item_equipment_part;
    if(slot == "보조무기") return 0;
    if(slot == "뱃지") return 0;
    if(slot == "훈장") return 0;
    if(slot == "어깨장식") return 0;
    if(slot == "엠블렘") return 0;
    if(part == "반지") return 0;
    return 1;
}

bool Discord_BOT::Is_Percentage(const std::string& Key) const{
    if(Key == "boss_damage" || Key == "ignore_monster_armor") return 1;
    if(Key == "all_stat" || Key == "max_hp_rate") return 1;
    return 0;
}

std::string Discord_BOT::Get_Equipment_Name(const Equipment_Info& Equipment) const{
    std::string Msg;
    if(Is_Starforce(Equipment)){
        for(int i = 1;i <= 30;i++){
            if(i % 5 == 1 && i != 1) Msg += " ";
            if(i == 16) Msg += "\n";
            Msg += (i <= std::stoi(Equipment.starforce) ? "★" : "☆");
        }   
        Msg += "\n";
    }
    Msg += Equipment.item_name;
    if(Is_Scroll(Equipment)){
        Msg += " (+" + Equipment.scroll_upgrade + ")";
    }
    if(Equipment.special_ring_level && Equipment.item_equipment_part == "반지"){
        Msg += " Lv." + std::to_string(Equipment.special_ring_level);
    }
    return Msg;
}

void Discord_BOT::Back_Summary_Page(const dpp::button_click_t& Event){
    const dpp::snowflake UID = Event.command.get_issuing_user().id;
    const dpp::snowflake MID = Event.command.message_id;
    const dpp::snowflake CID = Event.command.channel_id;
    size_t& Page = Message_Page[MID];

    auto it = Message_Map.find(MID);
    if(it == Message_Map.end()){
        dpp::message Msg("장비 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    const auto& Equipment = std::get<Equipment_Set>(it->second);
    Event.reply("로딩 중 입니다...");
    const std::vector <Equipment_Info>& Current_Equipment = !Page ? Equipment.item_equipment : (
        Page == 1 ? Equipment.item_equipment_preset_1 : (
            Page == 2 ? Equipment.item_equipment_preset_2 : Equipment.item_equipment_preset_3
        )
    );

    dpp::message Msg = Generate_Equipment_Embed(Current_Equipment, Page);
    Msg.id = Event.command.message_id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_edit(Msg);
    Event.delete_original_response();
}

void Discord_BOT::Delete_Command_Message(const dpp::button_click_t& Event){
    BOT.message_delete(Event.command.message_id, Event.command.channel_id);
}

void Discord_BOT::Move_Hexa_Stat_Page(const dpp::button_click_t& Event){
    dpp::snowflake UID = Event.command.get_issuing_user().id;
    dpp::snowflake MID = Event.command.message_id;
    size_t& Page = Message_Page[Event.command.message_id];
    Message_Info[UID] = { Event.command.message_id, Event.command.channel_id };

    auto it = Message_Map.find(MID);
    if(it == Message_Map.end()){
        dpp::message Msg("헥사 스탯 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    Event.reply("로딩 중 입니다...");
    const auto& Stat = std::get<Hexa_Stat>(it->second);

    int32_t Last_Page = Hexa_Core_Count;
    if(Event.custom_id == "next_hexa_stat_page" && Page < Last_Page - 1) Page++;
    if(Event.custom_id == "prev_hexa_stat_page" && Page > 0) Page--;

    dpp::message Msg = Generate_Hexa_Stat_Embed(Stat, Page);
    Msg.id = Event.command.message_id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_edit(Msg);
    Event.delete_original_response();
}

void Discord_BOT::Move_Symbol_Page(const dpp::button_click_t& Event){
    dpp::snowflake UID = Event.command.get_issuing_user().id;
    dpp::snowflake MID = Event.command.message_id;
    size_t& Page = Message_Page[Event.command.message_id];
    Message_Info[UID] = { Event.command.message_id, Event.command.channel_id };

    auto it = Message_Map.find(MID);
    if(it == Message_Map.end()){
        dpp::message Msg("심볼 정보를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    Event.reply("로딩 중 입니다...");
    const auto& Symbol_Info = std::get<Symbol>(it->second);

    int32_t Last_Page = Symbol_Info.symbol.size() / 6 + (Symbol_Info.symbol.size() % 6 ? 1 : 0);
    if(Event.custom_id == "next_symbol_page" && Page < Last_Page - 1) Page++;
    if(Event.custom_id == "prev_symbol_page" && Page > 0) Page--;

    dpp::message Msg = Generate_Symbol_Embed(Symbol_Info, Page);
    Msg.id = Event.command.message_id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_edit(Msg);
    Event.delete_original_response();
}

std::string Discord_BOT::Get_Symbol_Progress(const Symbol::Info& Symbol) const{
    std::string_view Suffix{ Symbol.symbol_name };
    std::string Ret;
    if(Suffix.starts_with("아케인") && Symbol.symbol_level == 20) Ret += " (MAX)";
    else if(Suffix.starts_with("어센틱") && Symbol.symbol_level == 11) Ret += " (MAX)";
    else if(Suffix.starts_with("그랜드 어센틱") && Symbol.symbol_level == 11) Ret += " (MAX)";
    else{
        Ret += " (" + std::to_string(Symbol.symbol_growth_count) + " / ";
        Ret += std::to_string(Symbol.symbol_require_growth_count) + ")";
    }
    
    return Ret;
}

dpp::component Discord_BOT::Create_Move_Page_Component(const std::string& Custom_ID) const{
    dpp::component Ret{};

    Ret.add_component(dpp::component()
        .set_label("◀️")
        .set_id("prev_" + Custom_ID + "_page")
        .set_style(dpp::cos_secondary)
        .set_type(dpp::cot_button)
    );

    Ret.add_component(dpp::component()
        .set_label("▶️")
        .set_id("next_" + Custom_ID + "_page")
        .set_style(dpp::cos_secondary)
        .set_type(dpp::cot_button)
    );

    Ret.add_component(dpp::component()
        .set_label("❌")
        .set_id("delete_command_message")
        .set_style(dpp::cos_secondary)
        .set_type(dpp::cot_button)
    );
       
    return Ret;
}

void Discord_BOT::Create_Message(dpp::message& Msg, const dpp::slashcommand_t& Event, const Map_Type& Map){
    const dpp::snowflake UID = Event.command.get_issuing_user().id;
    Msg.channel_id = Event.command.channel_id;
    BOT.message_create(Msg, [this, UID, Map](const dpp::confirmation_callback_t& cb){
        if(!this->Create_Message_Log(cb)) return;
        const dpp::message& Sent = std::get<dpp::message>(cb.value);
        this->Message_Info[UID] = { Sent.id, Sent.channel_id };
        this->Message_Map[Sent.id] = Map;
        this->Message_Page[Sent.id] = 0;
    });
}
