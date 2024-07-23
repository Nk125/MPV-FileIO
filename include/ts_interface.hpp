#pragma once
#ifdef MPV_WRAPPER_EXPORTS
#define MPV_WRAPPER __declspec(dllexport)
#else
#define MPV_WRAPPER __declspec(dllimport)
#endif

/**
 * @brief Sends raw data to the MPV pipe, normally a json command 
 * @param Data The data to send, it's a CString
 * @param Request_ID The request ID to expect to return from the pipe
 * @param Pipe_name The name of the pipe to connect
 * @param Is_Raw_Command If true, doesn't expect any response from the pipe, by default false
 * @return The json response for the command, this CString is empty if the command failed or if it's a Raw Command
 * @note The request_id must be added to get the right event response
 * @details Why raw commands doesn't expect responses: https://github.com/mpv-player/mpv/blob/master/DOCS/man/ipc.rst#socat-example
 * @details Why Request_ID must be defined appropiately: https://github.com/mpv-player/mpv/blob/master/DOCS/man/ipc.rst#protocol
 */
extern "C" MPV_WRAPPER const char* sendMPV(const char*, long long, const char* = R"(\\.\pipe\mpvsocket)", bool = false);