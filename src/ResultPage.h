#include "FL/Fl_Button.H"
#include "FL/Fl_Window.H"
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Widget.H>
#include <array>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Output.H>

#include "common.h"
#include "Optimizer.h"

struct Row {
    Fl_Output scelta;
    Fl_Output misura;
    Fl_Button reject;
};

class ResultPage {
    private:
        Data data;
        Optimizer* op;
        Fl_Window* win;

        Fl_Button *undo_button, *redo_button;
        Fl_Button *save_button, *quit_button;
        std::array<Row*, 8> scarico_rows;
        std::array<Row*, 8> aspirazione_rows;

        static void undo_callback(Fl_Widget *w, void *v);
        static void redo_callback(Fl_Widget *w, void *v);

        void display();
    public:
        ResultPage();

        void quit();
        void undo();
        void redo();
        void save();
        void exclude(int);

        void show();
        void set_data(Data);
};
