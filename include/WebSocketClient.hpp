#ifndef WEBSOCKET_CLIENT_HPP
#define WEBSOCKET_CLIENT_HPP

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/asio/ssl/context.hpp>
#include <memory>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

class WebSocketClient {
public:
    WebSocketClient();

    client &getClient();

    websocketpp::connection_hdl connect(const std::string &uri);

private:
    std::unique_ptr<client> m_endpoint;
};

#endif // WEBSOCKET_CLIENT_HPP
