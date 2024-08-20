#include <tap.hpp>
#include <Ethernet.hpp>
#include <Ip.hpp>

#include <bit>
#include <iostream>
#include <iomanip>
#include <print>

int main()
{
    if constexpr (std::endian::native == std::endian::little)
    {
        std::println("Host is little endian");
    }
    else
    {
        std::println("Host is not little endian, disaster!");
        return 1;
    }


    TapDevice tap{};
    std::println("Created tap device {} : descriptor {}", tap.name(), tap.descriptor());

    int messagesRemaining{100};

    char buffer[2000];
    while (messagesRemaining)
    {
        int bytesRead = read(tap.descriptor(), buffer, sizeof(buffer));
        messagesRemaining -= 1;
        if (bytesRead < 0)
        {
            std::println("Failed to read from Tap Device");
        }

        if (bytesRead < sizeof(EthernetHeader))
        {
            std::println("Received dodgy message of size {}", bytesRead);
        }

        std::size_t offset{0};
        auto ethernetHeader = fromWire<EthernetHeader>(buffer);
        offset += sizeof(ethernetHeader);

        std::println("Received a message of size {}, Ethernet Header: {}", bytesRead, ethernetHeader);
        if (ethernetHeader.mEthertype != EtherType::InternetProtocolVersion4)
        {
            continue;
        }

        auto ipHeader = fromWire<IpV4Header>(buffer + offset);
        std::println("IP Packet of version {}, header length {}, protocol {}, total length {}", int{ipHeader.mVersionLength.mVersion}, 
        int{ipHeader.mVersionLength.mLength}, ipHeader.mProto, ipHeader.mTotalLength);

        std::cout << std::flush;
    }
}
