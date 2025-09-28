#include <array>
#include <string>
#ifndef COMMONS
#define COMMONS

#define dispo std::array<int, 16>

struct Data {
    int asp_low, asp_high, sca_low, sca_high;
    std::array<int, 8> asp_misure, asp_spessori, sca_misure, sca_spessori;
};

std::string num_to_label(int);
std::string pick_filename(const char*);
bool is_4_cilindri(Data&);

#endif
