#include <FL/Fl.H>
#include <array>
#include <iostream>
#include <vector>
#include "common.h"
#include "InitialPage.h"

std::vector<int> misure, spessori, sum;

void print_disp(dispo &d) {
    for (int i=0; i<d.size(); i++) {
        std::cout << (i+1) << ": " << (d[i]+1);
        if (d[i]!=-1) {
            std::cout << " " << (sum[i] - spessori[d[i]]);
        }
        std::cout << "\n";
    }
}

int main(int argc, char** argv) {
    // int asp_low = 15, asp_high = 21;
    // int sca_low = 27, sca_high = 33;

    // misure = {5, 5, 5, 5, 15, 35, 15, 15,
    //     25, 25, 20, 25, 25, 0, 25, 20};

    // spessori = {178, 178, 180, 180, 183, 160, 180, 180,
    //     166, 168, 173, 165, 170, 188, 163, 161};

    // for (int i=0; i<spessori.size(); i++) {
    //     sum.push_back(misure[i] + spessori[i]);
    // }

    // Optimizer o(Data{asp_low,asp_high,sca_low,sca_high,
    //             std::array<int, 8>{5,   5,   5,   5,   15,  35,  15,  15  },
    //             std::array<int, 8>{178, 178, 180, 180, 183, 160, 180, 180 },
    //             std::array<int, 8>{25,  25,  20,  25,  25,  0,   25,  20  },
    //             std::array<int, 8>{166, 168, 173, 165, 170, 188, 163, 161 }
    //             });

    // dispo a = o.get_disp();
    // print_disp(a);

    InitialPage page;

    return Fl::run();
}
