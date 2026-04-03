#include "SRTF.h"


Process* SRTF::step(int current_time, Process* running, std::vector<Process*> arrived) {
    /// add each process to the existing_procceses vector
    for (auto& p: arrived) {
        existing_procceses.push_back(p);
    }

    bool is_cpu_empty = running == nullptr;
    Process* new_candidate = running;


    /// if cpu is not empty, loop trough all the newly arrived process and see if any of them have smaller remaining time than the currently
    /// running proccess

    if (!is_cpu_empty) {
        if (new_candidate != running)
            existing_procceses.push_back(running);
        for (auto& p : arrived) {
            if (p->remaining_time > 0 && p->remaining_time < new_candidate->remaining_time) {
                new_candidate = p;
            }
        }
    } else if (is_cpu_empty && !existing_procceses.empty()) {  /// If cpu is empty, Loop trough all procceses and pick the one that has the lowest remaining time left
        for (auto& p : existing_procceses) {
            if (p->remaining_time > 0 && new_candidate == nullptr) {
                new_candidate = p;
            } else if (new_candidate == nullptr) {
                continue;
            }
            /// if current process has less remaining time than the best candidate
            /// and has some time left then its the new candidate
            if (p->remaining_time < new_candidate->remaining_time && p->remaining_time > 0) {
                new_candidate = p;
            }
        }
    }

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
