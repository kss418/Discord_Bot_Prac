#include "Discord_BOT.h"
#include <iostream>

void Discord_BOT::Find_Error(const dpp::slashcommand_t& Event, int32_t Status_Code){
    if(Status_Code == 400) Not_Found_Name(Event);
    else if(Status_Code == 429) Many_Requests(Event);
    else if(Status_Code == 500) Internal_Server_Error(Event);
}

void Discord_BOT::Not_Found_Name(const dpp::slashcommand_t& Event){
    dpp::embed error;
    error.set_title("닉네임 오류")
        .set_description("존재하지 않는 캐릭터입니다.")
        .set_color(0xff0000);
    Event.reply(dpp::message().add_embed(error));
}

void Discord_BOT::Internal_Server_Error(const dpp::slashcommand_t& Event){
    dpp::embed error;
    error.set_title("내부 서버 오류")
        .set_description("메이플 API 서버에 오류가 발생했습니다.")
        .set_color(0xff0000);
    Event.reply(dpp::message().add_embed(error));
}

void Discord_BOT::Many_Requests(const dpp::slashcommand_t& Event){
    dpp::embed error;
    error.set_title("내부 서버 오류")
        .set_description("동시에 들어오는 요청이 너무 많습니다.")
        .set_color(0xff0000);
    Event.reply(dpp::message().add_embed(error));
}

bool Discord_BOT::Create_Message_Log(const dpp::confirmation_callback_t& cb){
    if(!cb.is_error()) return 1;
    std::cout << "Reply 실패: " << cb.get_error().message << std::endl;
    return 0;
}

void Discord_BOT::Create_Command_Log(const dpp::confirmation_callback_t& cb, const dpp::slashcommand& cmd){
    if(cb.is_error()) std::cout << cmd.name << " 명령어 등록 실패: " << cb.get_error().message << std::endl;
    else std::cout << cmd.name << " 명령어 등록 성공" << std::endl;
}