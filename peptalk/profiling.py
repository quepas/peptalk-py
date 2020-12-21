import peptalk_core_impl
import matplotlib.pyplot as plt
import numpy as np
from typing import List


def start(events: List[str], count_instruction_address: bool, sampling_threshold: int) -> None:
    peptalk_core_impl.profiling_init(events, count_instruction_address, sampling_threshold)
    peptalk_core_impl.profiling_start()


def stop_and_print():
    peptalk_core_impl.profiling_stop()
    measurements = peptalk_core_impl.profiling_get_measurements()
    peptalk_core_impl.profiling_cleanup()
    plot_profiles(measurements)


def plot_profiles(measurements):
    # Measurements has at least 2 elements (overflow event, and 1 measured event)
    num_profiles = len(measurements)
    fig, subplots = plt.subplots(num_profiles - 1, 1)
    fig.suptitle('Performance event profiles')
    x = measurements[0][1]
    for k in range(1, num_profiles):
        y = [measurements[k][1][0]]
        y.extend(np.diff(measurements[k][1]).tolist())
        subplots[k - 1].set_ylabel(measurements[k][0])
        subplots[k - 1].plot(x, y)
    subplots[num_profiles - 2].set_xlabel(measurements[0][0])
    plt.show()