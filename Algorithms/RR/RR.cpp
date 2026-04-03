#include "RR.h"

Process *RR::step(int current_time, Process *running,
                  std::vector<Process *> arrived) {
  // Add newly arrived processes to ready queue
  for (auto p : arrived) {
    ready_queue.push_back(p);
  }

  bool is_cpu_empty = (running == nullptr);

  // If current process has used up its quantum, switch it out
  if (!is_cpu_empty && slice_used == quantum_slice) {
    if (!ready_queue.empty()) {
      ready_queue.push_back(running);
      running = ready_queue.front();
      ready_queue.pop_front();

      if (running->response_time == -1) {
        running->response_time = current_time;
      }
    }

    // Reset quantum counter for the next slice
    slice_used = 0;
  }

  // If CPU is empty, get next process from ready queue
  if (running == nullptr) {
    if (ready_queue.empty()) {
      return nullptr;
    }

    running = ready_queue.front();
    ready_queue.pop_front();

    if (running->response_time == -1) {
      running->response_time = current_time;
    }

    slice_used = 0;
  }

  // Run current process for 1 time unit
  running->remaining_time -= 1;
  slice_used += 1;

  return running;
}
