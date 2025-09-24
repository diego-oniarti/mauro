#include <vector>
#include <z3++.h>
#include "optimizer.h"
#include "common.h"

Optimizer::Optimizer(
        int asp_low, int asp_high,
        int sca_low, int sca_high,
        std::vector<int> &mis,
        std::vector<int> &spe) : c(), opt(c), misure(mis), spessori(spe), num_empty(c.int_val(0)), num_changes(c.int_val(0)) {
    z3::params par(c);
    par.set("priority", "lex");
    opt.set(par);

    // Crea il vettore interno e assegna i valori
    z3::expr spessori_expr = c.constant("A", c.array_sort(c.int_sort(), c.int_sort()));
    for (int i = 0; i < 16; ++i) {
        opt.add(z3::select(spessori_expr, i+1) == spessori[i]);
    }

    // Crea le scelte e mettile nel range [0, 16]
    for (int i = 0; i < 16; i++) {
        std::string name = "sce" + std::to_string(i+1);
        z3::expr v = c.int_const(name.c_str());
        opt.add(v >= 0 && v <= 16);
        scelte.push_back(v);
    }

    // Valori non nulli unici
    for (int i = 0; i < 16; i++) {
        for (int j = i+1; j < 16; j++) {
            opt.add(z3::implies(scelte[i] != 0, scelte[i] != scelte[j]));
        }
    }

    // Ranges for aspiration (asp) and discharge (sca)
    for (int i = 0; i < 8; i++) {
        z3::expr val = misure[i] - z3::select(spessori_expr, scelte[i]);
        opt.add(z3::implies(scelte[i] != 0, val >= asp_low && val <= asp_high));
    }
    for (int i = 8; i < 16; i++) {
        z3::expr val = misure[i] - z3::select(spessori_expr, scelte[i]);
        opt.add(z3::implies(scelte[i] != 0, val >= sca_low && val <= sca_high));
    }

    // num_empty = count of zeros in sce
    num_empty = c.int_val(0);
    for (auto &v : scelte) {
        num_empty = num_empty + z3::ite(v == 0, c.int_val(1), c.int_val(0));
    }

    // num_changes = count of sce[i] != i+1 (ignoring 0)
    num_changes = c.int_val(0);
    for (int i = 0; i < 16; ++i) {
        num_changes = num_changes + z3::ite((scelte[i] != 0 && scelte[i] != (i+1)), c.int_val(1), c.int_val(0));
    }

    // Lexicographic optimization: first num_empty, then num_changes
    opt.minimize(num_empty);
    opt.minimize(num_changes);
}

dispo Optimizer::solve() {
    opt.check();
    z3::model m = opt.get_model();

    dispo ret;
    for (int i=0; i<16; i++) {
        ret[i] = m.eval(scelte[i]).as_int64();
    }

    return ret;
}

bool Optimizer::reject(int s) {
    int a = opt.get_model().eval(scelte[s]);
    if (a==0) {
        return false;
    }
    opt.add(scelte[s] != a);
    return true;
}

void Optimizer::push() {
    opt.push();
}

void Optimizer::undo() {
    opt.pop();
}
