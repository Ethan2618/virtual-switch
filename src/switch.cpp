#include "switch.h"


std::ostream& operator<<(std::ostream& os, const ForwardingResult& result){
        switch(result.decision) {
            case ForwardingDecision::DROP:       os << "DROP" << std::endl;
                                                 break;
            case ForwardingDecision::FLOOD:      os << "FLOOD" << std::endl;
                                                 break;
            case ForwardingDecision::FORWARD:    os << "FORWARD TO " << static_cast<int>(result.port) << std::endl;    
                                                 break;

        }
        return os;
    }

ForwardingResult Switch::processFrame(const EthernetFrame& frame, Port incoming_port){
    fdb[frame.source] = incoming_port;
    auto port_to_forward = fdb.find(frame.destination);
    if(port_to_forward == fdb.end()){
        return {ForwardingDecision::FLOOD, 0};
    } else if(port_to_forward->second == incoming_port){
        return {ForwardingDecision::DROP, 0};   //change later to std::optional
    } else {
        return {ForwardingDecision::FORWARD, port_to_forward->second};
    }
}