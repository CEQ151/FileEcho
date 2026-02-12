/*
 * Self-contained DEFLATE (RFC 1951) decompressor
 * No external dependencies (no zlib required)
 * Public domain / Unlicense
 *
 * Usage:
 *   #include "deflate_inflate.h"
 *   std::string decompressed = DeflateInflate::Inflate(compressed_data, compressed_size);
 *   std::string decompressed = DeflateInflate::InflateZlib(zlib_wrapped_data, size);
 */
#pragma once

#include <string>
#include <cstring>
#include <cstdint>

namespace DeflateInflate {

namespace detail {

struct BitStream {
    const uint8_t* data;
    size_t len;
    size_t byte_pos;
    uint32_t bit_buf;
    int bit_count;

    BitStream(const uint8_t* d, size_t l)
        : data(d), len(l), byte_pos(0), bit_buf(0), bit_count(0) {}

    uint32_t readBits(int count) {
        while (bit_count < count) {
            if (byte_pos >= len) return 0;
            bit_buf |= (uint32_t)data[byte_pos++] << bit_count;
            bit_count += 8;
        }
        uint32_t val = bit_buf & ((1u << count) - 1);
        bit_buf >>= count;
        bit_count -= count;
        return val;
    }

    void alignToByte() {
        bit_buf = 0;
        bit_count = 0;
    }

