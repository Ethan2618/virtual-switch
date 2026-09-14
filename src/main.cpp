#include <iostream>
#include <unordered_map>
#include "frame.h"
#include "switch.h"

int main()
{
    Switch mySwitch;
    MacAddress mac1{{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}};
    MacAddress mac2{{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}};

    EthernetFrame frame1{mac2, mac1, {0x01, 0x02, 0xFF, 128}};
    std::cout << mySwitch.processFrame(frame1, Port(1));

    EthernetFrame frame2{mac1, mac2, {0x01, 0x02, 0xFF, 132}};
    std::cout << mySwitch.processFrame(frame2, Port(2));

    std::cout << mySwitch.processFrame(frame1, Port(3));
    std::cout << mySwitch.processFrame(frame2, Port(2));

    std::cout << mySwitch.processFrame(frame1, Port(2));


    return 0;
}