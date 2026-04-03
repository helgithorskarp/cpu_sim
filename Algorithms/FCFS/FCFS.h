#ifndef FCFS_H
#define FCFS_H
#include <deque>

#include "../Algorithms.h"

class FCFS : public Algorithm {
public:
  std::deque<Process *> ready_queue;

  FCFS() {}

  Process *step(int current_time, Process *running,
                std::vector<Process *> arrived) override;
};
#endif