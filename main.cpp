#include <iostream>
#include <vector>
#include "FL/Enumerations.H"
#include "FL/Fl_Widget.H"
#include "optimizer.h"
#include "common.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

std::vector<int> misure, spessori, sum;

void print_disp(dispo &d) {
    for (int i=0; i<d.size(); i++) {
        std::cout << (i+1) << ": " << d[i];
        if (d[i]!=0) {
            std::cout << " " << (sum[i] - spessori[d[i]-1]);
        }
        std::cout << "\n";
    }
}

void MyFunction(Fl_Widget *w, void* userdata) {
    std::cout << "CIAO\n";
}

int main(int argc, char** argv) {
    int asp_low = 15, asp_high = 21;
    int sca_low = 27, sca_high = 33;

    misure = {5, 5, 5, 5, 15, 35, 15, 15,
        25, 25, 20, 25, 25, 0, 25, 20};

    spessori = {178, 178, 180, 180, 183, 160, 180, 180,
        166, 168, 173, 165, 170, 188, 163, 161};

    for (int i=0; i<spessori.size(); i++) {
        sum.push_back(misure[i] + spessori[i]);
    }

    Optimizer opt(asp_low, asp_high, sca_low, sca_high, sum, spessori);

    std::vector<dispo> history;

    history.push_back(opt.solve());

    print_disp(history[0]);



    Fl_Window win(400, 200, "Mauro");
    Fl_Button but(10, 10, 100,25, "testing");
    but.callback(MyFunction);
    win.show();

    return Fl::run();
}
