#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <ableton/Link.hpp>

namespace py = pybind11;

PYBIND11_MODULE(link_wrapper, m)
{
     m.doc() = "Python bindings for Ableton Link"; // Module docstring

     // Expose the main Link class
     py::class_<ableton::Link>(m, "Link")
         .def(py::init<double>(), py::arg("bpm"),
              "Construct a new Link instance with the given BPM")
         .def("enable", &ableton::Link::enable, py::arg("enabled"),
              "Enable/disable Link network functionality")
         .def("is_enabled", &ableton::Link::isEnabled,
              "Check if Link is currently enabled")
         .def("enable_start_stop_sync", &ableton::Link::enableStartStopSync, py::arg("enabled"),
              "Enable/disable start/stop synchronization")
         .def("is_start_stop_sync_enabled", &ableton::Link::isStartStopSyncEnabled,
              "Check if start/stop synchronization is enabled")
         .def("num_peers", &ableton::Link::numPeers,
              "Get the number of peers in the current Link session")
         .def("set_num_peers_callback", [](ableton::Link &link, py::function callback)
              { link.setNumPeersCallback([callback](std::size_t numPeers)
                                         {
                py::gil_scoped_acquire acquire;
                callback(numPeers); }); }, "Set callback for peer count changes")
         .def("set_tempo_callback", [](ableton::Link &link, py::function callback)
              { link.setTempoCallback([callback](double bpm)
                                      {
                py::gil_scoped_acquire acquire;
                callback(bpm); }); }, "Set callback for tempo changes")
         .def("set_start_stop_callback", [](ableton::Link &link, py::function callback)
              { link.setStartStopCallback([callback](bool isPlaying)
                                          {
                py::gil_scoped_acquire acquire;
                callback(isPlaying); }); }, "Set callback for start/stop state changes")
         .def("capture_audio_session_state", &ableton::Link::captureAudioSessionState, "Capture the current Link session state from the audio thread")
         .def("commit_audio_session_state", &ableton::Link::commitAudioSessionState, py::arg("state"), "Commit the given session state from the audio thread")
         .def("capture_app_session_state", &ableton::Link::captureAppSessionState, "Capture the current Link session state from an application thread")
         .def("commit_app_session_state", &ableton::Link::commitAppSessionState, py::arg("state"), "Commit the given session state from an application thread");

     // Expose the SessionState class
     py::class_<ableton::Link::SessionState>(m, "SessionState")
         .def("tempo", &ableton::Link::SessionState::tempo,
              "Get the current tempo in BPM")
         .def("set_tempo", &ableton::Link::SessionState::setTempo,
              py::arg("bpm"), py::arg("at_time"),
              "Set the tempo at the specified time")
         .def("beat_at_time", &ableton::Link::SessionState::beatAtTime,
              py::arg("time"), py::arg("quantum"),
              "Get the beat value at the given time for the specified quantum")
         .def("phase_at_time", &ableton::Link::SessionState::phaseAtTime,
              py::arg("time"), py::arg("quantum"),
              "Get the phase at the given time for the specified quantum")
         .def("time_at_beat", &ableton::Link::SessionState::timeAtBeat,
              py::arg("beat"), py::arg("quantum"),
              "Get the time at which the given beat occurs")
         .def("request_beat_at_time", &ableton::Link::SessionState::requestBeatAtTime,
              py::arg("beat"), py::arg("time"), py::arg("quantum"),
              "Request to map the given beat to the specified time")
         .def("force_beat_at_time", &ableton::Link::SessionState::forceBeatAtTime,
              py::arg("beat"), py::arg("time"), py::arg("quantum"),
              "Force map the given beat to the specified time")
         .def("set_is_playing", &ableton::Link::SessionState::setIsPlaying,
              py::arg("is_playing"), py::arg("time"),
              "Set the playing state at the specified time")
         .def("is_playing", &ableton::Link::SessionState::isPlaying,
              "Check if transport is currently playing")
         .def("time_for_is_playing", &ableton::Link::SessionState::timeForIsPlaying,
              "Get the time at which the current playing state was set")
         .def("request_beat_at_start_playing_time",
              &ableton::Link::SessionState::requestBeatAtStartPlayingTime,
              py::arg("beat"), py::arg("quantum"),
              "Request to map the given beat to the start playing time")
         .def("set_is_playing_and_request_beat_at_time",
              &ableton::Link::SessionState::setIsPlayingAndRequestBeatAtTime,
              py::arg("is_playing"), py::arg("time"), py::arg("beat"), py::arg("quantum"),
              "Set playing state and request beat at time in one operation");
}