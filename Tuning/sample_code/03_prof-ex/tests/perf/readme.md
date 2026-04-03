# General Remark
This is a sample of testing a profiler tool, `perf`. Please check whether perf is deployed in your system and environment variable PATH is properly set before testing it.

# Usage
1. First you run an executable file with perf.  You can use a script file, `run.sh`.
2. Next, you analyze the data in perf.data with `perf`. The script `run.sh` generates a report, named by `perf.out`.
3. You can also obtain annotated source files related to your performance analysis. To this end, a debug option (typically, `-g` ) is mandatory. The script file, `get_perf_ano.sh`, may be helpful for you.
