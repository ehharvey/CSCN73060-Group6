#pragma once

#include <iostream>
#include <memory>

namespace DataProtocol {
    typedef uint8_t FlightID;
    typedef uint32_t DataSize;
    typedef float FlightValue;
    

    struct Packet {
        FlightID flight_id;
    };


    class ClientTransmission
    {
        private:
        // Holds the payload
        // Will be null after getPayload() is called
        std::shared_ptr<std::byte> payload;
        std::size_t payload_size;

        public:
        ClientTransmission(
            uint_fast8_t flight_id,
            uint_fast32_t number_of_columns,
            std::unique_ptr<float> column_values
        );

        ClientTransmission(
            std::unique_ptr<std::byte> payload
        );

        uint_fast8_t getFlightId();   
        std::size_t getPayloadSize();
        
        std::shared_ptr<std::byte> getPayload();
    };
    

    class ServerTransmission
    {
        private:
        std::shared_ptr<std::byte> payload;

        public:
        ServerTransmission(std::shared_ptr<std::byte> payload);

        ServerTransmission(uint_fast8_t flight_id);

        uint_fast8_t getFlightId();
        uint_fast32_t getNumberOfBytesThatFollow();
    };
}
