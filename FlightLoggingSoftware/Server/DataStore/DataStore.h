#include "../../DataProtocol/DataProtocol.h"

namespace Server {

    class DataStore {

        public:
        void addValue(DataProtocol::FlightID id, DataProtocol::FlightValue value);
    };
}