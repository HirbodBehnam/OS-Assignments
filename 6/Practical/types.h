#ifndef PRACTICAL_TYPES_H
#define PRACTICAL_TYPES_H

#endif //PRACTICAL_TYPES_H

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <random>
#include <vector>
#include <unordered_map>

static uint64_t current_car_id = 0;

class Road {
private:
    uint64_t h;
    std::mutex mu;
    std::condition_variable condvar;

    [[nodiscard]] uint64_t calculate_stuff(uint64_t p) const {
        uint64_t result = 0;
        for (uint64_t k = 0; k <= (uint64_t) 10e7; k++) {
            result += k / ((uint64_t) 10e6 * p * h);
        }
        return result;
    }

    static uint64_t get_current_epoch() {
        using namespace std::chrono;
        milliseconds ms = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()
        );
        return ms.count();
    }

public:
    struct MoveInRoadResult {
        uint64_t lock_got_time, emission, next_emission, end_time;
    };

    explicit Road(uint64_t weight) : h(weight) {

    }

    MoveInRoadResult move_in_road(uint64_t p, std::atomic_uint64_t *total_emission) {
        std::unique_lock lk(this->mu); // lock the road
        uint64_t lock_got_time = get_current_epoch();
        uint64_t emission = calculate_stuff(p);
        uint64_t next_emission = total_emission->fetch_add(emission) + emission;
        uint64_t end_time = get_current_epoch();
        return MoveInRoadResult{
                .lock_got_time = lock_got_time,
                .emission = emission,
                .next_emission = next_emission,
                .end_time = end_time,
        };
    }
};

typedef std::unordered_map<char, std::unordered_map<char, Road *>> roads_t;

class Car {
private:
    uint64_t id;
    uint64_t p;
    size_t path_index;
    std::vector<char> path;

    static uint64_t generate_p() {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        static std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10);
        return dist6(rng);
    }

public:
    struct AdvanceResult {
        Road::MoveInRoadResult road_data;
        char start_node, end_node;
    };

    explicit Car(std::vector<char> &path) : id(++current_car_id), path(path), path_index(0) {
        p = generate_p();
    }

    [[nodiscard]] uint64_t get_id() const {
        return id;
    }

    bool next() {
        if (path_index == path.size() - 1)
            return false;
        path_index++;
        return true;
    }

    AdvanceResult advance(roads_t &r, std::atomic_uint64_t *total_emission) const {
        auto road = r.at(path.at(path_index - 1)).at(path.at(path_index));
        AdvanceResult result = {
                .road_data = road->move_in_road(this->p, total_emission),
                .start_node = path.at(path_index - 1),
                .end_node = path.at(path_index),
        };
        return result;
    }
};