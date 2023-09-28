#!/usr/bin/bash
#sudo perf record -g  -e"cpu-clock" ./concurrency/condition_variable/cv_rwlock 
#sudo perf record -g  -e"cpu-clock" -t 10327
sudo perf script -i perf.data &> perf.unfold
sudo ~/workspace/FlameGraph/stackcollapse-perf.pl perf.unfold &> perf.folded
sudo ~/workspace/FlameGraph/flamegraph.pl perf.folded > perf.svg

