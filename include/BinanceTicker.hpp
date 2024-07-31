#ifndef BINANCE_TICKER_HPP
#define BINANCE_TICKER_HPP

#include "WebSocketClient.hpp"
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>

class BinanceTicker {
public:
    explicit BinanceTicker(const std::string &symbol, std::shared_ptr<WebSocketClient> websocket_client);

    ~BinanceTicker();

    void start();

    void stop();

private:
    void run();

    void on_open(websocketpp::connection_hdl hdl);

    void on_fail(websocketpp::connection_hdl hdl);

    void on_close(websocketpp::connection_hdl hdl);

    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);

    void on_pong(websocketpp::connection_hdl hdl, std::string payload);

    void start_ping_timer();

    std::string m_symbol;
    std::shared_ptr<WebSocketClient> m_websocket_client;
    websocketpp::connection_hdl m_connection;
    std::atomic<bool> m_should_run;
    std::chrono::seconds m_reconnect_interval;
    std::unique_ptr<websocketpp::lib::asio::steady_timer> m_ping_timer;
    std::thread m_thread;
    std::mutex m_mutex;
};

#endif // BINANCE_TICKER_HPP
