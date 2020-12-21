#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>

#include "timing.hpp"
#include "counting.hpp"
#include "counting_in_file.hpp"
#include "profiling.hpp"
#include "profiling_in_file.hpp"

using namespace peptalk;
using std::map;
using std::pair;
using std::runtime_error;
using std::string;
using std::vector;

void OnErrorOrWarning(const string& peptalk_msg, const string& papi_msg) {
    throw runtime_error(peptalk_msg + ": " + papi_msg);
}

/*
 * Timing
 */
void TimingInit(const string &result_file,
                const vector<string> &parameters,
                unsigned int num_measurements) {
    timing::Init(result_file, parameters, num_measurements, OnErrorOrWarning);
}

void TimingStart(const vector<string> &parameters) {
    timing::Start(parameters, OnErrorOrWarning);
}

void TimingStop() {
    timing::Stop(OnErrorOrWarning);
}

void TimingClose() {
    timing::Close(OnErrorOrWarning);
}

/*
 * Counting in file
 */
void CountingInFileInit(const string &result_file,
                  const vector<string> &parameters,
                  const vector<string> &performance_events,
                  unsigned int num_measurements) {
    counting_in_file::Init(result_file, parameters, performance_events, num_measurements, OnErrorOrWarning);
}

void CountingInFileStart(const vector<string> &parameters) {
    counting_in_file::Start(parameters, OnErrorOrWarning);
}

void CountingInFileStop() {
    counting_in_file::Stop(OnErrorOrWarning);
}

void CountingInFileClose() {
    counting_in_file::Close(OnErrorOrWarning);
}

/*
 * Profiling in memory
 */
void ProfilingInit(const vector<string> &events,
                   bool instruction_address,
                   int overflow_threshold) {
    profiling::Init(events, instruction_address, overflow_threshold, OnErrorOrWarning);
}

void ProfilingStart() {
    profiling::Start(OnErrorOrWarning);
}

void ProfilingStop() {
    profiling::Stop(OnErrorOrWarning);
}

void ProfilingCleanup() {
    profiling::Cleanup(OnErrorOrWarning);
}

vector<pair<string, vector<peptalk::measurement_type>>> ProfilingGetMeasurements() {
    vector<pair<string, vector<peptalk::measurement_type>>> dict;
    for (int profile_idx = 0; profile_idx < profiling::GetNumProfile(); ++profile_idx) {
        auto event = profiling::GetProfileEvent(profile_idx);
        auto measurements = profiling::GetProfile(profile_idx);
        dict.push_back({event, {}});
        dict[profile_idx].second.insert(dict[profile_idx].second.end(), measurements.begin(), measurements.end());
    }
    return dict;
}

/*
 * Profiling in file
 */
void ProfilingInFileInit(const string &result_file,
                   const string &overflow_event,
                   int overflow_threshold,
                   const vector<string> &performance_events,
                   bool include_inst_address) {
    profiling_in_file::Init(
            result_file,
            overflow_event,
            overflow_threshold,
            performance_events,
            include_inst_address,
            OnErrorOrWarning);
}

void ProfilingInFileStart(const string &trace_header) {
    profiling_in_file::Start(trace_header, OnErrorOrWarning);
}

void ProfilingInFileStop() {
    profiling_in_file::Stop(OnErrorOrWarning);
}

void ProfilingInFileClose() {
    profiling_in_file::Close(OnErrorOrWarning);
}

namespace py = pybind11;

PYBIND11_MODULE(peptalk_core_impl, m) {
    m.doc() = R"pbdoc(
        pep-talk library for Python
        -----------------------
        .. currentmodule:: peptalk
        .. autosummary::
            :toctree: _generate
            timing_init
            timing_start
            timing_stop
            timing_close
            counting_in_file_init
            counting_in_file_start
            counting_in_file_stop
            counting_in_file_close
            profiling_init
            profiling_start
            profiling_stop
            profiling_cleanup
            profiling_num_profile
            profiling_get_profile
            profiling_get_inst_address
            profiling_get_event
            profiling_get_measurements
            profiling_in_file_init
            profiling_in_file_start
            profiling_in_file_stop
            profiling_in_file_close
    )pbdoc";
    /*
     * Timing
     */
    m.def("timing_init", &TimingInit, R"pbdoc(
        Initialise timing module
    )pbdoc");
    m.def("timing_start", &TimingStart, R"pbdoc(
        Start timing
    )pbdoc");
    m.def("timing_stop", &TimingStop, R"pbdoc(
        Stop timing
    )pbdoc");
    m.def("timing_close", &TimingClose, R"pbdoc(
        Close timing module
    )pbdoc");
    /*
     * Counting in file
     */
    m.def("counting_in_file_init", &CountingInFileInit, R"pbdoc(
        Initialise in-file counting module
    )pbdoc");
    m.def("counting_in_file_start", &CountingInFileStart, R"pbdoc(
        Start in-file counting
    )pbdoc");
    m.def("counting_in_file_stop", &CountingInFileStop, R"pbdoc(
        Stop in-file counting
    )pbdoc");
    m.def("counting_in_file_close", &CountingInFileClose, R"pbdoc(
        Close in-file counting module
    )pbdoc");
    /*
     * Profiling in memory
     */
    m.def("profiling_init", &ProfilingInit, R"pbdoc(
        Initialise in-memory profiling module
    )pbdoc");
    m.def("profiling_start", &ProfilingStart, R"pbdoc(
        Start in-memory profiling
    )pbdoc");
    m.def("profiling_stop", &ProfilingStop, R"pbdoc(
        Stop in-memory profiling
    )pbdoc");
    m.def("profiling_cleanup", &ProfilingCleanup, R"pbdoc(
        Cleanup in-memory profiling module
    )pbdoc");
    m.def("profiling_num_profile", &profiling::GetNumProfile, R"pbdoc(
        Return number of measured profiles
    )pbdoc");
    m.def("profiling_get_profile", &profiling::GetProfile, R"pbdoc(
        Get profile measurements
    )pbdoc", py::arg("index"));
    m.def("profiling_get_inst_address", &profiling::GetInstructionAddress, R"pbdoc(
        Get address of profiled instructions
    )pbdoc");
    m.def("profiling_get_event", &profiling::GetProfileEvent, R"pbdoc(
        Get profile event
    )pbdoc", py::arg("index"));
    m.def("profiling_get_measurements", &ProfilingGetMeasurements, R"pbdoc(
        Return measurements for profiles
    )pbdoc");
    /*
     * Profiling in file
     */
    m.def("profiling_in_file_init", &ProfilingInFileInit, R"pbdoc(
        Initialise in-file profiling module
    )pbdoc");
    m.def("profiling_in_file_start", &ProfilingInFileStart, R"pbdoc(
        Start in-file profiling
    )pbdoc");
    m.def("profiling_in_file_stop", &ProfilingInFileStop, R"pbdoc(
        Stop in-file profiling
    )pbdoc");
    m.def("profiling_in_file_close", &ProfilingInFileClose, R"pbdoc(
        Close in-file profiling module
    )pbdoc");
    m.attr("__version__") = "1.0";
}
