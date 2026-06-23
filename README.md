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
moving from quick prototyping in python to actual implementation in C to improve performance, enable deeper system integration and reduce overhead

## Current C Implementation
- PID extraction from `/proc/`
- lifecycle snapshot framework
- process hash table : O(1) PID lookup
- collision handling thru bucket chaining
- process ownership and memory management
- process tree implementation plus visualization
- proc connector implemented

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

## Proc Conn Example Output
```bash
```bash
[EXEC] Process PID: 535480 changed binaries
EVENT: 1
[FORK] Parent PID: 478391 -> Child PID: 511938
EVENT: 512
[COMM] Process PID: 511938 changed name to: StreamT~ns #247
EVENT: 512
[COMM] Process PID: 511938 changed name to: StreamT~ns #247
EVENT: 2147483648
[EXIT] Process PID: 478120 exited with code: 0
```
