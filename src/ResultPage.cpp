#include "ResultPage.h"
#include "FL/Fl_Button.H"
#include "FL/Fl_Native_File_Chooser.H"
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pack.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <array>
#include <string>

struct Params {
    int valvola;
    ResultPage* p;
};

ResultPage::ResultPage() {
    win = new Fl_Window(389, 329, "Soluzione");
    op = NULL;

    Fl_Pack *aspirazione_output = new Fl_Pack(20,40,159,0);
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
        aspirazione_rows[i]->scelta.copy_label(std::to_string(i+1).c_str());
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

    Fl_Pack *scarico_output = new Fl_Pack(210,40,159,0);
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
        scarico_rows[i]->scelta.copy_label(std::to_string(i+9).c_str());
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

    save_button = new Fl_Button(289,289,80,30,"@filesave   Salva");
    save_button->callback([](Fl_Widget *w, void *v){ ((ResultPage*)v)->save(); }, this);

    quit_button = new Fl_Button(199,289,80,30,"@returnarrow   Esci");
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
    Fl_Native_File_Chooser chooser;
    chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    chooser.filter("Text\t*.txt\nAll Files\t*");
    chooser.preset_file("Spessori.txt");

    if (chooser.show()==0) {
        std::string filename = chooser.filename();
        if (filename.find('.') == std::string::npos) {
            filename += ".txt";
        }

        FILE *fp = fopen(filename.c_str(), "w");
        if (fp) {
            dispo dis = op->get_disp();
            for (int i=0; i<8; i++) {
                fprintf(fp, "%d: %d (%d)\n", i+1, dis[i], data.asp_spessori[i]);
            }
            for (int i=0; i<8; i++) {
                fprintf(fp, "%d: %d (%d)\n", i+9, dis[i+8], data.sca_spessori[i]);
            }

            fclose(fp);
        } else {
            fl_alert("Could not save file!");
        }
    }else{
        fl_alert("File non salvato");
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
            ra->scelta.value("Compra");
            ra->misura.value(data.asp_misure[i]+data.asp_spessori[i]-asp_mid);
            ra->reject.deactivate();
        }else{
            ra->scelta.value(disp[i]+1);
            int m = data.asp_misure[i]+data.asp_spessori[i]-spessori[disp[i]];
            ra->misura.value(m);

            if (m==data.asp_low || m==data.asp_high) {
                ra->misura.color(92);
            }else{
                ra->misura.color(7);
            }
        }

        Row* rs = scarico_rows[i];
        if (disp[i+8]==-1) {
            rs->scelta.value("Compra");
            rs->misura.value(data.sca_misure[i]+data.sca_spessori[i]-sca_mid);
            rs->reject.deactivate();
        }else{
            rs->scelta.value(disp[i+8]+1);
            int m = data.sca_misure[i]+data.sca_spessori[i]-spessori[disp[i+8]];
            rs->misura.value(m);

            if (m==data.sca_low || m==data.sca_high) {
                rs->misura.color(92);
            }else{
                rs->misura.color(7);
            }
        }
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
