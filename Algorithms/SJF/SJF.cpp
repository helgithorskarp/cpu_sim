#include "SJF.h"


Process* SJF::step(int current_time, Process* running, std::vector<Process*> arrived) {
    /// add each process to the existing_procceses vector
    for (auto& p: arrived) {
        existing_procceses.push_back(p);
    }

    bool is_cpu_empty = running == nullptr;

    Process* new_candidate = running;

    /// if cpu is empty and there exist processes that could possibly run
    if (is_cpu_empty && !existing_procceses.empty()) {
        for (auto& p : existing_procceses) {
            if (p->remaining_time > 0 && new_candidate == nullptr) {
                new_candidate = p;
            } else if (new_candidate == nullptr) {
                continue;
            }

            /// if this process has less burst time than the best candidate
            /// and also has some remaining time left, then it is the new best candidate
            if (p->burst_time < new_candidate->burst_time && p->remaining_time > 0) {
                new_candidate = p;
            }
        }
    }

    // FIX: existing_process is now deque and processes are correctly removed
    existing_procceses.erase(
        std::remove(existing_procceses.begin(), existing_procceses.end(), new_candidate),
        existing_procceses.end()
    );

    /// update the remaining time of the running process
    /// and set the response time if its the process has never been on the cpu
    if (new_candidate != nullptr) {
        if (new_candidate->response_time == -1) {
            new_candidate->response_time = current_time;
        }

        new_candidate->remaining_time -= 1;
        return new_candidate;
    }


    return nullptr;
}
