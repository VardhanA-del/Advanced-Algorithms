#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <array>

class vrdn_SHA256 {
private:
    static constexpr std::array<uint32_t, 64> k = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    static uint32_t rotr(uint32_t v, uint32_t n) {
        return (v >> n) | (v << (32 - n));
    }

    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g) {
        return (e & f) ^ (~e & g);
    }

    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c) {
        return (a & b) ^ (a & c) ^ (b & c);
    }

    static uint32_t big_sigma0(uint32_t a) {
        return rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
    }

    static uint32_t big_sigma1(uint32_t e) {
        return rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
    }

    static uint32_t small_sigma0(uint32_t x) {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }

    static uint32_t small_sigma1(uint32_t x) {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }

public:
    static std::array<uint8_t, 32> compute(const std::string& vrdn_filename) {
        std::ifstream vrdn_file(vrdn_filename, std::ios::binary);
        if (!vrdn_file) {
            throw std::runtime_error("Cannot open file");
        }

        uint32_t vrdn_h[8] = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
        };

        // Read entire file into a string (simple, small files)
        std::string vrdn_data((std::istreambuf_iterator<char>(vrdn_file)),
                              std::istreambuf_iterator<char>());

        uint64_t vrdn_bit_len = vrdn_data.size() * 8;
        vrdn_data.push_back(0x80); // append '1' bit

        while ((vrdn_data.size() % 64) != 56) {
            vrdn_data.push_back(0x00); // pad with zeros
        }

        // Append 64-bit length in big-endian
        for (int i = 7; i >= 0; --i) {
            vrdn_data.push_back((vrdn_bit_len >> (i * 8)) & 0xFF);
        }

        // Process blocks
        for (size_t i = 0; i < vrdn_data.size(); i += 64) {
            uint32_t vrdn_w[64];
            for (int t = 0; t < 16; ++t) {
                vrdn_w[t] = (vrdn_data[i + t * 4] << 24) |
                            (vrdn_data[i + t * 4 + 1] << 16) |
                            (vrdn_data[i + t * 4 + 2] << 8) |
                            (vrdn_data[i + t * 4 + 3]);
            }
            for (int t = 16; t < 64; ++t) {
                vrdn_w[t] = small_sigma1(vrdn_w[t - 2]) + vrdn_w[t - 7] +
                             small_sigma0(vrdn_w[t - 15]) + vrdn_w[t - 16];
            }

            uint32_t vrdn_a = vrdn_h[0];
            uint32_t vrdn_b = vrdn_h[1];
            uint32_t vrdn_c = vrdn_h[2];
            uint32_t vrdn_d = vrdn_h[3];
            uint32_t vrdn_e = vrdn_h[4];
            uint32_t vrdn_f = vrdn_h[5];
            uint32_t vrdn_g = vrdn_h[6];
            uint32_t vrdn_hh = vrdn_h[7];

            for (int t = 0; t < 64; ++t) {
                uint32_t vrdn_t1 = vrdn_hh + big_sigma1(vrdn_e) + choose(vrdn_e, vrdn_f, vrdn_g) + k[t] + vrdn_w[t];
                uint32_t vrdn_t2 = big_sigma0(vrdn_a) + majority(vrdn_a, vrdn_b, vrdn_c);
                vrdn_hh = vrdn_g;
                vrdn_g = vrdn_f;
                vrdn_f = vrdn_e;
                vrdn_e = vrdn_d + vrdn_t1;
                vrdn_d = vrdn_c;
                vrdn_c = vrdn_b;
                vrdn_b = vrdn_a;
                vrdn_a = vrdn_t1 + vrdn_t2;
            }

            vrdn_h[0] += vrdn_a;
            vrdn_h[1] += vrdn_b;
            vrdn_h[2] += vrdn_c;
            vrdn_h[3] += vrdn_d;
            vrdn_h[4] += vrdn_e;
            vrdn_h[5] += vrdn_f;
            vrdn_h[6] += vrdn_g;
            vrdn_h[7] += vrdn_hh;
        }

        std::array<uint8_t, 32> vrdn_hash;
        for (int i = 0; i < 8; ++i) {
            vrdn_hash[i * 4] = (vrdn_h[i] >> 24) & 0xFF;
            vrdn_hash[i * 4 + 1] = (vrdn_h[i] >> 16) & 0xFF;
            vrdn_hash[i * 4 + 2] = (vrdn_h[i] >> 8) & 0xFF;
            vrdn_hash[i * 4 + 3] = vrdn_h[i] & 0xFF;
        }
        return vrdn_hash;
    }
};

int main() {
    // I have downloaded the "book_of_mark.html" file manually from: https://quod.lib.umich.edu/cgi/r/rsv/rsv-idx?type=DIV1&byte=4697892 and computes its SHA-256 hash.
    std::string vrdn_file_name = "book_of_mark.html";

    try {
        std::array<uint8_t, 32> vrdn_result = vrdn_SHA256::compute(vrdn_file_name);
        std::cout << "SHA-256: ";
        for (auto byte : vrdn_result) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        }
        std::cout << "\n";
    } catch (std::exception &vrdn_e) {
        std::cerr << "Error: " << vrdn_e.what() << "\n";
        return 1;
    }

    return 0;
}
