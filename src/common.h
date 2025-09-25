#include <array>
#ifndef COMMONS
#define COMMONS

#define dispo std::array<int, 16>

struct Data {
    int asp_low, asp_high, sca_low, sca_high;
    std::array<int, 8> asp_misure, asp_spessori, sca_misure, sca_spessori;
};

#endif
