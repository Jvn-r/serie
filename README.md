# Serie
Serie is a linux system analysis tool to interpret process behavior by using low-level raw system data combined with higher level analysis

built to answer "How do processes interact with files and each other to produce system behavior?"

## Core idea 
tools like `top` or `htop` show data, but they don’t really explain why things are happening or how processes interact.

hence the idea to model system activity by extracting and structuring data, from `/proc/`, and treating a process as a dynamic object with lifecycle, hierarchy, relationships and resources 

the goal is to make low-level system data easier to interpret for understanding system behavior, system monitoring, anomaly detection and pattern recognition

## Python Prototype Feats
- getting set of pids 
- detecting birth and death of processes : proc lifecycle
- process metadata extraction thru `/proc/<pid>/stat/`
- recognize parent-child relationships
- visualize said parent-child relationships as a process tree with `systemd` at the root
- read file descriptors from `/proc/<pid>/fd/`
- resolve fd symlink to get actual files 

## ONGOING REFACTOR TO C
moving from quick prototyping in python to actual implementation in C to enable deeper system integration and improve speed

## Current C Implementation
- PID extraction from `/proc/`
- lifecycle snapshot framework
- process hash table : O(1) PID lookup
- collision handling thru bucket chaining
- process ownership and memory management
- process tree implementation plus visualization
- proc connector implemented
- proc fs abstraction layer
- fork event handled
- added SIGINT handler
- exit event handled

## Architecture
### Process Table
- hash table for fast PID lookup
- bucket chaining for collision handling
### Process Tree
- parent-child hierarchy
- process traversal and visualization
### Event Layer
- proc connector (`proc_conn`)
- fork, exec and exit events
### Logging Layer
- periodic lifecycle flushing
- future analytics dataset generation

## Planned Features 
- process tree implementation
- proc connector integration
- lifecycle event tracking
- process tree visualization
- process-file interaction monitoring
- feature extraction for processes like monitoring specific processes behaviours
- anomaly or behaviour logging
- interpretation layer 
  - rule based interpretation
  - ml model for real time anomaly detection 

## Docs
processed deliverable docs in `/docs`
### Refactor docs
- DELI 6 - Python to C refactor
- DELI 7 - Lifecycle snapshot framework
- DELI 8 - Event architecture & proc connector research
- DELI 9 - Process hash table implementation
- DELI 10 - Process tree relationships
- DELI 11 - Proc Conn Integ 
- DELI 12 - ProcFS Integ
- DELI 13 - Mem Cleanups and Exit Integ

## Running
- python :`python /py_proto/serie.py`
- C : `gcc -Wall -Wextra main.c ser_proc.c -o serie; ./serie`
- proc conn : `gcc -Wall -Wextra tests/main.c c_core/ser_proc_conn.c -o test ;./test`

## Python Prototype Example output
```
387 processes
1 systemd
    3138 pmlogger
    4217 systemd
        5122 gvfs-mtp-volume
        5146 gjs
        5163 tracker-miner-f
        5245 xdg-desktop-por
        4233 pipewire
        4234 pipewire
```

## Proc Conn Example Output with FORK and EXIT:
```bash
 EXIT EXIT EXIT EXIT EXIT EXIT
PID     : 673830
Name    : pool-obsidian
Exit    : 0
Alive   : 0
Parent: -1
EVENT: 1

[FORK]

 HOLY SHIT FORK COMPETED 
Parent  : 478391
Child   : 673967
Name    : StreamT~s #1928
State   : S
FDs     : 50
EVENT: 512
[COMM] Process PID: 478660 changed name to: StreamT~s #1928
EVENT: 512
[COMM] Process PID: 478660 changed name to: StreamT~s #1928
EVENT: 2147483648

 EXIT EXIT EXIT EXIT EXIT EXIT
PID     : 673690
Name    : Isolated Web Co
Exit    : 0
Alive   : 0
Parent: -1
EVENT: 2147483648
```

- the unhandled fork's are short lifespan processes like cat, which die before i can read the procfs for data
