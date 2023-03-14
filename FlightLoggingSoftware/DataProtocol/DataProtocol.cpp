#include "DataProtocol.h"
#include <cstring>

namespace DataProtocol {

const uint_fast8_t flightIdOffset = 0;
const uint_fast8_t secondDeltaOffset = flightIdOffset + sizeof(uint_fast64_t);
const uint_fast8_t fuelLevelOffset = secondDeltaOffset + sizeof(uint_fast64_t);

ClientTransmission::ClientTransmission(uint_fast64_t flight_id,
                                       uint_fast64_t secondDelta,
                                       uint_fast64_t fuelLevel) {

  std::memcpy(payload.data() + flightIdOffset, &flight_id, sizeof(flight_id));
  std::memcpy(payload.data() + secondDeltaOffset, &secondDelta,
              sizeof(secondDelta));
  std::memcpy(payload.data() + fuelLevelOffset, &fuelLevel, sizeof(fuelLevel));
}

ClientTransmission::ClientTransmission(std::byte *buffer) {
  std::memcpy(payload.data(), buffer, PACKET_SIZE);
}

uint_fast64_t ClientTransmission::getFlightId() {
  uint_fast64_t flight_id;
  std::memcpy(&flight_id, payload.data() + flightIdOffset, sizeof(flight_id));
  return flight_id;
}

uint_fast64_t ClientTransmission::getSecondDelta() {
  uint_fast64_t secondDelta;
  std::memcpy(&secondDelta, payload.data() + secondDeltaOffset,
              sizeof(secondDelta));
  return secondDelta;
}

uint_fast64_t ClientTransmission::getFuelLevel() {
  uint_fast64_t fuelLevel;
  std::memcpy(&fuelLevel, payload.data() + fuelLevelOffset, sizeof(fuelLevel));
  return fuelLevel;
}

std::byte *ClientTransmission::getPayload() { return payload.begin(); }

} // namespace DataProtocol