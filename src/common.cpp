#include <filesystem>
#include "common.h"
#include "FL/Fl_Native_File_Chooser.H"

std::string num_to_label(int n) {
    return std::to_string((n&7)+1)+((n<8)?"A":"S");
}

std::string pick_filename(const char* default_name) {
    std::string filename = default_name;
    bool name_chosen = false;

    do {
        Fl_Native_File_Chooser chooser;
        chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
        chooser.filter("Text\t*.txt\nAll Files\t*");
        chooser.preset_file(filename.c_str());

        if (chooser.show()==0) {
            filename = chooser.filename();

            if (std::filesystem::exists(filename)) {
                if (fl_choice("File esistente. Sovrascrivere?", "No", "Sì", NULL) == 1) {
                    name_chosen = true;
                }
            }else{
                if (filename.find('.') == std::string::npos) {
                    filename += ".txt";
                }
            }

        }else{
            return "";
        }
    } while (!name_chosen);

    return filename;
}

bool is_4_cilindri(Data& d) {
    for (int i=4; i<8; i++) {
        if (
                d.asp_spessori[i]!=0
                || d.sca_spessori[i]!=0
                || d.asp_misure[i]!=0
                || d.sca_misure[i]!=0
           ) return false;
    }
    return true;
}
