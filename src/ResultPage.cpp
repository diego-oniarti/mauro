#include "ResultPage.h"
#include "FL/Fl_Button.H"
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pack.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <algorithm>
#include <array>
#include <cstdio>
#include <string>

struct Params {
    int valvola;
    ResultPage* p;
};

ResultPage::ResultPage() {
    win = new Fl_Window(399, 329, "Disposizione");
    op = NULL;

    Fl_Pack *aspirazione_output = new Fl_Pack(30,40,159,0);
    Fl_Box *aspirazione_label = new Fl_Box(0,0,aspirazione_output->w(),25,"Aspirazione");
    aspirazione_label->align(FL_ALIGN_CENTER);
    Fl_Pack *asp_column = new Fl_Pack(0,0,aspirazione_output->w(),0);
    asp_column->spacing(2);
    for (int i=0; i<8; i++) {
        Fl_Pack *tmp = new Fl_Pack(0,0,0,25);
        tmp->type(Fl_Pack::HORIZONTAL);
        tmp->spacing(2);
        aspirazione_rows[i] = new Row {
            Fl_Output(0, 0, 80, 25),
                Fl_Output(0, 0, 50, 25),
                Fl_Button(0, 0, 25, 25, "@refresh"),
        };
        aspirazione_rows[i]->scelta.align(FL_ALIGN_LEFT);
        aspirazione_rows[i]->scelta.copy_label((std::to_string(i+1)+"A").c_str());
        aspirazione_rows[i]->reject.callback([](Fl_Widget* w, void* v){
                int valvola = ((Params*)v)->valvola;
                ResultPage* page = ((Params*)v)->p;
                page->op->exclude(valvola);
                page->display();
                }, new Params{i, this});
        tmp->end();
    }
    asp_column->end();
    aspirazione_output->end();

    Fl_Pack *scarico_output = new Fl_Pack(220,40,159,0);
    Fl_Box *scarico_label = new Fl_Box(0,0,scarico_output->w(),25,"Scarico");
    scarico_label->align(FL_ALIGN_CENTER);
    Fl_Pack *sca_column = new Fl_Pack(0,0,scarico_output->w(),0);
    sca_column->spacing(2);
    for (int i=0; i<8; i++) {
        Fl_Pack *tmp = new Fl_Pack(0,0,0,25);
        tmp->type(Fl_Pack::HORIZONTAL);
        tmp->spacing(2);
        scarico_rows[i] = new Row {
            Fl_Output(0, 0, 80, 25),
                Fl_Output(0, 0, 50, 25),
                Fl_Button(0, 0, 25, 25, "@refresh"),
        };
        scarico_rows[i]->scelta.align(FL_ALIGN_LEFT);
        scarico_rows[i]->scelta.copy_label((std::to_string(i+1)+"S").c_str());
        scarico_rows[i]->reject.callback([](Fl_Widget* w, void* v){
                int valvola = ((Params*)v)->valvola;
                ResultPage* page = ((Params*)v)->p;
                page->op->exclude(valvola);
                page->display();
                }, new Params{i+8, this});
        tmp->end();
    }
    sca_column->end();
    scarico_output->end();

    undo_button = new Fl_Button(10,10,80,25,"@undo  Annulla");
    undo_button->callback([](Fl_Widget *w, void *v){ ((ResultPage*)v)->undo(); }, this);

    redo_button = new Fl_Button(100,10,80,25,"@redo   Ripeti");
    redo_button->callback([](Fl_Widget *w, void *v){ ((ResultPage*)v)->redo(); }, this);

    save_button = new Fl_Button(299,289,80,30,"@filesave   Salva");
    save_button->callback([](Fl_Widget *w, void *v){ ((ResultPage*)v)->save(); }, this);

    quit_button = new Fl_Button(209,289,80,30,"@returnarrow   Esci");
    quit_button->callback([](Fl_Widget *w, void *v){ ((ResultPage*)v)->quit(); }, this);

    win->end();
    win->set_modal();
}

void ResultPage::undo() {
    op->undo();
    display();
}
void ResultPage::redo() {
    op->redo();
    display();
}

void ResultPage::save() {
    std::string filename = pick_filename("Spessori.txt");

    if (filename.length()==0) return;

    int asp_mid = (data.asp_low+data.asp_high)/2;
    int sca_mid = (data.sca_low+data.sca_high)/2;

    std::array<int, 16> all_spessori;
    std::copy(data.asp_spessori.begin(), data.asp_spessori.end(), all_spessori.begin());
    std::copy(data.sca_spessori.begin(), data.sca_spessori.end(), all_spessori.begin()+8);

    FILE *fp;
    fopen_s(&fp, filename.c_str(), "w");
    int limit = is_4_cilindri(data) ? 4 : 8;
    if (fp) {
        dispo dis = op->get_disp();
        for (int i=0; i<limit; i++) {
            std::string indice = num_to_label(i);
            std::string scelta = (dis[i]==-1)?"Rimpiazzare":num_to_label(dis[i]);
            int third = (dis[i]!=-1) ? all_spessori[dis[i]] : data.asp_misure[i]+data.asp_spessori[i]-asp_mid ;
            fprintf(fp, "%s: %s (%d)\n", indice.c_str(), scelta.c_str(), third);
        }
        fprintf(fp, "\n");
        for (int i=8; i<8+limit; i++) {
            std::string indice = num_to_label(i);
            std::string scelta = (dis[i]==-1)?"Rimpiazzare":num_to_label(dis[i]);
            int third = (dis[i]!=-1) ? all_spessori[dis[i]] : data.asp_misure[i-8]+data.asp_spessori[i-8]-asp_mid ;
            fprintf(fp, "%s: %s (%d)\n", indice.c_str(), scelta.c_str(), third);
        }

        fclose(fp);
    }
}

