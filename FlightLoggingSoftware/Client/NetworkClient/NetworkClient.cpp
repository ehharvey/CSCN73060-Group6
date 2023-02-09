#include "NetworkClient.h"

Client::NetworkClient::NetworkClient(std::function<void(std::unique_ptr<std::byte> buf)> recv_callback)
{
}

Client::NetworkClient::~NetworkClient()
{
}

void Client::NetworkClient::connect(const char *ip)
{
}

void Client::NetworkClient::send(std::unique_ptr<std::byte> buf, std::size_t size)
{
}
