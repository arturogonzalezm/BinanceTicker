#include "../include/WebSocketClient.hpp"

WebSocketClient::WebSocketClient()
        : m_endpoint(std::make_unique<client>()) {
    m_endpoint->clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint->clear_error_channels(websocketpp::log::elevel::all);
    m_endpoint->init_asio();
    m_endpoint->set_tls_init_handler([](websocketpp::connection_hdl) {
        return std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    });
}

client &WebSocketClient::getClient() {
    return *m_endpoint;
}

websocketpp::connection_hdl WebSocketClient::connect(const std::string &uri) {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = m_endpoint->get_connection(uri, ec);
    if (ec) {
        throw std::runtime_error("Could not create connection: " + ec.message());
    }
    websocketpp::connection_hdl hdl = con->get_handle();
    m_endpoint->connect(con);
    return hdl;
}
