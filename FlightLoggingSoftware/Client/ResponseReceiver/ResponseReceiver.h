#pragma once

#include <iostream>
#include <cstddef>
#include <memory>

namespace Client
{
    class ResponseReceiver
    {
    private:
        std::ofstream* out;
    public:
        ResponseReceiver(std::ostream* out);
        ~ResponseReceiver();

        void addResponse(std::unique_ptr<std::byte> buf);

        void waitUntilDone();
    };


} // namespace Client
