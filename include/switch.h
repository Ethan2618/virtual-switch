#pragma once

#include "frame.h"
#include <unordered_map>
#include <iostream>

using Port = uint8_t;

enum class ForwardingDecision {DROP, FLOOD, FORWARD};

struct ForwardingResult{
    ForwardingDecision decision;
    Port port;    //later std::optionnal
};

std::ostream& operator<<(std::ostream& os, const ForwardingResult& result);

class Switch
{
    std::unordered_map<MacAddress, Port, MACHash> fdb;
    

    public:
    ForwardingResult processFrame(const EthernetFrame& frame, Port incoming_port);


};