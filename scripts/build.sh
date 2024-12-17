#!/bin/bash

# Build the kernel
echo "Building the kernel..."
gcc -o qhynlp_kernel.bin kernel/qhynlp_kernel.c kernel/process_manager.c kernel/memory_manager.c kernel/file_system.c python_support/python_interpreter.c qhynlp_support/qhynlp_interpreter.c blockchain/blockchain.c -I./ -std=c99

echo "Kernel built successfully."
