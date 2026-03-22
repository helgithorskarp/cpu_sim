#include "RR.h"

Process* RR::step(int current_time, Process *running, std::vector<Process *> arrived) {
    /// add all proccesses that just arrived to the ready queue
    for (auto p : arrived) {
        ready_queue.push_back(p);
    }

    bool is_cpu_empty = running == nullptr;
    if (!is_cpu_empty && running->remaining_time <= 0) {
        /// check if currently running CPU has finished its burst time, if so then we need to remove it
        running->completion_time = current_time;
        is_cpu_empty = true;
    }

    /// if this is true we need to preempt currently running proccess
    bool preempt = current_time % quantum_slice == 0;
    Process *new_running = is_cpu_empty ? nullptr : running; /// will be overwritting with a new process if we decide to change

    /// if preempt is true, or there is no proccess running on cpu currently
    /// we try to run a new proccess
    if (preempt || is_cpu_empty) {
        if (preempt && !is_cpu_empty) {
            ready_queue.push_back(running); // re queue the preempted process
        }

        if (!ready_queue.empty()) {
            new_running = ready_queue.front();
            ready_queue.pop_front();

            if (new_running->response_time == -1) { // is this the first time on cpu?
                new_running->response_time = current_time;
            }
        }
    }

    // update statistics for running proccess and return that to cpu
    /// indicating that this is the curretnly running proccess
    if (new_running != nullptr) {
        new_running->remaining_time -= 1;
        return new_running;
    }

    /// nothing is running
    return nullptr;
}
