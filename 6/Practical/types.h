#ifndef PRACTICAL_TYPES_H
#define PRACTICAL_TYPES_H

#endif //PRACTICAL_TYPES_H

#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>

class Road {
private:
    uint64_t h;
    std::mutex mu;
    std::condition_variable condvar;

    uint64_t calculate_stuff(uint64_t p) const {
        uint64_t result = 0;
        for (uint64_t k = 0; k <= (uint64_t) 10e7; k++) {
            result += k / ((uint64_t) 10e6 * p * h);
        }
        return result;
    }

public:
    explicit Road(uint64_t weight) : h(weight) {

    }

    uint64_t move_in_road(uint64_t p) {
        std::unique_lock lk(this->mu); // lock the road
        return calculate_stuff(p);
    }
};

static uint64_t current_car_id = 0;

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
    explicit Car(std::vector<char> &path) : id(current_car_id++), path(path), path_index(0) {
        p = generate_p();
    }

    bool next() {
        if (path_index == path.size())
            return false;
        path_index++;
        return true;
    }

    /**
     * Advances car to next road
     * @return True if we have reached the final destination otherwise false
     */
    uint64_t advance(Road &road) const {
        road.move_in_road(this->p);
    }
};