#include "CsvReader.h"
#include <regex>
#include <time.h>

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
  struct tm tm {};

  while (std::getline(ifs, line)) {
    const char *strp_res = strptime(line.c_str(), "%d_%m_%Y %H:%M:%S", &tm);

    // If didn't find the time
    if (strp_res == nullptr)
      continue;

    uint_fast64_t secondDelta = 0;

    // First time finding time
    if (Client::initialTime == 0) {
      Client::initialTime = std::mktime(&tm);
    } else {
      secondDelta = std::mktime(&tm) - Client::initialTime;
    }
    std::string subString(strp_res + 1);
    // To get fuel as Int, remove "."
    uint_fast64_t fuelLevel =
        std::stoi(subString.erase(subString.find("."), 1));

    return std::unique_ptr<DataProtocol::ClientTransmission>(
        new DataProtocol::ClientTransmission(Client::flight_id, secondDelta,
                                             fuelLevel));
  }
}

} // namespace Client
