#include "../../DataProtocol/DataProtocol.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>

#pragma once

namespace Client {
// Notes:
// * If a Client wants to read from many files
//   then we might need a container class (that contains CsvReader)
//   and only spawns like 5 CsvReaders at a time
//   We want to avoid spawning a ton of threads that each read from a file

class CsvReader {
private:
  std::ifstream ifs;

public:
  // Opens the file
  // Starts the worker
  // On error:
  //   worker does not start
  //   Throws exception (or just crash)
  CsvReader(std::filesystem::path path);

  //
  void start();

  // Retrieves the next from the file
  // and constructs a transmission
  // If there is no lines available,
  //   throw exception
  std::unique_ptr<DataProtocol::ClientTransmission> getNextTransmission();

  // Returns True if there are no more lines to retrieve
  bool isAtEnd();
};
} // namespace Client