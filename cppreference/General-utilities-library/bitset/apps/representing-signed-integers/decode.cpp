#include "common.h"
#include <unordered_map>


int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0]
            << "<twos|ones|orig> <code>" << std::endl;
        return -1;
    }
    std::string mode{argv[1]};
    std::string binstr{argv[2]};

    //for (auto [m, f]: decoders) {
    //    std::cout << "{m="<< m << ", f" 
    //        << reinterpret_cast<const void *>(f) << "}\n";
    //}

    std::cout << "mode: [" << mode << "]\n";
    word n = decoders.at(mode)(bin(binstr));
    std::cout << "decode: " << binstr << std::endl;
    std::cout << "n: " << static_cast<int>(n) << std::endl;
    return 0;
}
