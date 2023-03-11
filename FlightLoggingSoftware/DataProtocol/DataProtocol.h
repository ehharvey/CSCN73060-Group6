#include <iostream>
#include <memory>

#pragma once

namespace DataProtocol {
    // 64 bits (8 bytes): Flight ID
    // 64 bits (8 bytes): Second delta
    // 64 bits (8 bytes): fuel level
    // 24 bytes total
    const uint_fast8_t PACKET_SIZE = sizeof(uint64_t) * 3;

    class ClientTransmission
    {
        private:
        std::array<std::byte, PACKET_SIZE> payload;

        public:
        ClientTransmission(
            uint_fast64_t flight_id,
            uint_fast64_t secondDelta,
            uint_fast64_t fuelLevel
        );

        ClientTransmission(
            std::unique_ptr<std::byte> payload
        );

        uint_fast64_t getFlightId();
        uint_fast64_t getSecondDelta();
        uint_fast64_t getFuelLevel();

        std::byte* getPayload();
    };
}
