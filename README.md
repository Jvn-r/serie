# Serie
Serie is a linux processes analyzer, built to answer my ongoing question of How do systems work?

## Current Features
- getting set of pids 
- detecting birth and death of processes : proc lifecycle
- process metadata extraction
- recognize parent-child relationships
- visualize said parent-child relationships as a proc tree 

## Future plan 
- process tree visualization improvements
- process-file interaction monitoring
- feature extraction for processes like monitoring specific processes behaviours
- interpretation layer 
  - ml model analyzing process activity in real time 
- dynamic UI
- event logging

aiming to perform high level interpretation of system behaviour using lower level process data

which, for me, means learning low level mechanisms and high level analysis

## Docs
processed deliverable docs in `/docs`

## Running
- its a only a python file for now : >>python serie.py

## Example output
```
387 processes
1 systemd
    3138 pmlogger
    4217 systemd
        5122 gvfs-mtp-volume
        5146 gjs
        5163 tracker-miner-f
        75824 crashhelper
        5239 xdg-desktop-por
        4223 sd-pam)
        5245 xdg-desktop-por
        4233 pipewire
        4234 pipewire
```
