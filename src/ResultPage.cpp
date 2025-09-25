#include "ResultPage.h"
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include <string>

ResultPage::ResultPage(Data d) :
    asp_low(d.asp_low), asp_high(d.asp_high),
    sca_low(d.sca_low), sca_high(d.sca_high),
    asp_misure(d.asp_misure), asp_spessori(d.asp_spessori),
    sca_misure(d.sca_misure), sca_spessori(d.sca_spessori) {
        win = new Fl_Window(100,100, "CIAO");

        Fl_Box *tmp = new Fl_Box(10,10,80,25);
        tmp->copy_label(std::to_string(asp_low).c_str());
        tmp->align(FL_ALIGN_CENTER);

        for (int i=0; i<8; i++) {
            scarico_rows[i] = new Row {
                Fl_Text_Display(10,10,10,10),
                Fl_Text_Display(10,10,10,10),
                Fl_Button(10,10,10,10,"A")
            };
        }

        win->set_modal();
        win->end();
        win->show();
    }

ResultPage::~ResultPage() {
    std::cout << "DELETING" << std::endl;
    delete win;
}
