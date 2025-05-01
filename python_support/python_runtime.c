/*
 * python_runtime.c
 * Embedded Python Runtime for Quantum SMS OS
 * Integrates with Blockchain, QhyNLP, Themes, and Quantum Gateway
 */

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PY_MAX_SCRIPT 4096
#define PY_RESPONSE_SIZE 1024

// Theme system integration
extern void apply_theme(const char* theme);
extern char active_theme[16];

// Blockchain integration
extern bool add_transaction(struct Block* block, const char* sender, const char* recipient, double amount, const char* message);
extern struct Blockchain* chain;
extern struct Block* mine_block(void);

// Python Script Buffer
char python_script_buffer[PY_MAX_SCRIPT] = {0};

// Python Error Handler
void handle_python_error() {
    if (PyErr_Occurred()) {
        PyErr_Print();
        fprintf(stderr, "🚨 Python execution error occurred!\n");
    }
}

// Initialize Python Environment
void init_python_runtime() {
    Py_Initialize();
    if (!Py_IsInitialized()) {
        fprintf(stderr, "❌ Failed to initialize Python interpreter!\n");
        exit(1);
    }
    printf("🐍 Python runtime initialized successfully\n");
}

// Finalize Python Environment
void shutdown_python_runtime() {
    Py_Finalize();
    printf("🔒 Python runtime finalized\n");
}

// Execute raw Python script
void execute_python_script(const char* script) {
    printf("📄 Executing Python script:\n%s\n", script);

    int result = PyRun_SimpleString(script);
    if (result != 0) {
        handle_python_error();
    } else {
        printf("✅ Script executed successfully.\n");
    }
}

// Execute Python script and capture output
void execute_python_with_output(const char* script, char* output, size_t output_size) {
    PyObject *py_stdout, *py_stringio, *py_script;

    PyRun_SimpleString("import sys\nimport io\nsys.stdout = io.StringIO()");

    py_script = PyRun_String(script, Py_file_input, PyEval_GetGlobals(), PyEval_GetLocals());
    if (!py_script) {
        handle_python_error();
        snprintf(output, output_size, "Python execution failed.");
        return;
    }

    py_stdout = PyRun_String("sys.stdout.getvalue()", Py_eval_input, PyEval_GetGlobals(), PyEval_GetLocals());
    if (py_stdout && PyUnicode_Check(py_stdout)) {
        strncpy(output, PyUnicode_AsUTF8(py_stdout), output_size);
    } else {
        strncpy(output, "❌ Could not capture output.", output_size);
    }

    Py_XDECREF(py_stdout);
    Py_XDECREF(py_script);
}

// Predefined script for blockchain testing
void run_blockchain_test_script() {
    const char* script =
        "print('Simulating Python blockchain transaction')\n"
        "sender = 'py.user.qsms'\n"
        "recipient = 'block.qsms'\n"
        "amount = 42.0\n"
        "message = 'Python-powered transaction 🐍'\n";

    execute_python_script(script);
    add_transaction(chain->latest, "py.user.qsms", "block.qsms", 42.0, "Python-powered transaction 🐍");
    mine_block();
}

// Standalone Test Hook
#ifdef PY_RUNTIME_MAIN
int main() {
    extern struct Blockchain* init_blockchain();
    extern void traverse_chain();

    init_python_runtime();

    chain = init_blockchain();
    run_blockchain_test_script();

    char py_output[PY_RESPONSE_SIZE];
    execute_python_with_output("print('📡 Python says hello from Quantum SMS OS')", py_output, sizeof(py_output));
    printf("📝 Output: %s\n", py_output);

    traverse_chain();

    shutdown_python_runtime();
    return 0;
}
#endif
