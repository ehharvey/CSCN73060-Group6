#include "CsvReader.h"

Client::CsvReader::CsvReader(std::filesystem::path path)
{
}

void Client::CsvReader::start()
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
