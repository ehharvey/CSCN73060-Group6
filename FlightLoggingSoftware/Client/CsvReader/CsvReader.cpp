#include "CsvReader.h"
#include <ctime>
#include <regex>

namespace Client {

extern uint_fast64_t flight_id;
extern uint_fast64_t initialTime;

CsvReader::CsvReader(std::filesystem::path path) {
  try {
    ifs = std::ifstream(path, std::ios::in);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void CsvReader::start() {}

bool CsvReader::isAtEnd() { return ifs.eof(); }

std::unique_ptr<DataProtocol::ClientTransmission>
CsvReader::getNextTransmission() {
  std::string line;
  std::smatch match;

  while (std::getline(ifs, line)) {
    std::regex pattern(R"((\d+)_(\d+)_(\d+) (\d+):(\d+):(\d+),(\d+).(\d+))");

    if (std::regex_search(line, match, pattern)) {
      std::tm datetime{};
      datetime.tm_mday = std::stoi(match[1]);
      datetime.tm_mon = std::stoi(match[2]) - 1;     // month is 0 - 11
      datetime.tm_year = std::stoi(match[3]) - 1900; // year starts from 1900
      datetime.tm_hour = std::stoi(match[4]);
      datetime.tm_min = std::stoi(match[5]);
      datetime.tm_sec = std::stoi(match[6]);

      uint_fast64_t secondDelta = 0;

      // First time finding time
      if (Client::initialTime == 0) {
        Client::initialTime = std::mktime(&datetime);
      } else {
        secondDelta = std::mktime(&datetime) - Client::initialTime;
      }

      // Fuel Level
      std::string fuelLevelText = match[7].str() + match[8].str();
      uint_fast64_t fuelLevel = std::stoi(fuelLevelText);

      return std::unique_ptr<DataProtocol::ClientTransmission>(
          new DataProtocol::ClientTransmission(Client::flight_id, secondDelta,
                                               fuelLevel));
    }
  }
  return nullptr;
}
} // namespace Client