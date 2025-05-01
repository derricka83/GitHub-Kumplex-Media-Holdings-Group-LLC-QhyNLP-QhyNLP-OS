/*
 * qhynlp_kernel.c
 * Quantum SMS OS Kernel Main Loop
 * Coordinates all subsystems including Blockchain, Python, QhyNLP, Filesystem, and Process Scheduler
 */

#include "kernel.h"
#include "python_interpreter.h"
#include "qhynlp_interpreter.h"
#include "file_system.h"
#include "blockchain.h"
#include "memory_manager.h"
#include "process_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // for sleep()

// Debug flag
#define ENABLE_LOGGING 1
#define CYCLE_DELAY 1  // seconds between kernel cycles

void kernel_log(const char* msg) {
#if ENABLE_LOGGING
    printf("🧠 [Kernel] %s\n", msg);
#endif
}

// Boot sequence
void init_all_services() {
    kernel_log("Initializing all kernel subsystems...");
    
    apply_theme(THEME_DARK);
    print_theme_banner();

    init_memory_manager();
    init_process_manager();
    init_file_system();
    chain = init_blockchain();
    init_python_runtime();
    init_qhynlp_runtime();

    kernel_log("All subsystems initialized successfully.");
}

// Main execution loop
void kernel_loop() {
    kernel_log("Entering main runtime loop...");

    while (1) {
        // 🕒 Schedule and manage processes
        schedule_processes();

        // 🐍 Run embedded Python applications
        run_python_scripts();

        // 🧬 Run QhyNLP-based apps and commands
        run_qhynlp_scripts();

        // ⛓️ Synchronize and audit blockchain state
        handle_blockchain_operations();

        // 🌙 Sleep cycle (can adjust or remove for live kernel)
        sleep(CYCLE_DELAY);
    }
}

// Main kernel entry point
int main(int argc, char* argv[]) {
    printf("🚀 Quantum SMS OS Booting...\n");

    init_all_services();
    kernel_loop();

    return 0;
}
