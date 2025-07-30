# A STM Simulator

A tiny game in C using Raylib & emscripten that simulates a Scanning Tip Microscope (or HardDisk reader-writer-head) acting on a single byte at high temperature!<br>
[<b>Web demo here.](https://so-groenen.github.io/STM_simulator/)

## Description:
A single bit can be encoded using many magnetic atoms, which we can imagine as tiny compasses pointing either north (+1) or south (-1).<br>
Averaging over these atoms we can thus encode a binary state!
The bits can also be protected from affecting one another by having buffer regions between them.<br> 
And by lining up bits & buffers, we can encode a whole byte! But beware! Thermal fluctuations can destroy the data!
## Goal:

1. Write down "42" in binary by sending magnetic pulses on the bright regions
2. As soon as you manage, protect the data!
## Commands Usage:
* [TAB] to switch direction of the field
* [SPACE] to send magnetic pulses.
<br>
TODO: a lot of things..