#include <iostream>
#include "../CsvReader/CsvReader.h"
#include "../ResponseReceiver/ResponseReceiver.h"
#include "../NetworkClient/NetworkClient.h"
#include "../../DataProtocol/DataProtocol.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#define NUM_THREADS 8

int main(void)
{
    Client::CsvReader csv_reader("DataFile.txt");

    Client::ResponseReceiver response_receiver(&std::cout);
    
    Client::NetworkClient network_client(
        // This lambda defines what to do whenever the client
        // receives data. IT NEEDS TO BE THREAD SAFE
        [&response_receiver](std::unique_ptr<std::byte> buf) {

            response_receiver.addResponse(std::move(buf));

        }
    );

    while (!csv_reader.isAtEnd())
    {
        std::unique_ptr<DataProtocol::ClientTransmission> next = csv_reader.getNextTransmission();

        std::size_t transmission_size = next->getPayloadSize();

        network_client.send(std::move(next->getPayload()), transmission_size);
    }

    response_receiver.waitUntilDone();

    return 0;
}