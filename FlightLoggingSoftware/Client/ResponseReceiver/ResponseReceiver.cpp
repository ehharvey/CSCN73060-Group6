#include "ResponseReceiver.h"

Client::ResponseReceiver::ResponseReceiver(std::ostream *out)
{
}

Client::ResponseReceiver::~ResponseReceiver()
{
}

void Client::ResponseReceiver::addResponse(std::unique_ptr<std::byte> buf)
{
}

void Client::ResponseReceiver::waitUntilDone()
{
}
