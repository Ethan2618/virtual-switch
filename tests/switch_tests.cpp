#include <gtest/gtest.h>

#include "switch.h"


TEST(SwitchTest, UnknownDestination)
{
    Switch mySwitch;

    MacAddress mac1{{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}};
    MacAddress mac2{{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}};

    EthernetFrame frame{
        mac2,
        mac1,
        {0x01, 0x02, 0xFF, 128}
    };

    auto result = mySwitch.processFrame(frame, Port(1));

    EXPECT_EQ(result.decision, ForwardingDecision::FLOOD);
}


TEST(SwitchTest, KnownDestination)
{
    Switch mySwitch;

    MacAddress mac1{{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}};
    MacAddress mac2{{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}};

    EthernetFrame frame1{
        mac2,
        mac1,
        {0x01, 0x02, 0xFF, 128}
    };

    EthernetFrame frame2{
        mac1,
        mac2,
        {0x01, 0x02, 0xFF, 132}
    };

    // Learn CC on port 1.
    mySwitch.processFrame(frame1, Port(1));

    // AA sends to CC from port 2.
    auto result = mySwitch.processFrame(frame2, Port(2));

    EXPECT_EQ(result.decision, ForwardingDecision::FORWARD);
    EXPECT_EQ(result.port, Port(1));
}


TEST(SwitchTest, MacMovement)
{
    Switch mySwitch;

    MacAddress mac1{{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}};
    MacAddress mac2{{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}};

    EthernetFrame frame1{
        mac2,
        mac1,
        {0x01, 0x02, 0xFF, 128}
    };

    EthernetFrame frame2{
        mac1,
        mac2,
        {0x01, 0x02, 0xFF, 132}
    };

    // CC is initially learned on port 1.
    mySwitch.processFrame(frame1, Port(1));

    // AA is learned on port 2.
    mySwitch.processFrame(frame2, Port(2));

    // CC moves to port 3.
    auto result = mySwitch.processFrame(frame1, Port(3));

    // AA is still on port 2.
    EXPECT_EQ(result.decision, ForwardingDecision::FORWARD);
    EXPECT_EQ(result.port, Port(2));
}


TEST(SwitchTest, UpdatedMacLocation)
{
    Switch mySwitch;

    MacAddress mac1{{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}};
    MacAddress mac2{{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}};

    EthernetFrame frame1{
        mac2,
        mac1,
        {0x01, 0x02, 0xFF, 128}
    };

    EthernetFrame frame2{
        mac1,
        mac2,
        {0x01, 0x02, 0xFF, 132}
    };

    // CC starts on port 1.
    mySwitch.processFrame(frame1, Port(1));

    // AA is on port 2.
    mySwitch.processFrame(frame2, Port(2));

    // CC moves to port 3.
    mySwitch.processFrame(frame1, Port(3));

    // AA sends to CC from port 2.
    auto result = mySwitch.processFrame(frame2, Port(2));

    EXPECT_EQ(result.decision, ForwardingDecision::FORWARD);
    EXPECT_EQ(result.port, Port(3));
}


TEST(SwitchTest, SamePortDrop)
{
    Switch mySwitch;

    MacAddress mac1{{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}};
    MacAddress mac2{{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}};

    EthernetFrame frame1{
        mac2,
        mac1,
        {0x01, 0x02, 0xFF, 128}
    };

    EthernetFrame frame2{
        mac1,
        mac2,
        {0x01, 0x02, 0xFF, 132}
    };

    // Learn CC on port 1.
    mySwitch.processFrame(frame1, Port(1));

    // Learn AA on port 2.
    mySwitch.processFrame(frame2, Port(2));

    // CC sends to AA from port 2.
    auto result = mySwitch.processFrame(frame1, Port(2));

    EXPECT_EQ(result.decision, ForwardingDecision::DROP);
}
