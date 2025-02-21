# Linux Kernel Practices - Perf notes
> [time=2025,02,22]
> [為什麼該接觸 GNU/Linux 開發工具](https://hackmd.io/@sysprog/gnu-linux-dev/https%3A%2F%2Fhackmd.io%2F%40sysprog%2Flinux-perf)

## Introduction
* Perf: Performance Event
    * user可藉PMU(Performance Monitoring Unit)、tracepoint，以及kernel內部的counter來收集效能資料與分析運行中的kernel code。
    * 相比於OProfile & GProf，Perf的優勢在於與Linux Kernel的緊密結合，並能充分利用Kernel的最新功能。
    * 原理為透過對target進行取樣，在每次tick中斷時觸發取樣點，並紀錄process在該時間點的context。
    * Perf支援多種事件的取樣
        * hardware events: cpu-cycles, instructions, cache-misses, branch-misses等
        * software events: page-faults, context-switches等
        * Tracepoint events
    * 可利用這些資料來深入分析程式效能問題。
        * instruction per cycle: cpu
        * cache-misses: locality of reference
        * branch-misses: pipeline hazard