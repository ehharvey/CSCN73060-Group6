#include "DataProtocol.h"

DataProtocol::ClientTransmission::ClientTransmission(uint_fast8_t flight_id, uint_fast32_t number_of_columns, std::unique_ptr<float> column_values)
{
}

uint_fast8_t DataProtocol::ClientTransmission::getFlightId()
{
    return uint_fast8_t();
}

uint_fast32_t DataProtocol::ClientTransmission::getNumberOfColumuns()
{
    return uint_fast32_t();
}

uint_fast32_t DataProtocol::ClientTransmission::getNumberOfBytesThatFollow()
{
    return uint_fast32_t();
}

const float *DataProtocol::ClientTransmission::getColumnValues()
{
    return nullptr;
}

std::size_t DataProtocol::ClientTransmission::getPayloadSize()
{
    return std::size_t();
}

std::unique_ptr<std::byte> DataProtocol::ClientTransmission::getPayload()
{
    return std::unique_ptr<std::byte>();
}

DataProtocol::ServerTransmissionParser::ServerTransmissionParser(std::unique_ptr<std::byte> payload)
{
}

uint_fast8_t DataProtocol::ServerTransmissionParser::getFlightId()
{
    return uint_fast8_t();
}

uint_fast32_t DataProtocol::ServerTransmissionParser::getNumberOfBytesThatFollow()
{
    return uint_fast32_t();
}
