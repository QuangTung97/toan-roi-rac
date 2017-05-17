#include <stdio.h>
#include <position_reader.h>
#include <overflow_time.h>
#include <iostream>
#include <matrix.h>
#include <solver.h>


using std::cout;
using std::endl;


int main() {
    tung::MatrixReader reader;
    auto mat = reader.read("matrix.txt");

    tung::OverflowTime time_reader;
    auto overflow_time = time_reader.read("overflow_time.txt");
    tung::print_path(overflow_time);

    tung::SolverSettings settings;
    settings.num_remain = 10;
    settings.num_units = 3;
    settings.num_lookahead = 3;

    tung::Unit unit;
    tung::Unit::Weight w;
    w.travel = 1;
    w.timeout = 1;

    // num_lookahead = 3
    unit.weights.push_back(w);
    unit.weights.push_back(w);
    unit.weights.push_back(w);

    tung::Solver solver{mat, overflow_time, 
                        10000, settings};
    solver.init();

    cout << "---------------------------------\n";
    solver.calculate_fitness_for_unit(unit);

    printf("num_missed: %d \n", unit.num_missed);
    printf("overflow_rate: %f \n", unit.overflow_rate);
    printf("travel_cost: %f \n", unit.travel_cost);

    tung::print_path(unit.path);
    printf("%d\n", solver.get_travel_time(unit.path));
    // solver.solve(1);
	return 0;
}