    uint8_t readByte() {
        if (byte_pos >= len) return 0;
        return data[byte_pos++];
    }
};

struct HuffTable {
    uint16_t counts[16];
    uint16_t symbols[320];
};

inline bool buildHuffTable(HuffTable& tab, const uint16_t* lengths, int num) {
    std::memset(tab.counts, 0, sizeof(tab.counts));
    for (int i = 0; i < num; i++) {
        if (lengths[i] > 15) return false;
        tab.counts[lengths[i]]++;
    }
    tab.counts[0] = 0;

    uint16_t offsets[16];
    offsets[0] = 0;
    for (int i = 1; i < 16; i++)
        offsets[i] = offsets[i - 1] + tab.counts[i - 1];

    for (int i = 0; i < num; i++) {
        if (lengths[i] > 0)
            tab.symbols[offsets[lengths[i]]++] = (uint16_t)i;
    }
    return true;
}

inline int decodeSymbol(BitStream& bs, const HuffTable& tab) {
    int code = 0, first = 0, index = 0;
    for (int len = 1; len <= 15; len++) {
        code |= bs.readBits(1);
        int count = tab.counts[len];
        if (code - first < count)
            return tab.symbols[index + code - first];
        index += count;
        first = (first + count) << 1;
        code <<= 1;
    }
    return -1;
}

inline void buildFixedTables(HuffTable& litlen, HuffTable& dist) {
    uint16_t lengths[320];
    for (int i = 0; i <= 143; i++) lengths[i] = 8;
    for (int i = 144; i <= 255; i++) lengths[i] = 9;
    for (int i = 256; i <= 279; i++) lengths[i] = 7;
    for (int i = 280; i <= 287; i++) lengths[i] = 8;
    buildHuffTable(litlen, lengths, 288);

    for (int i = 0; i < 32; i++) lengths[i] = 5;
    buildHuffTable(dist, lengths, 32);
}

// RFC 1951 tables
static const uint16_t lenBase[] = {
    3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,
    35,43,51,59,67,83,99,115,131,163,195,227,258
};
static const uint16_t lenExtra[] = {
    0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,
    3,3,3,3,4,4,4,4,5,5,5,5,0
};
static const uint16_t distBase[] = {
    1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
    257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577
};
static const uint16_t distExtra[] = {
    0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,
    7,7,8,8,9,9,10,10,11,11,12,12,13,13
};
static const uint16_t clOrder[] = {
    16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
};

inline bool readDynamicTables(BitStream& bs, HuffTable& litlen, HuffTable& dist) {
    int hlit = bs.readBits(5) + 257;
    int hdist = bs.readBits(5) + 1;
    int hclen = bs.readBits(4) + 4;

    uint16_t clLengths[19] = {0};
    for (int i = 0; i < hclen; i++)
        clLengths[clOrder[i]] = (uint16_t)bs.readBits(3);

    HuffTable clTable;
    buildHuffTable(clTable, clLengths, 19);

    uint16_t lengths[320] = {0};
    int total = hlit + hdist;

    for (int i = 0; i < total;) {
        int sym = decodeSymbol(bs, clTable);
        if (sym < 0) return false;
        if (sym < 16) {
            lengths[i++] = (uint16_t)sym;
        } else if (sym == 16) {
            int rep = bs.readBits(2) + 3;
            uint16_t prev = i > 0 ? lengths[i - 1] : 0;
            while (rep-- > 0 && i < total) lengths[i++] = prev;
        } else if (sym == 17) {
            int rep = bs.readBits(3) + 3;
            while (rep-- > 0 && i < total) lengths[i++] = 0;
        } else if (sym == 18) {
            int rep = bs.readBits(7) + 11;
            while (rep-- > 0 && i < total) lengths[i++] = 0;
        } else {
            return false;
        }
    }

    buildHuffTable(litlen, lengths, hlit);
    buildHuffTable(dist, lengths + hlit, hdist);
    return true;
}

} // namespace detail

// Inflate raw DEFLATE data (RFC 1951)
inline std::string Inflate(const uint8_t* data, size_t dataLen) {
    using namespace detail;
    BitStream bs(data, dataLen);
    std::string output;
    output.reserve(dataLen * 3);
    const size_t MAX_OUTPUT = 10 * 1024 * 1024; // 10MB safety limit

    int bfinal;
    do {
        bfinal = bs.readBits(1);
        int btype = bs.readBits(2);

        if (btype == 0) {
            // Stored block
            bs.alignToByte();
            uint32_t len = bs.readByte() | ((uint32_t)bs.readByte() << 8);
            bs.readByte(); bs.readByte(); // skip nlen
            for (uint32_t i = 0; i < len && output.size() < MAX_OUTPUT; i++)
                output += (char)bs.readByte();
        } else if (btype == 1 || btype == 2) {
            HuffTable litlen, dist;
            if (btype == 1) {
                buildFixedTables(litlen, dist);
            } else {
                if (!readDynamicTables(bs, litlen, dist)) return "";
            }
            while (output.size() < MAX_OUTPUT) {
                int sym = decodeSymbol(bs, litlen);
                if (sym < 0) return output;
                if (sym < 256) {
                    output += (char)sym;
                } else if (sym == 256) {
                    break;
                } else {
                    int lenIdx = sym - 257;
                    if (lenIdx < 0 || lenIdx >= 29) return output;
                    uint32_t length = lenBase[lenIdx] + bs.readBits(lenExtra[lenIdx]);
                    int distSym = decodeSymbol(bs, dist);
                    if (distSym < 0 || distSym >= 30) return output;
                    uint32_t distance = distBase[distSym] + bs.readBits(distExtra[distSym]);
                    if (distance > output.size()) return output;
                    for (uint32_t i = 0; i < length && output.size() < MAX_OUTPUT; i++)
                        output += output[output.size() - distance];
                }
            }
        } else {
            return ""; // btype 3 is reserved/error
        }
    } while (!bfinal);

    return output;
}

// Inflate zlib-wrapped data (RFC 1950: 2-byte header + raw DEFLATE + 4-byte Adler-32 checksum)
inline std::string InflateZlib(const uint8_t* data, size_t dataLen) {
    if (dataLen < 6) return "";

    // Check for zlib header (CMF byte: bits 0-3 = CM, should be 8 for DEFLATE)
    bool hasZlibHeader = false;
    if ((data[0] & 0x0F) == 8) {
        uint32_t check = ((uint32_t)data[0] << 8) | data[1];
        if (check % 31 == 0)
            hasZlibHeader = true;
    }

    if (hasZlibHeader) {
        size_t offset = 2;
        if (data[1] & 0x20) offset += 4; // FDICT present
        if (dataLen <= offset + 4) return "";
        return Inflate(data + offset, dataLen - offset - 4);
    }

    // Try raw DEFLATE
    return Inflate(data, dataLen);
}

// Convenience overloads for std::string
inline std::string Inflate(const std::string& data) {
    return Inflate(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

inline std::string InflateZlib(const std::string& data) {
    return InflateZlib(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

} // namespace DeflateInflate
