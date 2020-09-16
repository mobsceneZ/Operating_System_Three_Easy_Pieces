#ifndef __pro_structure
#define __pro_structure

typedef unsigned int unit;

// the registers xv6 will save and restore
// to stop subsequently restart a process
struct context {
    int eip;
    int esp;
    int ebx;
    int acx;
    int edx;
    int esi;
    int edi;
    int ebp;
};

// the different states a process can be in
enum proc_state {
    UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE
};

// the infomation xv6 tracks about each process
// including its register context and state
struct proc {
    char *mem;                  // Start of process memory
    unit sz;                    // Size of process memory
    char *kstack;               // Bottom of kernel stack

    enum proc_state state;      // Process state
    int pid;                    // Process ID
    struct proc* parent;        // Parent process
    void *chan;                 // If !zero, sleeping on chan
    int killed;                 // If !zero, has been killed
    struct file * ofile[NOFILE];// Open files
    struct inode *cwd;          // Current directory
    struct context context;     // switch here to run process
    struct trapframe *tf;       // Trap frame for the current interrupt
};


#endif