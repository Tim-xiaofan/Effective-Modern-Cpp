#include "common.h"

#include <iomanip>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] 
            << "bin1 bin2" << std::endl;
        return -1;
    }

    bin a(argv[1]);
    bin b(argv[2]);

    auto [sum, carry] = fake_add(a, b);
    std::cout << "a  : " << a << "\n";
    std::cout << "b  : " << b << "\n";
    std::cout << "sum: " << sum << "\n";

    std::cout << "carry: " << carry << std::endl;
    std::cout << "overflow: " << std::boolalpha 
        << detect_overflow(a[SIGN_INDEX], b[SIGN_INDEX], sum[SIGN_INDEX]) << std::endl;
}
