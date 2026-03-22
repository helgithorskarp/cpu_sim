#include "../Algorithms.h"
#include "../../common.h"

class RR : public Algorithm {
public:
    std::deque<Process*> ready_queue;
    uint32_t quantum_slice;
    uint32_t slice_used;

    RR(uint32_t _quantum_slice)
        : quantum_slice(_quantum_slice), slice_used(0) {}

    Process* step(int current_time, Process* running, std::vector<Process*> arrived) override;
};