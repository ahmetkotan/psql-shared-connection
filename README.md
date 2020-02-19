# Multi Processing on PostgreSQL
Multi processing on Postgresql with fork() and libpq

## Read on medium
[https://medium.com/@ahmetkotan/multi-processing-on-postgresql-fork-and-libpq-3eb95b485fbe](https://medium.com/@ahmetkotan/multi-processing-on-postgresql-fork-and-libpq-3eb95b485fbe)

## Benchmark Results
|  | Shared Connection | New Connection |
| -- |--|--|
| 10 Process | 0m 10s 578ms | 0m 19s 825ms
| 50 Process | 1m 18s 684ms | 1m 57s 139ms
