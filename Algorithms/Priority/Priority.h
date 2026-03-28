#ifndef PRIORITY_H
#define PRIORITY_H
#include <deque>
#include <algorithm>
#include "../Algorithms.h"

class Priority : public Algorithm {
public:
    std::deque<Process*> ready_queue;

    Priority() {}

    Process* step(int current_time, Process* running, std::vector<Process*> arrived) override;
};
#endif