# Debugging Multithreaded Programs - Practice

This repository contains some code to go along with the lab on debugging multithreaded programs.

## `assert` and Structural Invariants

queue.cpp contains code for the example covered in lab on validating structural invariants for the
threadsafe queue discussed in lecture. To build and execute the program with assertions enabled, 
run `make debug` and `./debug`. To build and execute with assertions disabled, run `make ndebug`
and `./ndebug`. Try commenting out different lines in `enqueue` and `dequeue` to introduce bugs
and see if the assertions catch it.

## Using `cout` to find bugs.

piano.cpp contains version 2 of the piano lab from last week, with some added bugs. Using print 
statements around calls to `wait` and changes to shared state, identify which threads are improperly
blocked and try to locate the bugs.

## GDB Reference Guide

Here you can find a quick reference guide for some particularly useful GDB functionality. Where
relevant, the corresponding commands in LLDB for MacOS are included. Most of these commands have
short versions which are quicker to type once you're comfortable with them. I've omitted them
for brevity as they can be found quickly online.

For a more thorough list of useful debugger features and a mapping between GDB and LLDB commands,
see [this page](https://lldb.llvm.org/use/map.html).

#### Execution

In GDB/LLDB, you can step through execution just as in a visual debugger. Once execution is paused,
you can `step`, which is equivalent to "step into" in a visual debugger. You can also do `next`, which
is equivalent to "step over".

#### Breakpoints

Breakpoints allow you to pause your program's execution when you hit a given line. To set a breakpoint
in GDB, run the following command:
```
(gdb) break <filename>:<line>
```
In LLDB, the command is as follows:
```
(lldb) breakpoint set --file <filename> --line <line>
```

### Conditional Breakpoints

Conditional breakpoints are like normal breakpoints, but only pause execution if some condition is true.
These conditions can be based on the state of your program. To set a conditional breakpoint in GDB, run
the following command:
```
(gdb) break <filename>:<line> if <cond>
```
In LLDB, the command is as follows:
```
(lldb) breakpoint set --file <filename> --line <line> --condition '<cond>' # note the quotes around <cond>
```

### Watchpoints

Watchpoints are similar to breakpoints, except that they pause execution when a given variable or location
in memory is modified. This is extremely useful if you know some state in your program has the wrong
value at some point, but aren't sure where its being changed. To set a watchpoint in GDB, run the following
command:
```
(gdb) watch <var_name> # watch a variable
(gdb) watch -location <my_ptr> # watch a memory location
```
In LLDB, the command is as follows:
```
(lldb) watchpoint set variable <var_name> # watch a variable
(lldb) watchpoint set expression -- <my_ptr> # watch a memory location
```

For both breakpoints and watchpoints, you can see a list of all created points as follows:
```
(gdb) info break # show breakpoints
(gdb) info watch # show watchpoints
```
In LLDB,
```
(lldb) breakpoint list
(lldb) watchpoint list
```

### Examining Variables

When execution is paused, you can print the current value of a variable. In both GDB and LLDB, run the
following command:
```
(gdb) p <var_name>
(lldb) p <var_name>
```

You can also show the arguments an local variables for the current stack frame. In GDB, run the following
command:
```
(gdb) info args
(gdb) info locals
```
In LLDB,
```
(lldb) frame variable
```

### Viewing the Call Stack

In GDB, you can view the call stack and move up/down stack frames to examine the state of the program in
the different frames. In GDB, the commands are as follows:
```
(gdb) backtrace # show the call stack
(gdb) up # move up one stack frame
(gdb) down # mvoe down one stack frame
```
In LLDB. the command is as follows. `up` and `down` are the same.
```
(lldb) thread backtrace
```

### Examining Threads

You can view the states of all threads in the program, as well as switch between them. In GDB, use the
following commands:
```
(gdb) info threads # list the threads
(gdb) thread <n> # switch to thread n
```
In LLDB,
```
(lldb) thread list
(lldb) thread select <n>
```

To run a command on all threads automatically (like showing the call stack), you can do the following in GDB:
```
(gdb) thread apply all backtrace
```
In LLDB,
```
(lldb) thread backtrace all
```

**Note: The project 1 infrastructure does not support these commands for viewing threads. Using `cout` is likely a more effective method to inspect the state of different threads in project 1.**
