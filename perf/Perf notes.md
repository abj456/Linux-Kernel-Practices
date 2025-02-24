# Linux Kernel Practices - Perf notes

[![hackmd-github-sync-badge](https://hackmd.io/yrTYDj3PRPKzUJcsRoLkiw/badge)](https://hackmd.io/yrTYDj3PRPKzUJcsRoLkiw)

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
* 虛擬機若無模擬PMU則無法收集部份仰賴hardware performance counter的資料

## 使用 perf_events 分析程式效能
* perf_events分為三部份：
    * 在user space的`perf`命令
    * kernel space負責處理和儲存事件紀錄的機制
    * kernel space的事件來源：PMU, interrupts, page fault, system call, KProbe, UProbe, FTrace, Trace Point, etc.
* CPU-bound workload: 最常使用的事件來源為PMU。
* IO-bound workload / share specific resource(e.g. mutex, semaphore): 建議使用KProbe, UProbe, FTrace, Trace Point等來源。

## 常用命令概覽
* `perf list`
    * 列出 Linux 核心與硬體支援的事件種 類。
* `perf stat [-e [events]] [cmd]`
    * 執行指定程式。執行結束後，印出各 事件的總體統計數據。(選項`-e`用以指定要測量的事件種類。各事件種類以`,`分隔。)
* `perf record [-e [events]] [-g] [cmd]`
    * 執行指定程式。執行過程中，取樣並記錄各事件。這個命令能讓我們能大略知道事件的發生地點。(選項`-e`用以指定要測量的事件種類。各事件種類以`,`分隔；選項`-g`讓`perf record`在記錄事件時，同時記錄取樣點的Stack Trace；選項`-o`用以指定一個檔案名稱作為事件記錄儲存位置。預設值為`perf.data`)
* `perf report [-g graph,0.5,caller]`
    * 讀取 perf record 的記錄。(選項`-g`用以指定樹狀圖的繪製方式。`graph,0.5,caller`代表以 呼叫者（Caller）作為親代節點，以被呼叫者（Callee）作為子節點。這是最新版`perf`命令的預設值；選項 `i`用以指定一個檔案名稱作為事件記錄讀取來源。預設值為`perf.data`。)
* 若測量標的沒有安全疑慮，以上所有命令都建議透過`sudo`執行。

## 簡介 perf_events 與 Call Graph
