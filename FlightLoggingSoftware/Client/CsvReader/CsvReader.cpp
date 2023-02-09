#include "CsvReader.h"

Client::CsvReader::CsvReader(std::filesystem::path path)
{
}


bool Client::CsvReader::isAtEnd()
{
    return true;
}

std::unique_ptr<DataProtocol::ClientTransmission> Client::CsvReader::getNextTransmission()
{
    return std::unique_ptr<DataProtocol::ClientTransmission>();
}
