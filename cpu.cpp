#include "cpu.h"




result_simulation Cpu::run_scheduling_sim(Algorithm& scheduling_algorithm) {
    for (int i = 0; i < time_line_length; i++) {

        /// send running process, and unit time num to algorithm
        auto res = scheduling_algorithm.step(i, &running_process);

        /// if no change was made, go to next unit time
        if (!res) {
            time_line[i] = running_process;
        };

        /// update running process if a new process was returned
        Process new_process = *res;
        time_line[i] = new_process;
        running_process = new_process;
    }


    /// calculate eveything needed for the result struct and return it
    result_simulation result;
    result.timeline = time_line;
    ///... For every result metric


    return result;
}