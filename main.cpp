#include <iostream>
#include <string>
#include <vector>
#include "FL/Enumerations.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Widget.H"
#include "FL/Fl_Window.H"
#include "common.h"
#include <array>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Pack.H>

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

class InitialPage {
    private:
        Fl_Window* win;
        Fl_Int_Input *asp_low, *asp_high, *sca_low, *sca_high;
        std::array<Fl_Int_Input*, 8> asp_misure, asp_spessori, sca_misure, sca_spessori;
        Fl_Button *confirm_button;
    public:
        InitialPage() {
            win = new Fl_Window(310, 360, "Mauro");

            Fl_Pack *inputs = new Fl_Pack(20,5,0,300);
            inputs->type(Fl_Pack::HORIZONTAL);
            inputs->spacing(30);
            // Aspirazione
            Fl_Pack *asp = new Fl_Pack(0,0,120,0);
            Fl_Box *asp_title = new Fl_Box(0,0,asp->w(),25, "Aspirazione");
            asp_title->align(FL_ALIGN_CENTER);
            Fl_Group *asp_range_group = new Fl_Group(0,0,asp->w(),40);
            asp_low = new Fl_Int_Input(0,15,asp->w()/2,25, "min");
            asp_low->align(FL_ALIGN_TOP);
            asp_high = new Fl_Int_Input(asp->w()/2,15,asp->w()/2,25, "max");
            asp_high->align(FL_ALIGN_TOP);
            asp_range_group->end();
            Fl_Pack *asp_inputs = new Fl_Pack(0,0,0,240);
            asp_inputs->type(Fl_Pack::HORIZONTAL);
            Fl_Pack *asp_spe_titled = new Fl_Pack(0,0,asp->w()/2,0);
            Fl_Box *asp_spe_title = new Fl_Box(0,0,asp_spe_titled->w(),25, "Spessori");
            asp_spe_title->align(FL_ALIGN_CENTER);
            Fl_Pack *asp_spe = new Fl_Pack(0,0, asp_spe_titled->w(), 0);
            asp_spe->spacing(2);
            for (int i=0; i<8; i++) {
                asp_spessori[i] = new Fl_Int_Input(00,0,asp_spe->w(),25);
                asp_spessori[i]->copy_label(std::to_string(i+1).c_str());
            }
            asp_spe->end();
            asp_spe_titled->end();
            Fl_Pack *asp_mis_titled = new Fl_Pack(0,0,asp->w()/2,0);
            Fl_Box *asp_mis_title = new Fl_Box(0,0,asp_mis_titled->w(),25, "Misure");
            asp_mis_title->align(FL_ALIGN_CENTER);
            Fl_Pack *asp_mis = new Fl_Pack(50, 100, asp_mis_titled->w(), 0);
            asp_mis->spacing(2);
            for (int i=0; i<8; i++) {
                asp_misure[i] = new Fl_Int_Input(00,0,asp_mis->w(),25);
            }
            asp_mis->end();
            asp_mis_titled->end();
            asp_inputs->end();
            asp->end();

            // Scarico
            Fl_Pack *sca = new Fl_Pack(0,0,120,0);
            Fl_Box *sca_title = new Fl_Box(0,0,sca->w(),25, "Scarico");
            sca_title->align(FL_ALIGN_CENTER);
            Fl_Group *sca_range_group = new Fl_Group(0,0,sca->w(),40);
            sca_low = new Fl_Int_Input(0,15,sca->w()/2,25, "min");
            sca_low->align(FL_ALIGN_TOP);
            sca_high = new Fl_Int_Input(sca->w()/2,15,sca->w()/2,25, "max");
            sca_high->align(FL_ALIGN_TOP);
            sca_range_group->end();
            Fl_Pack *sca_inputs = new Fl_Pack(0,0,0,240);
            sca_inputs->type(Fl_Pack::HORIZONTAL);
            Fl_Pack *sca_spe_titled = new Fl_Pack(0,0,sca->w()/2,0);
            Fl_Box *sca_spe_title = new Fl_Box(0,0,sca_spe_titled->w(),25, "Spessori");
            sca_spe_title->align(FL_ALIGN_CENTER);
            Fl_Pack *sca_spe = new Fl_Pack(0,0, sca_spe_titled->w(), 0);
            sca_spe->spacing(2);
            for (int i=0; i<8; i++) {
                sca_spessori[i] = new Fl_Int_Input(00,0,sca_spe->w(),25);
                sca_spessori[i]->copy_label(std::to_string(i+9).c_str());
            }
            sca_spe->end();
            sca_spe_titled->end();
            Fl_Pack *sca_mis_titled = new Fl_Pack(0,0,sca->w()/2,0);
            Fl_Box *sca_mis_title = new Fl_Box(0,0,sca_mis_titled->w(),25, "Misure");
            sca_mis_title->align(FL_ALIGN_CENTER);
            Fl_Pack *sca_mis = new Fl_Pack(50, 100, sca_mis_titled->w(), 0);
            sca_mis->spacing(2);
            for (int i=0; i<8; i++) {
                sca_misure[i] = new Fl_Int_Input(00,0,sca_mis->w(),25);
            }
            sca_mis->end();
            sca_mis_titled->end();
            sca_inputs->end();
            sca->end();

            inputs->end();

            confirm_button = new Fl_Button(200,320,90,30, "Conferma");

            win->resizable(inputs);
            win->end();
            win->show();
        }
        ~InitialPage() {
            delete win;
        }
};

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


    InitialPage page;

    return Fl::run();
}
