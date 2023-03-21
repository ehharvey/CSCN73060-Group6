#include <iostream>
#include <memory>
#include <array>
#include <cstddef>
#pragma once

namespace DataProtocol {
// 64 bits (8 bytes): Flight ID
// 64 bits (8 bytes): Second delta
// 64 bits (8 bytes): fuel level
// 24 bytes total
const uint_fast8_t PACKET_SIZE = sizeof(uint64_t) * 3;

const std::array<std::byte, 1> ACK = { std::byte(0) };

class Transmission
{
    public:
    virtual std::byte* getPayload() = 0;
};

class DummyTransmission : public Transmission
{
    private:
    std::array<std::byte, PACKET_SIZE> payload = {
        std::byte('h'), std::byte('e'),
        std::byte('l'), std::byte('l'),
        std::byte('o'), std::byte(' '),
        std::byte('w'), std::byte('o'),
        std::byte('r'), std::byte('l'),
        std::byte('d')
    };

    public:
    std::byte* getPayload();
};

class ClientTransmission : public Transmission {
private:
  std::array<std::byte, PACKET_SIZE> payload;
public:
  ClientTransmission(uint64_t flight_id, uint64_t secondDelta,
                     uint64_t fuelLevel);

  ClientTransmission(std::byte* payload);

  uint_fast64_t getFlightId();
  uint_fast64_t getSecondDelta();
  uint_fast64_t getFuelLevel();

  std::byte *getPayload();
};
} // namespace DataProtocol
