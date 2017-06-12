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
    Unit();
    Unit(const Unit& other);
    Unit(Unit&& other) noexcept;
    Unit& operator = (Unit&& other) noexcept;
    Unit& operator = (const Unit& other);

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
    bool operator < (const Unit& other) const noexcept;
};


struct RemainVertex {
    int index;
    int current_time = 0;
    std::vector<int> path;
    std::vector<int> overflow_time;
    float sum = 0.0;

    RemainVertex() {
        path.reserve(1000);
        path.push_back(0);
    }
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

    float survive_ratio = 0.3;
    float mutation_prob = 0.05;
};


class Solver {
private:
    const Matrix &mat;
    const std::vector<int> overflow_time;

    // ------------------------------------------
    // ------------- Settings -------------------
    // ------------------------------------------
    const int num_vertices;
    const int max_travel_time;
    const int FITNESS_OVERFLOW_COEFFICIENT = 10;
    const float survive_ratio;
    const float mutation_prob;
    const int num_lookahead;
    const int num_remain;
    const int num_units;

    std::vector<Unit> units;

    // ------------------------------------------
    // -------------- Random --------------------
    // ------------------------------------------
    std::mt19937 rand_engine;
    std::normal_distribution<> normal{2, 2};
    std::uniform_real_distribution<float> uniform{0, 1};

    std::vector<RemainVertex> remains;
    std::vector<WeightedSum> weighted_sums;

private:
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

    // Cross and mutation
    float make_float_by_swap_bits(float a, float b);
    float random_switch_bits(float n);

    void init();
    void calculate_fitness(float survive_ratio = 0.0);
            
    void selection();
    void crossover();
    void mutation();

public:
    Solver(const Matrix &mat, 
        const std::vector<int>& overflow_time,
        const int max_travel_time,
        const SolverSettings& settings = SolverSettings{});

    void solve(int num_loops);

    Path get_best() {
        return units[0].path;
    }
};

} // namespace tung

#endif
