#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <array>

// class SHA-256
class vrdn_SHA256 {
private:
    static constexpr std::array<uint32_t, 64> vrdn_magic = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, // start mix stuff
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, // rotate bits here
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, // sigma0 do thing
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, // keep it moving
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, // more rotate stuff
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, // middle chunk
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, // tweak bits
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, // almost half
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, // keep rotating
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, // round stuff
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, // last blocks
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, // wrap it up
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, // final rounds
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, // almost done
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, // keep hashin
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2  // end here
    };

    // rotate right funky
    static uint32_t vrdn_spin(uint32_t vrdn_val, uint32_t vrdn_amt) {
        return (vrdn_val >> vrdn_amt) | (vrdn_val << (32 - vrdn_amt));
    }

    // choose thing
    static uint32_t vrdn_pick(uint32_t vrdn_x, uint32_t vrdn_y, uint32_t vrdn_z) {
        return (vrdn_x & vrdn_y) ^ (~vrdn_x & vrdn_z);
    }

    // majority guy
    static uint32_t vrdn_vote(uint32_t vrdn_a, uint32_t vrdn_b, uint32_t vrdn_c) {
        return (vrdn_a & vrdn_b) ^ (vrdn_a & vrdn_c) ^ (vrdn_b & vrdn_c);
    }

    // big sum 0
    static uint32_t vrdn_bsum0(uint32_t vrdn_n) {
        return vrdn_spin(vrdn_n, 2) ^ vrdn_spin(vrdn_n, 13) ^ vrdn_spin(vrdn_n, 22);
    }

    // big sum 1
    static uint32_t vrdn_bsum1(uint32_t vrdn_n) {
        return vrdn_spin(vrdn_n, 6) ^ vrdn_spin(vrdn_n, 11) ^ vrdn_spin(vrdn_n, 25);
    }

    // small sum 0
    static uint32_t vrdn_ssum0(uint32_t vrdn_n) {
        return vrdn_spin(vrdn_n, 7) ^ vrdn_spin(vrdn_n, 18) ^ (vrdn_n >> 3);
    }

    // small sum 1
    static uint32_t vrdn_ssum1(uint32_t vrdn_n) {
        return vrdn_spin(vrdn_n, 17) ^ vrdn_spin(vrdn_n, 19) ^ (vrdn_n >> 10);
    }

public:
    static std::array<uint8_t, 32> vrdn_go(const std::string& vrdn_file) {
        std::ifstream vrdn_stream(vrdn_file, std::ios::binary);
        if (!vrdn_stream) throw std::runtime_error("file cannot read");

        uint32_t vrdn_h[8] = {
            0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
            0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
        };

        std::string vrdn_dat((std::istreambuf_iterator<char>(vrdn_stream)), std::istreambuf_iterator<char>());

        uint64_t vrdn_bits = vrdn_dat.size()*8; // bits length
        vrdn_dat.push_back(0x80); // append one

        while((vrdn_dat.size()%64)!=56) vrdn_dat.push_back(0x00); // pad zeros

        // length append
        for(int vrdn_i=7; vrdn_i>=0;--vrdn_i) vrdn_dat.push_back((vrdn_bits>>(vrdn_i*8)) & 0xFF);

        // process blocks
        for(size_t vrdn_i=0;vrdn_i<vrdn_dat.size();vrdn_i+=64){
            uint32_t vrdn_w[64];
            for(int vrdn_j=0;vrdn_j<16;++vrdn_j) // first 16
                vrdn_w[vrdn_j] = (vrdn_dat[vrdn_i+vrdn_j*4]<<24)|
                                 (vrdn_dat[vrdn_i+vrdn_j*4+1]<<16)|
                                 (vrdn_dat[vrdn_i+vrdn_j*4+2]<<8)|
                                 (vrdn_dat[vrdn_i+vrdn_j*4+3]);

            for(int vrdn_j=16;vrdn_j<64;++vrdn_j) // rest w extend
                vrdn_w[vrdn_j] = vrdn_ssum1(vrdn_w[vrdn_j-2])+
                                 vrdn_w[vrdn_j-7]+
                                 vrdn_ssum0(vrdn_w[vrdn_j-15])+
                                 vrdn_w[vrdn_j-16];

            uint32_t vrdn_a=vrdn_h[0],vrdn_b=vrdn_h[1],vrdn_c=vrdn_h[2],vrdn_d=vrdn_h[3];
            uint32_t vrdn_e=vrdn_h[4],vrdn_f=vrdn_h[5],vrdn_g=vrdn_h[6],vrdn_hh=vrdn_h[7];

            for(int vrdn_j=0;vrdn_j<64;++vrdn_j){ // main loop
                uint32_t vrdn_t1=vrdn_hh+vrdn_bsum1(vrdn_e)+vrdn_pick(vrdn_e,vrdn_f,vrdn_g)+vrdn_magic[vrdn_j]+vrdn_w[vrdn_j];
                uint32_t vrdn_t2=vrdn_bsum0(vrdn_a)+vrdn_vote(vrdn_a,vrdn_b,vrdn_c);
                vrdn_hh=vrdn_g; vrdn_g=vrdn_f; vrdn_f=vrdn_e; vrdn_e=vrdn_d+vrdn_t1;
                vrdn_d=vrdn_c; vrdn_c=vrdn_b; vrdn_b=vrdn_a; vrdn_a=vrdn_t1+vrdn_t2;
            }

            // add block to hash
            for(int vrdn_j=0;vrdn_j<8;++vrdn_j) vrdn_h[vrdn_j]+= (vrdn_j==0?vrdn_a: vrdn_j==1?vrdn_b: vrdn_j==2?vrdn_c: vrdn_j==3?vrdn_d: vrdn_j==4?vrdn_e: vrdn_j==5?vrdn_f: vrdn_j==6?vrdn_g:vrdn_hh);
        }

        std::array<uint8_t,32> vrdn_res;
        for(int vrdn_j=0;vrdn_j<8;++vrdn_j){
            vrdn_res[vrdn_j*4] = (vrdn_h[vrdn_j]>>24) & 0xFF;
            vrdn_res[vrdn_j*4+1] = (vrdn_h[vrdn_j]>>16) & 0xFF;
            vrdn_res[vrdn_j*4+2] = (vrdn_h[vrdn_j]>>8) & 0xFF;
            vrdn_res[vrdn_j*4+3] = vrdn_h[vrdn_j] & 0xFF;
        }
        return vrdn_res;
    }
};

int main(){
    // I have downloaded the file "The Book of Mark" from the given link and saved it as book_of_mark.html
    std::string vrdn_file="book_of_mark.html";

    try{
        std::array<uint8_t,32> vrdn_out=vrdn_SHA256::vrdn_go(vrdn_file); // get hash
        std::cout<<"SHA-256 of The Book of Mark: ";
        for(auto vrdn_x:vrdn_out) std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)vrdn_x; // print hash
        std::cout<<"\n";
    }catch(std::exception &vrdn_e){
        std::cerr<<"Error: "<<vrdn_e.what()<<"\n";
        return 1;
    }

    return 0;
}
