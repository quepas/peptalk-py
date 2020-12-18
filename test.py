import peptalk

peptalk.timing_init("execution_time.csv", ["test"], 1)
peptalk.timing_start(["no_instructions"])
peptalk.timing_stop()
peptalk.timing_close()

peptalk.counting_in_file_init("performance_counters.csv", ["test"], ["INST_RETIRED:ANY_P", "CPU_CLK_UNHALTED:THREAD_P"], 1)
peptalk.counting_in_file_start(["no_instructions"])
peptalk.counting_in_file_stop()
peptalk.counting_in_file_close()

peptalk.profiling_in_file_init("performance_profiles.pep", "INST_RETIRED:ANY_P", 2000, ["CPU_CLK_UNHALTED:THREAD_P"], True)
peptalk.profiling_in_file_start("no_instructions")
peptalk.profiling_in_file_stop()
peptalk.profiling_in_file_close()

peptalk.profiling_init(["INST_RETIRED:ANY_P", "CPU_CLK_UNHALTED:THREAD_P", "L1D:REPLACEMENT"], True, 1000)
peptalk.profiling_start()
peptalk.profiling_stop()
print("Profiles={}".format(peptalk.profiling_get_measurements()))
print("Inst address={}".format(peptalk.profiling_get_inst_address()))
peptalk.profiling_cleanup()


