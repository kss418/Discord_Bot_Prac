#include "Discord_BOT.h"
#include <iostream>

void Discord_BOT::Command_Create_Log(const dpp::confirmation_callback_t& cb, const dpp::slashcommand& cmd){
    if(cb.is_error()) std::cout << cmd.name << " 명령어 등록 실패: " << cb.get_error().message << std::endl;
    else std::cout << cmd.name << " 명령어 등록 성공" << std::endl;
}
