#include "cpu.h"
#include "Algorithms/RR/RR.h"

int main(void) {
    RR alg(2);

    std::vector<Process> processes = {
        Process(0, 7, 1, "P1"),
        Process(1, 4, 2, "P2"),
        Process(2, 5, 3, "P3"),
        Process(4, 3, 4, "P4"),
        Process(6, 6, 5, "P5"),
    };

    Cpu cpu(24, processes);
    result_simulation result = cpu.run_scheduling_sim(alg);

    std::cout << "Avg Response Time:   " << result.avg_response_time << '\n';
    std::cout << "Avg Turnaround Time: " << result.avg_turnaround_time << '\n';
    std::cout << "Avg Waiting Time:    " << result.avg_waiting_time << '\n';
    std::cout << "Throughput:          " << result.throughput << '\n';
}