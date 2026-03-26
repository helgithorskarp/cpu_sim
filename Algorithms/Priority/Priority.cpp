#include "Priority.h"

Process* Priority::step(int current_time, Process* running, std::vector<Process*> arrived) {
    // Add newly arrived processes to ready queue
    for (auto p : arrived) {
        ready_queue.push_back(p);
    }

    // If CPU is empty, get next process from ready queue
    if (running == nullptr) {
        if (ready_queue.empty()) {
            return nullptr;
        }

        // Sort by priority 
        std::sort(ready_queue.begin(), ready_queue.end(), [](Process* a, Process* b) {
            // If equal, FCFS
            if (a->priority == b->priority) {
                return a->arrival_time < b->arrival_time;
            }
            return a->priority > b->priority; 
        });

        running = ready_queue.front();
        ready_queue.pop_front();

        if (running->response_time == -1) {
            running->response_time = current_time;
        }
    }

    // Run current process for 1 time unit
    running->remaining_time -= 1;

    return running;
}