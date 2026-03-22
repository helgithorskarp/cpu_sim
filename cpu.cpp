#include "cpu.h"

result_simulation Cpu::run_scheduling_sim(Algorithm& scheduling_algorithm) {
    bool did_finish = false;
    int finish_time;

    /// Process with id -1, in timeline represents nothing being running
    Process empty_process(-1, -1, -1, "EMPTY", -1);

    for (int i = 0; i < time_line_length; i++) {
        /// get all procceses that arrived at this time slice
        std::vector<Process*> arrived_procceses;
        for (auto& p : processes) {
            if (p.arrival_time == i)
                arrived_procceses.push_back(&p);
        }

        /// /// send running process, and unit time num to algorithm, and the procceses that arrived
        Process* res = scheduling_algorithm.step(i, running_process, arrived_procceses);

        /// if no change was made, go to next unit time
        running_process = res;
        if (res != nullptr) {
            time_line.push_back(*running_process);
            if (running_process->remaining_time == 0) { /// has the process finished?
                running_process->completion_time = i + 1;
                running_process = nullptr;
            }
        } else {
            time_line.push_back(empty_process);
        }


        /// Check if all proccesses are done if so break loop early
        did_finish = true;
        for (auto& p : processes) {
            if (p.completion_time == -1) {
                did_finish = false;
                break;
            }
        }
        if (did_finish) {
            finish_time = i + 1;
            break;
        }
    }

    ////  Calculate the statistics of simulation
    int total_waiting_sum = 0;
    int total_completed = 0;
    int total_response_sum = 0;
    int response_count = 0;
    int total_turnaround_sum = 0;

    for (const auto& p : processes) {
        if (p.response_time != -1) {
            /// if proccess has ran on cpu then then its response time is
            response_count++;
            total_response_sum += p.response_time - p.arrival_time;
        }
        if (p.completion_time != -1) {
            total_completed += 1;
            /// total waiting waiting time is if proccess completed its run:
            total_waiting_sum += (p.completion_time - p.arrival_time) - p.burst_time;
            /// Total turnaround time here would be completation time - arrival time
            total_turnaround_sum += p.completion_time - p.arrival_time;
        }
        /// if proccess does not finish we do not include it in the calculations
    }

    /// calculate eveything needed for the result struct and return it
    result_simulation result;
    result.avg_response_time = static_cast<double>(total_response_sum) / response_count;
    result.avg_turnaround_time = static_cast<double>(total_turnaround_sum) / total_completed;
    result.avg_waiting_time = static_cast<double>(total_waiting_sum) / total_completed;
    result.processes = processes;
    result.timeline = time_line;

    if (did_finish) {
        result.throughput = static_cast<double>(total_completed) / finish_time;
    } else {
        result.throughput = static_cast<double>(total_completed) / time_line_length;
    }

    return result;
}