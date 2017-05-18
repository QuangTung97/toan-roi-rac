#include <stdio.h>
#include <position_reader.h>
#include <overflow_time.h>
#include <iostream>
#include <matrix.h>
#include <solver.h>
#include <string>
#include <algorithm>
#include <path.h>
#include <chrono>


using std::cout;
using std::endl;
using namespace std::chrono;

int main() {
    tung::MatrixReader reader;
    auto mat = reader.read("matrix.txt");

    tung::OverflowTime time_reader;
    auto overflow_time = time_reader.read("overflow_time.txt");
    tung::print_path(overflow_time);

    tung::SolverSettings settings;
    settings.num_remain = 10;
    settings.num_units = 400;
    settings.num_lookahead = 4;

    tung::Solver solver{mat, overflow_time, 
                        30000, settings};
    auto t0 = steady_clock::now();
    solver.solve(20);
    auto t1 = steady_clock::now();
    nanoseconds dt = t1 - t0;

    cout << duration_cast<hours>(dt).count() << ":";
    dt -= duration_cast<hours>(dt);
    cout << duration_cast<minutes>(dt).count() << ":";
    dt -= duration_cast<minutes>(dt);
    cout << duration_cast<seconds>(dt).count() << "s  ";
    dt -= duration_cast<seconds>(dt);
    cout << duration_cast<milliseconds>(dt).count() << "ms" << endl;

    tung::PathWriter writer;
    writer.write("result.txt", solver.get_best());
	return 0;
}

