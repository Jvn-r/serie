# Serie
Serie is a linux system analysis tool to interpret process behavior by using low-level raw system data combined with higher level analysis

built to answer "How do processes interact with files and each other to produce system behavior?"

## Core idea 
tools like `top` or `htop` show data, but they don’t really explain why things are happening or how processes interact.

hence the idea to model system activity by extracting and structuring data, from `/proc/`, and treating a process as a dynamic object with lifecycle, hierarchy, relationships and resources 

the goal is to make low-level system data easier to interpret for understanding system behavior, system monitoring, anomaly detection and pattern recognition

## Current Features
- getting set of pids 
- detecting birth and death of processes : proc lifecycle
- process metadata extraction thru `/proc/<pid>/stat/`
- recognize parent-child relationships
- visualize said parent-child relationships as a process tree with `systemd` at the root
- read file descriptors from `/proc/<pid>/fd/`
- resolve fd symlink to get actual files 

## ONGOING REFACTOR TO C
moving from quick prototyping in python to actual implementation in C to improve performance, enable deeper system integration and reduce overhead

### Current Progress
- PID's from `/proc/` using `readdir()`
- plan for lifecycle detection 

## Planned Features 
- process tree visualization improvements
- process-file interaction monitoring
- feature extraction for processes like monitoring specific processes behaviours
- anomaly or behaviour logging
- interpretation layer 
  - rule based interpretation
  - ml model for real time anomaly detection 

## Docs
processed deliverable docs in `/docs`

## Running
- python :`python /py_proto/serie.py`
- C : `gcc /c_imple/serie.c -o serie ; ./serie`

## Example output
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
