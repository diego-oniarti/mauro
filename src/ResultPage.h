#include "FL/Fl_Button.H"
#include "FL/Fl_Window.H"
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Widget.H>
#include <array>
#include <FL/Fl_Int_Input.H>

#include "common.h"

struct Row {
    Fl_Text_Display scelta;
    Fl_Text_Display misura;
    Fl_Button reject;
};

class ResultPage {
    private:
        int asp_low, asp_high, sca_low, sca_high;
        std::array<int, 8> asp_misure, asp_spessori, sca_misure, sca_spessori;
        Fl_Window* win;

        Fl_Button *undo_button, *save_button;
        std::array<Row*, 8> scarico_rows;
        // std::array<Row, 8> aspirazione_rows;
    public:
        ResultPage(Data d);
        ~ResultPage();
};
