#include <iostream>
#include "../../DataProtocol/DataProtocol.h"
#include "../AverageCalculator/AverageCalculator.h"
#include "../DataStore/DataStore.h"
#include "../NetworkServer/NetworkServer.h"

int main(void)
{
    Server::AverageCalculator average_calculator;
    Server::DataStore data_store;


    Server::NetworkServer network_server(
        // This lambda defines what to do whenever the server receives a packet
        // This NEEDS TO BE THREAD SAFE
        [&] (std::unique_ptr<std::byte> payload)
        {
            auto parsed = DataProtocol::ServerTransmissionParser(std::move(payload));
            
            // Use average_calculator to get new average

            // Use data_store to add new value

            // Return to sender
            return std::unique_ptr<std::byte>();
        }
    );


    network_server.waitUntilStops();

    return 0;
}