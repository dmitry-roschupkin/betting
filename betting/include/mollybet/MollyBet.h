#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

#include <string>
#include <set>

namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

namespace betting::mollybet
{

class MollyBet
{
public:
    explicit MollyBet(ssl::context& ctx);

    virtual ~MollyBet() = default;

    net::awaitable<std::string> takeToken();
    net::awaitable<void> runMessageLoop(const std::string &token);

    void printCompetitions() const;

    // std::set<std::string> &getCompetitions();

private:
    static constexpr std::string mbMESSAGE_SYNC = "sync";
    static constexpr std::string mbMESSAGE_EVENT = "event";

    static constexpr int mbMESSAGE_LOOP_STATE_NO_RUN   = 0;
    static constexpr int mbMESSAGE_LOOP_STATE_RUN      = 1;
    static constexpr int mbMESSAGE_LOOP_STATE_STOPPING = 2;

    void onMessageSync(const std::pair<std::string, json::value>& messagePair);
    void onMessageEvent(const std::pair<std::string, json::value>& messagePair);

    static std::string parseTokenData(const std::string& data);

    void processSentMessage(const std::string& sentMessage);
    void processMessagePair(const std::pair<std::string, json::value>& messagePair);

    std::set<std::string> competitions;
    int messageLoopStatus = mbMESSAGE_LOOP_STATE_NO_RUN;

    //This configuration date can be moved to config and we can make singleton class MollyBetConfig to read and store this data
    const std::string username = "devinterview";
    const std::string password = "OwAb6wrocirEv";

    const std::string host = "api.mollybet.com";
    const std::string port = "443";
    const std::string sessionTarget = "/v1/sessions/";
    const std::string streamTargetWithoutToken = "/v1/stream/?token=";
    
    const http::verb method = http::verb::post;

    ssl::context* pContext;
};

}
