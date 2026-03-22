#include "cpu.h"
#include "Algorithms/SJF/SJF.h"
#include "Algorithms/RR/RR.h"
#include "Algorithms/SRTF/SRTF.h"

int main(void) {
    SRTF alg;
    std::vector<Process> processes = {
        Process(0, 8, 1, "P1"),  // long job, heavily starved
        Process(1, 4, 2, "P2"),  // preempts P1
        Process(2, 2, 3, "P3"),  // preempts P2
        Process(3, 1, 4, "P4"),  // ties with P3 remaining
        Process(4, 3, 5, "P5"),  // gets starved for a while
        Process(6, 1, 6, "P6"),  // preempts P2 mid-run
        Process(8, 5, 7, "P7"),  // long wait
        Process(9, 2, 8, "P8"),  // short, jumps queue
        Process(12, 1, 9, "P9"), // preempts P5
    };
    Cpu cpu(25, processes);
    result_simulation result = cpu.run_scheduling_sim(alg);

    std::cout << "Avg Response Time:   " << result.avg_response_time << '\n';
    std::cout << "Avg Turnaround Time: " << result.avg_turnaround_time << '\n';
    std::cout << "Avg Waiting Time:    " << result.avg_waiting_time << '\n';
    std::cout << "Throughput:          " << result.throughput << '\n';
}