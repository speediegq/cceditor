/* cceditor
 * Geometry Dash save editor
 * Licensed under the GNU General Public License version 3.0.
 *
 * See included LICENSE file for copyright and license details.
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace CCData {
    class CCData {
        private:
            uint8_t key = 11; // this might need to be changed if gigachad robtop changes the XOR key
        public:
            void setXorKey(uint8_t _key); // probably useless method, but for future proofing i guess

            std::vector<uint8_t> XorEncryptDecrypt(const std::vector<uint8_t>& data, uint8_t key);
            std::string Base64Encode(const std::vector<uint8_t>& data);
            std::vector<uint8_t> Base64Decode(const std::string& data);
            std::vector<uint8_t> GzipCompress(const std::vector<uint8_t>& data);
            std::vector<uint8_t> GzipDecompress(const std::vector<uint8_t>& data);
            std::vector<uint8_t> Encrypt(std::vector<uint8_t>& data);
            std::vector<uint8_t> Decrypt(std::vector<uint8_t>& data);
    };
} // CCData
