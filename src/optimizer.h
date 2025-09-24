#include <array>
#include <vector>
#include <z3++.h>
#include "common.h"

class Optimizer {
    private:
        z3::context c;
        z3::optimize opt;

        int asp_low, asp_high, sca_low, sca_high;
        std::vector<int> misure;
        std::vector<int> spessori;
        std::vector<z3::expr> scelte;

        z3::expr num_empty;
        z3::expr num_changes;

    public:
        Optimizer(int asp_low, int asp_high, int sca_low, int sca_high,
                std::vector<int> &mis, std::vector<int> &spe);

        dispo solve();

        bool reject(int s);

        void push();
        void undo();
};

