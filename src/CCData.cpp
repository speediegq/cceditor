/* cceditor
 * Geometry Dash save editor
 * Licensed under the GNU General Public License version 3.0.
 *
 * See included LICENSE file for copyright and license details.
 */
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <zlib.h>

#include "CCData.hpp"

void CCData::CCData::setXorKey(uint8_t _key) { // probably useless method, but for future proofing i guess
    key = _key;
}

std::vector<uint8_t> CCData::CCData::XorEncryptDecrypt(const std::vector<uint8_t>& data, uint8_t key) {
    std::vector<uint8_t> decrypted;
    decrypted.reserve(data.size());

    for (uint8_t byte : data) {
        decrypted.push_back(byte ^ key);
    }

    return decrypted;
}

/* I won't even pretend to understand what this does in detail because quite frankly, I am useless with
 * algorithms. It works, though.
 */
std::string CCData::CCData::Base64Encode(const std::vector<uint8_t>& data) {
    const std::string base64_a =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-_";

    std::stringstream ret;
    size_t len = data.size();
    size_t i = 0;

    while (i < len) {
        uint32_t a = i < len ? static_cast<uint8_t>(data[i++]) : 0;
        uint32_t b = i < len ? static_cast<uint8_t>(data[i++]) : 0;
        uint32_t c = i < len ? static_cast<uint8_t>(data[i++]) : 0;

        uint32_t triple = (a << 16) + (b << 8) + c;

        ret << base64_a[(triple >> 18) & 0x3F]
                << base64_a[(triple >> 12) & 0x3F]
                << base64_a[(triple >> 6) & 0x3F]
                << base64_a[triple & 0x3F];
    }

    size_t pad = len % 3;
    if (pad) {
        ret.seekp(-static_cast<int>(3 - pad), std::ios_base::end);
        for (size_t j = 0; j < 3 - pad; ++j) {
            ret << '=';
        }
    }

    return ret.str();
}

std::vector<uint8_t> CCData::CCData::Base64Decode(const std::string& data) {
    const std::string base64_a =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-_";

    std::vector<uint8_t> ret;
    int three = 0;
    uint32_t four = 0;

    for (char c : data) {
        if (std::isspace(c)) {
            continue;
        }

        if ((c == '=') || (c == '\0')) {
            break;
        }

        if (std::isalnum(c) || (c == '-') || (c == '_')) {
            four = (four << 6) | base64_a.find(c);
            three += 6;

            if (three >= 8) {
                three -= 8;
                ret.push_back(static_cast<uint8_t>((four >> three) & 0xFF));
            }
        } else {
            throw std::runtime_error{"invalid string"};
        }
    }

    return ret;
}

std::vector<uint8_t> CCData::CCData::GzipCompress(const std::vector<uint8_t>& data) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error{"failed to deflate"};
    }

    zs.next_in = const_cast<Bytef*>(data.data());
    zs.avail_in = static_cast<uInt>(data.size());

    int status;
    std::vector<uint8_t> buffer(4096);
    std::vector<uint8_t> ret;

    do {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(buffer.size());

        status = deflate(&zs, Z_FINISH);

        if (ret.size() < zs.total_out) {
            std::copy(buffer.begin(), buffer.begin() + (zs.total_out - ret.size()), std::back_inserter(ret));
        }

    } while (status == Z_OK);

    deflateEnd(&zs);

    if (status != Z_STREAM_END) {
        throw std::runtime_error{"failed to compress"};
    }

    return ret;
}

std::vector<uint8_t> CCData::CCData::GzipDecompress(const std::vector<uint8_t>& data) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (inflateInit2(&zs, 16 + MAX_WBITS) != Z_OK) {
        throw std::runtime_error{"failed to inflate"};
    }

    zs.next_in = const_cast<Bytef*>(data.data());
    zs.avail_in = static_cast<uInt>(data.size());

    int status;
    std::vector<uint8_t> buffer(4096);
    std::vector<uint8_t> ret;

    do {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(buffer.size());

        status = inflate(&zs, Z_NO_FLUSH);

        if (ret.size() < zs.total_out) {
            std::copy(buffer.begin(), buffer.begin() + (zs.total_out - ret.size()), std::back_inserter(ret));
        }

    } while (status == Z_OK);

    inflateEnd(&zs);

    if (status != Z_STREAM_END) {
        throw std::runtime_error{"failed to decompress"};
    }

    return ret;
}

std::vector<uint8_t> CCData::CCData::Encrypt(std::vector<uint8_t>& data) {
    uint8_t key = 11; // this might need to be changed if gigachad robtop changes the XOR key

    std::string base64Encoded = Base64Encode(GzipCompress(data));
    return XorEncryptDecrypt(std::vector<uint8_t>(base64Encoded.begin(), base64Encoded.end()), key);
}

std::vector<uint8_t> CCData::CCData::Decrypt(std::vector<uint8_t>& data) {
    uint8_t key = 11; // this might need to be changed if gigachad robtop changes the XOR key

    std::vector<uint8_t> decrypted = XorEncryptDecrypt(data, key);
    std::string base64Decoded(decrypted.begin(), decrypted.end());

    return GzipDecompress(Base64Decode(base64Decoded));
}
