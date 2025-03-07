#include <dpp/dpp.h>
#include <cstdlib>
#include <string>

int main() {
    //디스코드 봇 토큰 환경변수 설정하기
    std::string token = std::getenv("DISCORD_BOT_TOKEN");
    dpp::cluster bot(token);

    bot.on_ready([](const dpp::ready_t& event){
        std::cout << "봇이 실행되었습니다!" << std::endl;
    });

    bot.start(dpp::st_wait);

    return 0;
}
