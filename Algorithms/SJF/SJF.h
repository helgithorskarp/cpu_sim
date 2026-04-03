#ifndef SJF_H
#define SJF_H
#include <algorithm>
#include <deque>

#include "../Algorithms.h"

class SJF : public Algorithm {
public:
  std::deque<Process *> existing_procceses;

  /// takes in no arguments, simply just runs the job with shortest burst
  SJF() {};

  Process *step(int current_time, Process *running,
                std::vector<Process *> arrived) override;
};
#endif
