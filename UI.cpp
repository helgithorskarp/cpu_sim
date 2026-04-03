#include "UI.h"
#include "imgui.h"

namespace CpuSimUI {
    // monolithic static variables list in case they need to
    // be modified or accessed elsewhere
    static std::vector<Process> processes;
    static std::map<int, std::vector<process_time_seg>> pid_segments;
    static std::string n_process_name;
    static int n_arrival_time = 0;
    static int n_burst_time = 1;
    static int n_priority = -1;
    static int n_pid = 1;
    static int n_sim_runs = 0;
    static std::string add_process_status_msg = "";
    static std::string simulation_status_msg = "";
    static int selected_algorithm_idx = 0;
    static const char* selected_algorithm_preview = "Select an algorithm";
    static algorithm_listing selected_algorithm;
    static std::vector<algorithm_listing> algorithms;
    static int rr_quantum = 1;
    static result_simulation simulation_results;
    static std::set<int> unique_pids;
    static std::map<int, Process> pid_to_process;
    static float offset = 0.0f;
    static float offset_max = 0.0f;
    static float slot_w = 15.0f;
    static float row_h = 25.0f;
    static ImGuiTableFlags processes_table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg;
    static ImGuiTableFlags results_table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

    // Setup Dockspace provided by ImGui
    // Boilerplate from ImGui examples
    void SetDockspace() {
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        ImGui::Begin("Dockspace Parent", nullptr, window_flags);
            if (opt_fullscreen) ImGui::PopStyleVar(2);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace"); // Give it a unique string name

            // 1. Check if the dockspace needs initialization
            // We use DockBuilderGetNode to see if it already exists
            if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {

                // 2. Clear out any old state and start fresh
                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->WorkSize);

                // 3. Create your splits
                ImGuiID dock_id_main = dockspace_id;
                ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.6f, nullptr, &dock_id_main);
                ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Right, 0.5f, nullptr, &dock_id_main);
                ImGuiID dock_id_main_bottom = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.5f, nullptr, &dock_id_main);


                // 4. Dock your windows by name
                ImGui::DockBuilderDockWindow("Create Process", dock_id_main);
                ImGui::DockBuilderDockWindow("Processes", dock_id_right);
                ImGui::DockBuilderDockWindow("Results", dock_id_down); // Center
                ImGui::DockBuilderDockWindow("Simulation", dock_id_main_bottom);

                // 5. Finalize
                ImGui::DockBuilderFinish(dockspace_id);
            }

            // 6. NOW call the actual DockSpace function
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            ImGui::End();
    }

    // Initialize the list of algorithms to be used in the simulation
    // TODO: Allow custom Round Robin quantum slice
    void InitAlgorithms() {
        processes.reserve(1000);
        algorithms.push_back({std::make_shared<FCFS>(), "First Come, First Serve", "FCFS"});
        algorithms.push_back({std::make_shared<RR>(1), "Round Robin", "RR"});
        algorithms.push_back({std::make_shared<SJF>(), "Shortest Job First", "SJF"});
        algorithms.push_back({std::make_shared<SRTF>(), "Shortest Remaining Time First", "SRTF"});
        algorithms.push_back({std::make_shared<Priority>(), "Priority", "Priority"});
    }

    // Create a new process with the given name, arrival time, and burst time
    // Returns 0 on success, error code otherwise
    int CreateProcess(std::string name, int arrival_time, int burst_time, int priority) {
        if (processes.size() >= 999) {
            return PADD_MAX_PROCESSES;
        }
        if (name.empty())
            return PADD_EMPTY_NAME;
        for (const auto& p : processes) {
            if (p.name == name)
                return PADD_EXISTS;
        }
        processes.emplace_back(arrival_time, burst_time, n_pid++, name, priority);
        return 0;
    }

    // Start a simulation using the given algorithm
    // Returns 0 on success, error code otherwise
    int StartSimulation(std::shared_ptr<Algorithm> algorithm) {
        if (processes.empty())
            return SIM_EMPTY_PROCESSES;
        if (algorithm == nullptr)
            return SIM_ALG_NULLPTR;
        int total_time = 0;
        for (const auto& p : processes) {
            total_time += p.burst_time;
        }
        Cpu cpu((total_time < 100) ? 100 : total_time, processes);
        simulation_results = cpu.run_scheduling_sim(*algorithm);

        // Clear previous simulation results and populate pid_segments for rendering
        unique_pids.clear();
        pid_to_process.clear();
        pid_segments.clear();
        if (!simulation_results.timeline.empty()) {
            for (const auto& p : simulation_results.timeline) {
                unique_pids.insert(p.pid);
                pid_to_process[p.pid] = p;
            }

            int current_pid = simulation_results.timeline[0].pid;
            int start_time = 0;

            // Combine consecutive segments of the same pid into a single segment
            for (int i = 1; i <= simulation_results.timeline.size(); ++i) {
                if (i == simulation_results.timeline.size() || simulation_results.timeline[i].pid != current_pid) {
                    pid_segments[current_pid].push_back({start_time, i - start_time});
                    if (i <simulation_results.timeline.size()) {
                        current_pid = simulation_results.timeline[i].pid;
                        start_time = i;
                    }
                }
            }
        }
        return 0;
    }

    // For scrolling the timeline
    void CalculateTimelineOffset() {
        offset_max = 0.0f;
        for (const auto& p : processes) {
            offset_max += p.burst_time;
        }
    }

    // Main UI rendering
    void RenderUI() {
        /*
         * BEGIN - Create process
         */
        if (algorithms.empty())
            InitAlgorithms();

        ImGui::Begin("Create Process", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::InputText("Process Name", &n_process_name);
        ImGui::SliderInt("Arrival Time", &n_arrival_time, 0, 50);
        ImGui::SliderInt("Burst Time", &n_burst_time, 1, 10);
        ImGui::SliderInt("Priority", &n_priority, -1, 10);

        ImGui::Text("%s", (add_process_status_msg.length() == 0) ? "" : add_process_status_msg.c_str());

        if (ImGui::Button("Add Process")) {
            int padd_status = CreateProcess(n_process_name, n_arrival_time, n_burst_time, n_priority);
            if (padd_status == PADD_EMPTY_NAME)
                add_process_status_msg = "Process name cannot be empty";
            else if (padd_status == PADD_EXISTS)
                add_process_status_msg = std::format("Process \"{}\" already exists", n_process_name);
            else if (padd_status == PADD_MAX_PROCESSES)
                add_process_status_msg = "Max number of processes reached (999)";
            else
                add_process_status_msg = std::format("Process \"{}\" (PID: {}) added", n_process_name, n_pid - 1);
        }
        ImGui::SameLine(0, -1);

        if (ImGui::Button("Add Random Process")) {
            n_arrival_time = std::rand() % 50;
            n_burst_time = std::rand() % 9 + 1;
            n_priority = std::rand() % 11 - 1;
            std::string n_process_name = std::format("Process {}", n_pid);
            if ((CreateProcess(n_process_name.c_str(), n_arrival_time, n_burst_time, n_priority)) == PADD_MAX_PROCESSES)
                add_process_status_msg = "Max number of processes reached (999)";
            else
                add_process_status_msg = std::format("Random process (PID: {}) added", n_pid - 1);
        }

        if (ImGui::Button("Add TOO MANY Processes")) {
            int padd_status = 0;
            for (int i = 0; i < 500; ++i) {
                n_arrival_time = std::rand() % 50;
                n_burst_time = std::rand() % 9 + 1;
                n_priority = std::rand() % 11 - 1;
                std::string n_process_name = std::format("Process {}", i);
                int padd_status = CreateProcess(n_process_name.c_str(), n_arrival_time, n_burst_time, n_priority);
            }
            if (padd_status == PADD_MAX_PROCESSES)
                add_process_status_msg = "Max number of processes reached (999)";
            else
                add_process_status_msg = "A ridiculous amount of processes have been added";
        }
        ImGui::End();
        /*
         * END - Create process
         */

        /*
         * BEGIN - Processes list
         */
        ImGui::Begin("Processes", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::BeginTable("Processes", 5, processes_table_flags)) {
            ImGui::TableSetupColumn("PID");
            ImGui::TableSetupColumn("Process Name");
            ImGui::TableSetupColumn("Arrival Time");
            ImGui::TableSetupColumn("Burst Time");
            ImGui::TableSetupColumn("Priority");
            ImGui::TableHeadersRow();

            for (int i = 0; i < processes.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", processes[i].pid);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", processes[i].name.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", processes[i].arrival_time);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%d", processes[i].burst_time);
                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%d", processes[i].priority);
            }
            ImGui::EndTable();
        }
        ImGui::End();
        /*
         * END - Processes list
         */

        /*
         * BEGIN - Run simulation
         */
        ImGui::Begin("Simulation", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::BeginCombo("Algorithms", selected_algorithm_preview)) {
            for (int i = 0; i < algorithms.size(); ++i) {
                const bool is_selected = (selected_algorithm_idx == i);
                if (ImGui::Selectable(algorithms[i].name.c_str(), is_selected)) {
                    selected_algorithm_idx = i;
                    selected_algorithm_preview = algorithms[i].name.c_str();
                    selected_algorithm = algorithms[i];
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (selected_algorithm.ident == "RR") {
            ImGui::SliderInt("RR Quantum", &rr_quantum, 1, 10);
        }

        if (ImGui::Button("Run Simulation")) {
            if (selected_algorithm.ident == "RR") {
                auto temp = std::dynamic_pointer_cast<RR>(selected_algorithm.algorithm);
                if (temp != nullptr) {
                    temp->quantum_slice = rr_quantum;
                }
            }
            int simulation_status = StartSimulation(selected_algorithm.algorithm);
            if (simulation_status == SIM_ALG_NULLPTR)
                simulation_status_msg = "Please select an algorithm";
            else if (simulation_status == SIM_EMPTY_PROCESSES)
                simulation_status_msg = "Please add at least one process";
            else {
                CalculateTimelineOffset();
                simulation_status_msg = std::format("Successfully ran {}", algorithms[selected_algorithm_idx].ident);
            }
        };
        ImGui::Text("%s", simulation_status_msg.c_str());
        ImGui::End();
        /*
         * END - Run simulation
         */

        /*
         * BEGIN - Show simulation results
         */
        ImGui::Begin("Results", nullptr);


        ImGui::SliderFloat("Offset", &offset, 0.0f, offset_max, "%.1f");
        ImGui::SliderFloat("Zoom", &slot_w, 1.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Row Height", &row_h, 25.0f, 100.0f, "%.1f");

        std::vector<Process> timeline = simulation_results.timeline;
        std::vector<Process> process_stats = simulation_results.processes;

        ImGui::Separator();
        if (ImGui::BeginTable("Gantt Chart", 2, results_table_flags, ImVec2(0.0f, 0.0f))) {
            if (!timeline.empty()) {
                ImGui::TableSetupColumn("Process", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                ImGui::TableSetupColumn("Timeline", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                for (int pid : unique_pids) {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, row_h);

                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("%s", pid_to_process[pid].name.c_str());
                    ImGui::SameLine(0, -1);
                    ImGui::TextDisabled("PID: %d", pid);

                    ImGui::TableSetColumnIndex(1);
                    ImVec2 start_pos = ImGui::GetCursorScreenPos();
                    ImDrawList* dl = ImGui::GetWindowDrawList();

                    ImRect cell_rect = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 1);

                    ImGui::PushClipRect(cell_rect.Min, cell_rect.Max, true);
                    for (const auto& seg : pid_segments[pid]) {
                        float x_start = (seg.start_time - offset) * slot_w;
                        float x_end = seg.duration * slot_w;

                        if (x_start + x_end < 0)
                            continue;
                        if (x_start > cell_rect.Max.x - cell_rect.Min.x)
                            break;

                        ImVec2 b_min = ImVec2(start_pos.x + x_start, start_pos.y);
                        ImVec2 b_max = ImVec2(b_min.x + x_end, start_pos.y + row_h);

                        if (b_max.x > cell_rect.Min.x && b_min.x < cell_rect.Max.x) {
                            ImU32 color = ImColor::HSV(fmodf(pid * 0.618f, 1.0f), 0.7f, 0.8f);
                            dl->AddRectFilled(b_min, b_max, color);
                        }
                    }
                    ImGui::PopClipRect();
                }
                ImGui::Dummy(ImVec2(timeline.size() * slot_w, row_h));
            }
            ImGui::EndTable();

            if (!timeline.empty()) {
                ImGui::Separator();
                ImGui::Text("Averages:");
                ImGui::Text("Response time: %f, Turnaround time: %f, Waiting time: %f",
                    simulation_results.avg_response_time, simulation_results.avg_turnaround_time, simulation_results.avg_waiting_time);
                ImGui::Separator();
                for (Process &p : process_stats) {
                    ImGui::Text("%s (PID: %d): Completed at %d, with response time of %d", p.name.c_str(), p.pid, p.completion_time, p.response_time);
                }
            }
        }
        ImGui::End();
        /*
         * END - Show simulation results
         */
    }
}
