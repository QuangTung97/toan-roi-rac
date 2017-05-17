#include <solver.h>
#include <ctime>
#include <algorithm>
#include <iostream>


namespace tung {

using std::cout;
using std::endl;


void print_path(const Path& path) {
    for (auto v: path) {
        printf("%2d ", v);
    }
    printf("\n");
}

// Less is better
bool Unit::operator < (const Unit& other) const {
    if (num_missed < other.num_missed) {
        return true;
    }
    else if (num_missed > other.num_missed) {
        return false;
    }
    else {
        if (overflow_rate < other.overflow_rate) {
            return true;
        }
        else if (overflow_rate > other.overflow_rate) {
            return false;
        }
        else {
            if (travel_cost < other.travel_cost)
                return true;
            else
                return false;
        }
    }
}


Solver::Solver(
        const Matrix& mat, 
        const std::vector<int>& overflow_time,
        const int max_travel_time):
    mat{mat}, num_vertices{mat.get_m()},
    max_travel_time{max_travel_time}, 
    overflow_time{overflow_time}
{
    SolverSettings settings;
    num_lookahead = settings.num_lookahead;
    num_remain = settings.num_remain;
    num_units = settings.num_units;

    units.reserve(num_units);
    weighted_sums.reserve(num_lookahead * num_remain);
}

Solver::Solver(
        const Matrix& mat, 
        const std::vector<int>& overflow_time,
        const int max_travel_time,
        const SolverSettings& settings):
    mat{mat}, num_vertices{mat.get_m()},
    max_travel_time{max_travel_time},
    overflow_time{overflow_time}
{
    num_lookahead = settings.num_lookahead;
    num_remain = settings.num_remain;
    num_units = settings.num_units;

    units.reserve(num_units);
    weighted_sums.reserve(num_lookahead * num_remain);
}

void Solver::solve(int num_loops) {
    init();
    calculate_fitness();
    selection();

    for (int i = 0; i < num_loops; i++) {
        crossover();
        mutation();
        calculate_fitness();
        selection();
    }
}

void Solver::init() {
    // Init random engine
    rand_engine.seed(time(nullptr));

    // Init all weights of all units
    for (int i = 0; i < num_units; i++) {
        Unit unit;
        unit.weights.resize(num_lookahead);
        unit.weights[0].travel = 1;
        unit.weights[0].timeout = 0;
        for (int j = 1; j < num_lookahead; j++) {
            float w1 = std::max(normal(rand_engine), 0.0);
            float w2 = std::max(normal(rand_engine), 0.0);
            unit.weights[j] = Unit::Weight{w1, w2};
        }

        units.push_back(unit);
    }
}

void Solver::calculate_fitness() {
    for (auto& unit: units) 
        calculate_fitness_for_unit(unit);
}

void Solver::calculate_fitness_for_unit(Unit& unit) 
{
    // init remains before continous get_next
    RemainVertex remain;
    remain.index = 0;
    remain.current_time = 0;
    remain.overflow_time = overflow_time;

    remains.clear();
    remains.push_back(remain);

    // Init path of unit
    unit.path = {0};
    
    // Do something here
    auto is_terminated = [](const Path& path)->bool {
        return path.size() > 1 && path.back() == 0;
    };

    while (!is_terminated(unit.path)) {
        get_next_remains(unit);
        choose_next_vertices(unit);
    }

    calculate_num_missed(unit);
    calculate_overflow_rate(unit);
    calculate_travel_cost(unit);
}

void Solver::get_next_remains(const Unit& unit) 
{
    for (int depth = 0; depth < num_lookahead; depth++) {
        weighted_sums.clear();

        for (auto& remain: remains) {
            for (int vertex = 1; vertex < num_vertices; vertex++) {
                if (vertex == remain.index) 
                    continue;

                float sum = calculate_weighted_sum(unit, depth, 
                                remain, vertex);
                WeightedSum wsum;
                wsum.belong = &remain;
                wsum.sum = sum;
                wsum.vertex_index = vertex;
                weighted_sums.push_back(wsum);
            }
        }
         
        // Sort by sum asc
        std::sort(weighted_sums.begin(), weighted_sums.end(),
            [](WeightedSum &a, WeightedSum &b) {
                return a.sum < b.sum;
        });

        std::vector<RemainVertex> tmp_remains(num_remain);
        size_t N = std::min((size_t)num_remain, 
                weighted_sums.size());

        // Choose num_remain of best
        
        // Copy to other locate to avoid collision
        for (size_t i = 0; i < N; i++) {
            auto& wsum = weighted_sums[i];
            tmp_remains.push_back(*wsum.belong);
            wsum.belong = &tmp_remains.back();
        }

        remains.clear();
        for (size_t i = 0; i < N; i++) {
            auto& wsum = weighted_sums[i];
            int vertex_current = wsum.belong->index;
            int vertex_next = wsum.vertex_index;

            RemainVertex remain;
            remain = *wsum.belong;
            remain.index = vertex_next;

            remain.current_time += 
                mat(vertex_current, vertex_next);

            remain.overflow_time[vertex_next] = 
                remain.current_time + 
                this->overflow_time[vertex_next];

            remain.path.push_back(vertex_next);
            
            remains.push_back(remain);
        }
    }
}

float Solver::calculate_weighted_sum(const Unit &unit, int depth,
        RemainVertex &r, int vertex) 
{
    float sum = mat(r.index, vertex) * unit.weights[depth].travel;

    sum += (r.overflow_time[vertex] - r.current_time) 
        * unit.weights[depth].timeout;
    return sum;
}

void Solver::choose_next_vertices(Unit& unit) 
{
    const auto& best = remains[0];
    if (best.current_time < max_travel_time) {
        unit.path = best.path;
        return;
    }

    int current_time = best.current_time;
    Path path = best.path;

    int end = path.back();
    path.push_back(0);
    current_time += mat(end, 0);

    while (current_time > max_travel_time) {
        size_t N = path.size();
        int vertex_prev = path[N - 3];
        int vertex_current = path[N - 2];
        auto vertex_current_it = std::next(path.begin(), N - 2);

        current_time -= mat(vertex_current, 0);
        current_time -= mat(vertex_prev, vertex_current);
        current_time += mat(vertex_prev, 0);
        path.erase(vertex_current_it);
    }

    unit.path = path;
}

int Solver::get_travel_time(const Path& path) 
{
    int time = 0;
    size_t N = path.size();
    for (size_t i = 0; i < N - 1; i++) {
        time += mat(path[i], path[i + 1]);
    }

    return time;
}

void Solver::calculate_num_missed(Unit& unit) 
{
    std::vector<bool> visited(num_vertices);
    std::fill(visited.begin(), visited.end(), false);

    size_t N = unit.path.size();
    for (size_t i = 0; i < N - 1; i++) {
        visited[i] = true;
    }

    unit.num_missed = std::count(visited.begin(),
                        visited.end(), false);
}

void Solver::calculate_overflow_rate(Unit& unit) 
{
    auto overflow_time = this->overflow_time;
    int current_time = 0;
    size_t N = unit.path.size();
    const Path& path = unit.path;

    const int num_travelled = N - 2; 
    int overflow_count = 0;

    for (size_t i = 1; i < N - 1; i++) {
        current_time += mat(path[i - 1], path[i]);
        if (current_time > overflow_time[path[i]])
            overflow_count++;

        overflow_time[path[i]] = current_time
            + this->overflow_time[path[i]];
    }

    unit.overflow_rate = (float)overflow_count / num_travelled;
}

void Solver::calculate_travel_cost(Unit& unit) {
    auto overflow_time = this->overflow_time;
    int current_time = 0;
    size_t N = unit.path.size();
    const Path& path = unit.path;
    const int num_travelled = N - 2; 

    float travel_cost = 0;

    for (size_t i = 1; i < N - 1; i++) {
        current_time += mat(path[i - 1], path[i]);
        if (current_time > overflow_time[path[i]])
            travel_cost += FITNESS_OVERFLOW_COEFFICIENT *
                (current_time - overflow_time[path[i]]);
        else
            travel_cost += overflow_time[path[i]] - current_time;
                

        overflow_time[path[i]] = current_time
            + this->overflow_time[path[i]];
    }

    unit.travel_cost = travel_cost / num_travelled;
}

void Solver::selection() {
}

void Solver::crossover() {
}

void Solver::mutation() {
}


} // namespace tung
