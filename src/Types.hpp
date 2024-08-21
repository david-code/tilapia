#pragma once

#include <format>

struct SimpleFormatter
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }
};

enum class EtherType : std::uint16_t
{
    InternetProtocolVersion4 = 0x800,
    AddressResolutionProtocol = 0x806,
    ReverseAddressResolutionProtocol = 0x8035,
    InternetProtocolVersion6 = 0x86DD,
};

template <> struct std::formatter<EtherType> : SimpleFormatter
{
    template <typename FormatContext>
    auto format(const EtherType& etherType, FormatContext& ctx) const
    {
        using enum EtherType;
        switch (etherType)
        {
        case InternetProtocolVersion4:
            return std::format_to(ctx.out(), "IPv4");
        case AddressResolutionProtocol:
            return std::format_to(ctx.out(), "ARP");
        case ReverseAddressResolutionProtocol:
            return std::format_to(ctx.out(), "RARP");
        case InternetProtocolVersion6:
            return std::format_to(ctx.out(), "IPv6");
        default:
            throw std::runtime_error{std::format("Unexpected ethertype: {}", std::to_underlying(etherType))};
        }
    }
};



struct MacAddress
{
    char mValue[6];
};

template <> struct std::formatter<MacAddress> : SimpleFormatter
{
    template <typename FormatContext>
    auto format(const MacAddress& address, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{:02X}-{:02X}-{:02X}-{:02X}-{:02X}-{:02X}", address.mValue[5], address.mValue[4],
        address.mValue[3], address.mValue[2], address.mValue[1], address.mValue[0]);
    }
};

struct IpAddress
{
    char mValue[4];
};

template <> struct std::formatter<IpAddress> : SimpleFormatter
{
    template <typename FormatContext>
    auto format(const IpAddress& address, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{:d}.{:d}.{:d}.{:d}", address.mValue[3], address.mValue[2],
        address.mValue[1], address.mValue[0]);
    }
};