void ResultPage::quit() {
    if (fl_choice_n("Chiudere questa finestra?", "Sì", "No", NULL)==0) {
        win->hide();
    }
}

void ResultPage::show() {
    win->show();
}

void ResultPage::set_data(Data d) {
    // d = Data{15,21,27,33,
    //     std::array<int, 8>{5,   5,   5,   5,   15,  35,  15,  15  },
    //     std::array<int, 8>{178, 178, 180, 180, 183, 160, 180, 180 },
    //     std::array<int, 8>{25,  25,  20,  25,  25,  0,   25,  20  },
    //     std::array<int, 8>{166, 168, 173, 165, 170, 188, 163, 161 }
    // };
    if (op!=NULL) delete op;
    data = d;
    op = new Optimizer(d);

    if (is_4_cilindri(data)) {
        for (int i=4; i<8; i++) {
            aspirazione_rows[i]->misura.hide();
            aspirazione_rows[i]->scelta.hide();
            aspirazione_rows[i]->reject.hide();
            scarico_rows[i]->misura.hide();
            scarico_rows[i]->scelta.hide();
            scarico_rows[i]->reject.hide();
        }
    }else{
        for (int i=4; i<8; i++) {
            aspirazione_rows[i]->misura.show();
            aspirazione_rows[i]->scelta.show();
            aspirazione_rows[i]->reject.show();
            scarico_rows[i]->misura.show();
            scarico_rows[i]->scelta.show();
            scarico_rows[i]->reject.show();
        }
    }

    display();
}

void ResultPage::display() {
    dispo disp = op->get_disp();

    std::string (*itoa)(int) = [](int a) { return std::to_string(a); };

    int asp_mid = (data.asp_low+data.asp_high)/2;
    int sca_mid = (data.sca_low+data.sca_high)/2;

    std::array<int, 16> spessori;
    for (int i=0; i<8; i++) {
        spessori[i] = data.asp_spessori[i];
        spessori[i+8] = data.sca_spessori[i];
    }

    for (int i=0; i<8; i++) {
        Row* ra = aspirazione_rows[i];
        if (disp[i]==-1) {
            ra->scelta.value("Sostituire");
            ra->misura.value(std::to_string(data.asp_misure[i]+data.asp_spessori[i]-asp_mid).c_str());
            ra->reject.deactivate();
            // Sostituire
            ra->misura.color(9);
            ra->misura.textcolor(0);
        }else{
            ra->scelta.value(num_to_label(disp[i]).c_str());
            int m = data.asp_misure[i]+data.asp_spessori[i]-spessori[disp[i]];
            ra->misura.value(std::to_string(m).c_str());

            if (m==data.asp_low || m==data.asp_high) {
                // Limite
                ra->misura.color(0);
                ra->misura.textcolor(7);
            }else{
                // Normale
                ra->misura.color(7);
                ra->misura.textcolor(0);
            }
            ra->reject.activate();
        }

        Row* rs = scarico_rows[i];
        if (disp[i+8]==-1) {
            rs->scelta.value("Sostituire");
            rs->misura.value(std::to_string(data.sca_misure[i]+data.sca_spessori[i]-sca_mid).c_str());
            rs->reject.deactivate();
            rs->misura.color(9);
            rs->misura.textcolor(0);
        }else{
            rs->scelta.value(num_to_label(disp[i+8]).c_str());
            int m = data.sca_misure[i]+data.sca_spessori[i]-spessori[disp[i+8]];
            rs->misura.value(std::to_string(m).c_str());

            if (m==data.sca_low || m==data.sca_high) {
                rs->misura.color(0);
                rs->misura.textcolor(7);
            }else{
                rs->misura.color(7);
                rs->misura.textcolor(0);
            }
            rs->reject.activate();
        }

        ra->misura.damage(Fl_Damage::FL_DAMAGE_ALL);
        rs->misura.damage(Fl_Damage::FL_DAMAGE_ALL);
    }

    if (op->can_undo()) {
        undo_button->activate();
    }else{
        undo_button->deactivate();
    }
    if (op->can_redo()) {
        redo_button->activate();
    }else{
        redo_button->deactivate();
    }
}
