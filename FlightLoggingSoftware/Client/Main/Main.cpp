#include <boost/asio.hpp>
#include <iostream>

int main() {
    try {
        // Create an io_context object
        boost::asio::io_context io_context;

        // Create a socket object
        boost::asio::ip::tcp::socket socket(io_context);

        // Resolve the localhost address and port number
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", "1234");

        // Connect to the server
        boost::asio::connect(socket, endpoints);

        // Send 24 bytes of data to the server
        std::string message = "This is a test message.";
        boost::asio::write(socket, boost::asio::buffer(message, 24));

        // Close the socket
        socket.close();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
