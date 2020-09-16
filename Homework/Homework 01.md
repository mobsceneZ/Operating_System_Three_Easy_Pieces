# Homework 01

Running On MacOS Catalina( 10.15.5 )

**A1**:

```sh
prompt> ./process-run.py -l 5:100,5:100 -c -p
Time    PID: 0    PID: 1       CPU       IOs
  1    RUN:cpu     READY         1          
  2    RUN:cpu     READY         1          
  3    RUN:cpu     READY         1          
  4    RUN:cpu     READY         1          
  5    RUN:cpu     READY         1          
  6       DONE   RUN:cpu         1          
  7       DONE   RUN:cpu         1          
  8       DONE   RUN:cpu         1          
  9       DONE   RUN:cpu         1          
 10       DONE   RUN:cpu         1          

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

Since all instructions are CPU instructions, it should be 100% CPU utilization.

**A2**:

it should be ( 1 + 4 + 1 ) + 4 = 10. (actually I think the 10th is unnesessary... )

```sh
prompt> ./process-run.py -l 4:100,1:0 -c -p
Time    PID: 0    PID: 1       CPU       IOs
  1    RUN:cpu     READY         1          
  2    RUN:cpu     READY         1          
  3    RUN:cpu     READY         1          
  4    RUN:cpu     READY         1          
  5       DONE    RUN:io         1          
  6       DONE   WAITING                   1
  7       DONE   WAITING                   1
  8       DONE   WAITING                   1
  9       DONE   WAITING                   1
 10*      DONE      DONE         1

Stats: Total Time 10
Stats: CPU Busy 6 (60.00%)
Stats: IO Busy  4 (40.00%)
```

**A3**:

Amazing, it matters!

When one process goes into blocked state, the other one which is ready can occupy the CPU.

```sh
prompt> ./process-run.py -l 1:0,4:100 -c -p
Time    PID: 0    PID: 1       CPU       IOs
  1     RUN:io     READY         1          
  2    WAITING   RUN:cpu         1         1
  3    WAITING   RUN:cpu         1         1
  4    WAITING   RUN:cpu         1         1
  5    WAITING   RUN:cpu         1         1
  6*      DONE      DONE         1

Stats: Total Time 6
Stats: CPU Busy 6 (100.00%)
Stats: IO Busy  4 (66.67%)
```

**A4**:

it should look like results in A2, with I/O executed first.

```sh
prompt> ./process-run.py -l 1:0,4:100 -S SWITCH_ON_END -c
Time    PID: 0    PID: 1       CPU       IOs
  1     RUN:io     READY         1          
  2    WAITING     READY                   1
  3    WAITING     READY                   1
  4    WAITING     READY                   1
  5    WAITING     READY                   1
  6*      DONE   RUN:cpu         1          
  7       DONE   RUN:cpu         1          
  8       DONE   RUN:cpu         1          
  9       DONE   RUN:cpu         1          
```

**A5**:

it should look like results in A3.

```sh
prompt> ./process-run.py -l 1:0,4:100 -S SWITCH_ON_IO -c
Time    PID: 0    PID: 1       CPU       IOs
  1     RUN:io     READY         1          
  2    WAITING   RUN:cpu         1         1
  3    WAITING   RUN:cpu         1         1
  4    WAITING   RUN:cpu         1         1
  5    WAITING   RUN:cpu         1         1
  6*      DONE      DONE         1
```

**A6**:

What I think should be: 

P1I1 runs(issue an I/O) --> P1I1 continues, P2I(1-4) runs --> P1I1 done, P2I5 runs --> P2 done, P1I2 runs(issue an I/O) --> P1I2 continues, P3I(1-4) runs --> P1I2 done, P3I5 runs --> P3 done, P1I3 runs(issue an I/O) --> P1I3 continues, P4I(1-4) runs --> P1I3 done, P4I5 runs --> P4 done, P1 done.

But the fact is... The system resources are not effectively utilized.

```sh
prompt> ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p
Time    PID: 0    PID: 1    PID: 2    PID: 3       CPU       IOs
  1     RUN:io     READY     READY     READY         1          
  2    WAITING   RUN:cpu     READY     READY         1         1
  3    WAITING   RUN:cpu     READY     READY         1         1
  4    WAITING   RUN:cpu     READY     READY         1         1
  5    WAITING   RUN:cpu     READY     READY         1         1
  6*     READY   RUN:cpu     READY     READY         1          
  7      READY      DONE   RUN:cpu     READY         1          
  8      READY      DONE   RUN:cpu     READY         1          
  9      READY      DONE   RUN:cpu     READY         1          
 10      READY      DONE   RUN:cpu     READY         1          
 11      READY      DONE   RUN:cpu     READY         1          
 12      READY      DONE      DONE   RUN:cpu         1          
 13      READY      DONE      DONE   RUN:cpu         1          
 14      READY      DONE      DONE   RUN:cpu         1          
 15      READY      DONE      DONE   RUN:cpu         1          
 16      READY      DONE      DONE   RUN:cpu         1          
 17     RUN:io      DONE      DONE      DONE         1          
 18    WAITING      DONE      DONE      DONE                   1
 19    WAITING      DONE      DONE      DONE                   1
 20    WAITING      DONE      DONE      DONE                   1
 21    WAITING      DONE      DONE      DONE                   1
 22*    RUN:io      DONE      DONE      DONE         1          
 23    WAITING      DONE      DONE      DONE                   1
 24    WAITING      DONE      DONE      DONE                   1
 25    WAITING      DONE      DONE      DONE                   1
 26    WAITING      DONE      DONE      DONE                   1
 27*      DONE      DONE      DONE      DONE         1

Stats: Total Time 27
Stats: CPU Busy 19 (70.37%)
Stats: IO Busy  12 (44.44%)
```

**A7**:

```sh
prompt> ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -c -p
Time    PID: 0    PID: 1    PID: 2    PID: 3       CPU       IOs
  1     RUN:io     READY     READY     READY         1          
  2    WAITING   RUN:cpu     READY     READY         1         1
  3    WAITING   RUN:cpu     READY     READY         1         1
  4    WAITING   RUN:cpu     READY     READY         1         1
  5    WAITING   RUN:cpu     READY     READY         1         1
  6*    RUN:io     READY     READY     READY         1          
  7    WAITING   RUN:cpu     READY     READY         1         1
  8    WAITING      DONE   RUN:cpu     READY         1         1
  9    WAITING      DONE   RUN:cpu     READY         1         1
 10    WAITING      DONE   RUN:cpu     READY         1         1
 11*    RUN:io      DONE     READY     READY         1          
 12    WAITING      DONE   RUN:cpu     READY         1         1
 13    WAITING      DONE   RUN:cpu     READY         1         1
 14    WAITING      DONE      DONE   RUN:cpu         1         1
 15    WAITING      DONE      DONE   RUN:cpu         1         1
 16*      DONE      DONE      DONE   RUN:cpu         1          
 17       DONE      DONE      DONE   RUN:cpu         1          
 18       DONE      DONE      DONE   RUN:cpu         1          

Stats: Total Time 18
Stats: CPU Busy 18 (100.00%)
Stats: IO Busy  12 (66.67%)
```

This behavior performances far better than behavior specified in A6.

Beacause it may request another I/O issues again.

**A8**:

不想写了O.o



 



