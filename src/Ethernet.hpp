#pragma once

#include <Headers.hpp>
#include <Types.hpp>

#include <cstdint>
#include <cstring>
#include <format>
#include <print>
#include <utility>

enum class EtherType : std::uint16_t
{
    InternetProtocolVersion4 = 0x800,
    AddressResolutionProtocol = 0x806,
    ReverseAddressResolutionProtocol = 0x8035,
    InternetProtocolVersion6 = 0x86DD,
};

struct EthernetHeader
{
    MacAddress mDestinationMacAddress;
    MacAddress mSourceMacAddress;
    EtherType  mEthertype;
};
static_assert(sizeof(EthernetHeader) == 14, "Ethernet header must be 14 bytes long");

template <>
struct LayoutInfo<EthernetHeader>
{
    static constexpr std::index_sequence<6, 6, 2> Sizes{};
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

template <> struct std::formatter<EthernetHeader> : SimpleFormatter
{
    template <typename FormatContext>
    auto format(const EthernetHeader& header, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}: {} -> {}", header.mEthertype, header.mSourceMacAddress, header.mDestinationMacAddress);
    }
};
