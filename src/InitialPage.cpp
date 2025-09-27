#include <FL/Fl.H>
#include <algorithm>
#include <array>
#include <cstdio>
#include <string>
#include "FL/Enumerations.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Image.H"
#include "FL/Fl_Widget.H"
#include "FL/Fl_Window.H"
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Pack.H>
#include <FL/fl_ask.H>
#include <Fl/Fl_File_Chooser.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_XPM_Image.H>

#include "m.xpm"
#include "InitialPage.h"
#include "common.h"

std::array<int, 8> inputs_to_ints(std::array<Fl_Int_Input*, 8> inps) {
    std::array<int, 8> ret;
    std::transform(inps.begin(), inps.end(), ret.begin(), [](Fl_Int_Input* inp){ return atoi(inp->value()); });
    return ret;
}

void salva(Fl_Widget* w, void* v) {
    InitialPage *p = (InitialPage*)v;
    Data d = p->getData();

    Fl_Native_File_Chooser chooser;
    chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    chooser.filter("Text\t*.txt\nAll Files\t*");
    chooser.preset_file("Dati.txt");

    if (chooser.show()==0) {
        std::string filename = chooser.filename();
        if (filename.find('.') == std::string::npos) {
            filename += ".txt";
        }

        FILE *fp = fopen(filename.c_str(), "w");
        if (fp) {
            fprintf(fp, "%d %d %d %d\n", d.asp_low, d.asp_high, d.sca_low, d.sca_high);

            for (int v: d.asp_spessori) {
                fprintf(fp, "%d ", v);
            }
            fprintf(fp, "\n");

            for (int v: d.asp_misure) {
                fprintf(fp, "%d ", v);
            }
            fprintf(fp, "\n");

            for (int v: d.sca_spessori) {
                fprintf(fp, "%d ", v);
            }
            fprintf(fp, "\n");

            for (int v: d.sca_misure) {
                fprintf(fp, "%d ", v);
            }
            fprintf(fp, "\n");

            fclose(fp);
        } else {
            fl_alert("File non salvato");
        }
    }
}

void chiudi(Fl_Widget* w, void* v) {
    switch (fl_choice("Uscire?", "Salva @filesave", "Sì", "No")) {
        case 0:
            salva(w,v);
            exit(0);
            break;
        case 1:
            exit(0);
            break;
        case 2:
            break;
    }
}

void InitialPage::confirm_callback(Fl_Widget* w, void* v){
    ((InitialPage*)v)->conferma();
}

void InitialPage::carica() {
    Fl_Native_File_Chooser chooser;
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (chooser.show()==0) {
        FILE *fp = fopen(chooser.filename(), "r");
        if (fp) {
            int v;
            fscanf_s(fp, "%d", &v);
            asp_low->value(v);
            fscanf_s(fp, "%d", &v);
            asp_high->value(v);
            fscanf_s(fp, "%d", &v);
            sca_low->value(v);
            fscanf_s(fp, "%d", &v);
            sca_high->value(v);

            for (int i=0; i<8; i++) {
                fscanf_s(fp, "%d", &v);
                asp_spessori[i]->value(v);
            }

            for (int i=0; i<8; i++) {
                fscanf_s(fp, "%d", &v);
                asp_misure[i]->value(v);
            }

            for (int i=0; i<8; i++) {
                fscanf_s(fp, "%d", &v);
                sca_spessori[i]->value(v);
            }

            for (int i=0; i<8; i++) {
                fscanf_s(fp, "%d", &v);
                sca_misure[i]->value(v);
            }

            fclose(fp);
        }
    }
}

Data InitialPage::getData() {
    return Data{
        atoi(asp_low->value()),
            atoi(asp_high->value()),
            atoi(sca_low->value()),
            atoi(sca_high->value()),
            inputs_to_ints(asp_misure),
            inputs_to_ints(asp_spessori),
            inputs_to_ints(sca_misure),
            inputs_to_ints(sca_spessori),
    };
}

void InitialPage::conferma() {
    result_page.set_data(getData());
    result_page.show();
}

InitialPage::InitialPage() {
    win = new Fl_Window(310, 360, "Mauro");

    Fl_Pixmap *pix = new Fl_Pixmap(m2);
    Fl_RGB_Image *rgb = new Fl_RGB_Image(pix, 0);
    win->icon(rgb);

    Fl_Pack *inputs = new Fl_Pack(20,5,0,350);
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
        sca_spessori[i] = new Fl_Int_Input(0,0,sca_spe->w(),25);
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

    confirm_button = new Fl_Button(200,320,90,30, "Conferma @UpArrow");
    confirm_button->callback(confirm_callback, this);

    save_button = new Fl_Button(10,320,80,30, "@filesave  Salva");
    Data d = getData();
    save_button->callback(salva, this);

    load_button = new Fl_Button(90,320,80,30, "@fileopen  Carica");
    load_button->callback([](Fl_Widget* w, void* v){((InitialPage*)v)->carica();}, this);

    // win->resizable(win);
    win->callback(chiudi, this);
    win->end();
    win->show();
}

InitialPage::~InitialPage() {
    delete win;
}
