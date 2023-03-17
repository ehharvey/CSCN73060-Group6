#include "DataProtocol.h"
#include <cstring>

namespace DataProtocol {

const uint_fast8_t flightIdOffset = 0;
const uint_fast8_t secondDeltaOffset = flightIdOffset + sizeof(uint64_t);
const uint_fast8_t fuelLevelOffset = secondDeltaOffset + sizeof(uint64_t);

ClientTransmission::ClientTransmission(uint64_t flight_id,
                                       uint64_t secondDelta,
                                       uint64_t fuelLevel) {

  std::memcpy(payload.data() + flightIdOffset, &flight_id, sizeof(flight_id));
  std::memcpy(payload.data() + secondDeltaOffset, &secondDelta,
              sizeof(secondDelta));
  std::memcpy(payload.data() + fuelLevelOffset, &fuelLevel, sizeof(fuelLevel));
}

ClientTransmission::ClientTransmission(std::byte *buffer) {
  std::memcpy(payload.data(), buffer, PACKET_SIZE);
}

uint64_t ClientTransmission::getFlightId() {
  uint64_t result;
  memcpy(&result, payload.data() + flightIdOffset, sizeof(result));
  return result;
}

uint64_t ClientTransmission::getSecondDelta() {
  uint64_t result;
  memcpy(&result, payload.data() + secondDeltaOffset, sizeof(result));
  return result;
}

uint64_t ClientTransmission::getFuelLevel() {
  uint64_t result;
  memcpy(&result, payload.data() + fuelLevelOffset, sizeof(result));
  return result;
}

std::byte *ClientTransmission::getPayload() { return payload.data(); }

} // namespace DataProtocol

std::byte *DataProtocol::DummyTransmission::getPayload()
{
    return this->payload.data();
}
