
#include <iostream>
#include <utility>
#include "../include/BinanceTicker.hpp"

BinanceTicker::BinanceTicker(const std::string &symbol, std::shared_ptr<WebSocketClient> websocket_client)
        : m_symbol(symbol),
          m_should_run(true),
          m_reconnect_interval(std::chrono::seconds(5)),
          m_websocket_client(std::move(websocket_client)) {
    auto &endpoint = m_websocket_client->getClient();
    endpoint.set_message_handler([this](auto &&... args) { this->on_message(std::forward<decltype(args)>(args)...); });
    endpoint.set_open_handler([this](auto &&... args) { this->on_open(std::forward<decltype(args)>(args)...); });
    endpoint.set_fail_handler([this](auto &&... args) { this->on_fail(std::forward<decltype(args)>(args)...); });
    endpoint.set_close_handler([this](auto &&... args) { this->on_close(std::forward<decltype(args)>(args)...); });
    endpoint.set_pong_handler([this](auto &&... args) { this->on_pong(std::forward<decltype(args)>(args)...); });

    m_ping_timer = std::make_unique<websocketpp::lib::asio::steady_timer>(endpoint.get_io_service());
}

BinanceTicker::~BinanceTicker() {
    stop();
}

void BinanceTicker::start() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_thread.joinable()) {
        return;  // Already running
    }
    m_should_run = true;
    m_thread = std::thread([this] { run(); });
}

void BinanceTicker::stop() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_should_run = false;
        if (m_connection.lock()) {
            m_websocket_client->getClient().close(m_connection, websocketpp::close::status::normal,
                                                  "Closing connection");
        }
    }
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void BinanceTicker::run() {
    while (m_should_run) {
        try {
            m_connection = m_websocket_client->connect("wss://stream.binance.com:9443/ws/" + m_symbol + "@ticker");
            m_websocket_client->getClient().run();
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }

        if (m_should_run) {
            std::cerr << "Connection lost. Reconnecting in " << m_reconnect_interval.count() << " seconds..."
                      << std::endl;
            std::this_thread::sleep_for(m_reconnect_interval);
        }
    }
}

void BinanceTicker::on_open(websocketpp::connection_hdl hdl) {
    std::cout << "Connection opened" << std::endl;
    start_ping_timer();
}

void BinanceTicker::on_fail(websocketpp::connection_hdl hdl) {
    std::cerr << "Connection failed" << std::endl;
}

void BinanceTicker::on_close(websocketpp::connection_hdl hdl) {
    std::cerr << "Connection closed" << std::endl;
    m_ping_timer->cancel();
}

void BinanceTicker::on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

void BinanceTicker::on_pong(websocketpp::connection_hdl hdl, std::string payload) {
    std::cout << "Pong received" << std::endl;
}

void BinanceTicker::start_ping_timer() {
    m_ping_timer->expires_from_now(std::chrono::seconds(30));
    m_ping_timer->async_wait([this](const websocketpp::lib::error_code &ec) {
        if (!ec && m_connection.lock()) {
            m_websocket_client->getClient().ping(m_connection, "");
            start_ping_timer();
        }
    });
}
