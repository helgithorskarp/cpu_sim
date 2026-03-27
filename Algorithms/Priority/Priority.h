#include "../Algorithms.h"
#include "../../common.h"

class Priority : public Algorithm {
public:
    std::deque<Process*> ready_queue;

    Priority() {}

    Process* step(int current_time, Process* running, std::vector<Process*> arrived) override;
};