#include "../../DataProtocol/DataProtocol.h"

namespace Server {

    class AverageCalculator {
        
        public:
        DataProtocol::FlightValue getNewAverage(
            DataProtocol::FlightID id, 
            DataProtocol::FlightValue new_value
            );
    };
}