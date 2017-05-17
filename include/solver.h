#ifndef SOLVER_H
#define SOLVER_H


#include <matrix.h>
#include <vector>
#include <random>
#include <chrono>


namespace tung {

using namespace std::chrono;
typedef std::vector<int> Path;

void print_path(const Path& path);

struct Unit {
    int num_missed; // Number of vertices are not visited
    float overflow_rate;
    float travel_cost;

    struct Weight {
        float travel;
        float timeout;
    };
    std::vector<Weight> weights;

    Path path;

    // Less is better
    bool operator < (const Unit& other) const;
};


struct RemainVertex {
    int index;
    int current_time = 0;
    std::vector<int> path = {0};
    std::vector<int> overflow_time;
};


struct WeightedSum {
    float sum;
    RemainVertex *belong;
    int vertex_index;
};


struct SolverSettings {
    int num_lookahead = 4;
    int num_remain = 10;
    int num_units = 100;
};


class Solver {
private:
    const Matrix &mat;
    const int num_vertices;
    const int max_travel_time;
    const int FITNESS_OVERFLOW_COEFFICIENT = 10;

    int num_lookahead;
    int num_remain;
    int num_units;

    std::vector<Unit> units;

    const std::vector<int> overflow_time;

    const float selection_rate = 0.3;
    const float mutation_rate = 0.03;

    std::default_random_engine rand_engine;
    std::normal_distribution<> normal{2, 2};
    std::uniform_real_distribution<float> uniform{0, 1};

    std::vector<RemainVertex> remains;
    std::vector<WeightedSum> weighted_sums;

// private
public:
    // ------------------------------------------
    // -------- For calculate_fitness -----------
    // ------------------------------------------
    float calculate_weighted_sum(const Unit& unit, int depth,
            RemainVertex &r, int v);
    
    void get_next_remains(const Unit& unit);
    void choose_next_vertices(Unit& unit);
    void calculate_num_missed(Unit& unit);
    void calculate_overflow_rate(Unit& unit);
    void calculate_travel_cost(Unit& unit);

    void calculate_fitness_for_unit(Unit& unit);
    // -------- calculate_fitness -----------

    int get_travel_time(const Path& path);

    void init();
    void calculate_fitness();
    void selection();
    void crossover();
    void mutation();

public:
    Solver(const Matrix &mat, 
        const std::vector<int>& overflow_time,
        const int max_travel_time);

    Solver(const Matrix &mat, 
        const std::vector<int>& overflow_time,
        const int max_travel_time,
        const SolverSettings& settings);

    void solve(int num_loops);

};

} // namespace tung

#endif
