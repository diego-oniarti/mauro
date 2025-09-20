#include <z3++.h>
#include <iostream>
#include <vector>

int main() {
    z3::context c;
    z3::optimize opt(c);
    z3::params par(c);
    par.set("priority", "lex");
    opt.set(par);

    int asp_low = 15, asp_high = 21;
    int sca_low = 27, sca_high = 33;

    std::vector<int> misure = { 183, 183, 185, 185, 198, 195, 195, 195,
        191, 193, 193, 190, 195, 188, 188, 181 };

    std::vector<int> spessori = {178, 178, 180, 180, 183, 160, 180, 180,
        166, 168, 173, 165, 170, 188, 163, 161};

    z3::expr spessori_expr = c.constant("A", c.array_sort(c.int_sort(), c.int_sort()));
    // Crea il vettore interno e assegna i valori
    for (int i = 0; i < 16; ++i) {
        opt.add(z3::select(spessori_expr, i+1) == spessori[i]);
    }

    std::vector<z3::expr> scelte;
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
    z3::expr num_empty = c.int_val(0);
    for (auto &v : scelte) {
        num_empty = num_empty + z3::ite(v == 0, c.int_val(1), c.int_val(0));
    }

    // num_changes = count of sce[i] != i+1 (ignoring 0)
    z3::expr num_changes = c.int_val(0);
    for (int i = 0; i < 16; ++i) {
        num_changes = num_changes + z3::ite((scelte[i] != 0 && scelte[i] != (i+1)), c.int_val(1), c.int_val(0));
    }

    // Lexicographic optimization: first num_empty, then num_changes
    opt.minimize(num_empty);
    opt.minimize(num_changes);

    // Solve
    if (opt.check() == z3::sat) {
        z3::model m = opt.get_model();
        std::cout << "num_empty: "   << m.eval(num_empty)   << "\n";
        std::cout << "num_changes: " << m.eval(num_changes) << "\n";
        for (int i = 0; i < 16; ++i) {
            std::cout << scelte[i] << " = " << m.eval(scelte[i]) << "\n";
        }
    } else {
        std::cout << "No solution found.\n";
    }

    return 0;
}
