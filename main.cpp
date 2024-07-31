#include "include/BinanceTicker.hpp"
#include "include/WebSocketClient.hpp"
#include <iostream>
#include <memory>

int main() {
    try {
        std::string symbol = "btcusdt";
        auto websocket_client = std::make_shared<WebSocketClient>();
        BinanceTicker ticker(symbol, websocket_client);

        ticker.start();

        std::cout << "Press Enter to stop the program..." << std::endl;
        std::cin.get();

        ticker.stop();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    return 0;
}
