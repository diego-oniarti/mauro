#include "FL/Fl_Button.H"
#include "FL/Fl_Window.H"
#include <FL/Fl_Widget.H>
#include <array>
#include <FL/Fl_Int_Input.H>

#include "ResultPage.h"

class InitialPage {
    private:
        Fl_Window* win;
        Fl_Int_Input *asp_low, *asp_high, *sca_low, *sca_high;
        std::array<Fl_Int_Input*, 8> asp_misure, asp_spessori, sca_misure, sca_spessori;
        Fl_Button *confirm_button;

        ResultPage *result_page;

        static void confirm_callback(Fl_Widget*, void*);
    public:
        InitialPage();
        ~InitialPage();
        void conferma();
};
