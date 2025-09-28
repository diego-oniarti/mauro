#include "common.h"
#include <array>
#include <set>
#include <utility>
#include <vector>

struct Step {
    int valvola;
    int misura;
    dispo disposizione;
};

class Optimizer {
private:
    Data data;
    std::vector<Step> history;
    std::array<std::set<int>, 16> exclusions;
    int pos;
    bool is_4_cil;
public:
    Optimizer(Data d);
    dispo exclude(int);
    dispo get_disp();
    dispo undo();
    dispo redo();
    bool can_undo();
    bool can_redo();
    dispo solve();
};
