#ifndef SRTF_H
#define SRTF_H
#include <deque>
#include <algorithm>
#include "../Algorithms.h"

class SRTF : public Algorithm {
    public:
        std::deque<Process*> existing_procceses;

        /// takes in no arguments, simply just runs the job with shortest burst
        SRTF() {};

        Process* step(int current_time, Process* running, std::vector<Process*> arrived) override;
};
#endif
