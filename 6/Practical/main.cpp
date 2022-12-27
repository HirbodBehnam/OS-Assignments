#include <atomic>
#include <fstream>
#include <list>
#include <iostream>
#include <thread>
#include <string>
#include <unordered_map>
#include "types.h"
#include "util.h"

// List of all roads
std::unordered_map<char, std::unordered_map<char, Road *>> roads;
// Total emission of cars
std::atomic_uint64_t total_emission;

void car_thread(Car *car) {
    // TODO
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Please enter the input filename as first argument" << std::endl;
        return 1;
    }
    // Read input
    std::ifstream input_file(argv[1]);
    while (true) { // read roads
        std::string line;
        std::getline(input_file, line);
        if (line == "#") // end of roads
            break;
        auto parts = split_string(line, "-");
        char s1 = parts.at(0).at(0), s2 = parts.at(1).at(0);
        uint64_t h = std::strtoull(parts.at(2).c_str(), nullptr, 10);
        roads[s1][s2] = new Road(h);
    }
    // Read cars and create treads
    std::list<std::thread> threads;
    while (true) {
        // Read input
        std::string line;
        if (!std::getline(input_file, line))
            break;
        auto path = split_string(line, "-");
        std::vector<char> path_c(path.size());
        for (auto &p: path)
            path_c.push_back(p.at(0));
        Car *car = new Car(path_c);
        threads.emplace_back(car_thread, car);
    }
    input_file.close();
    // Wait for threads
    for (auto &t: threads)
        t.join();
    // Done
    return 0;
}
