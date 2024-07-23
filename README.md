## MPV-Wrapper

> [!WARNING]
> Although the library works, has many flaws that needs to be patched, this was done in a speed-coding session

## Interface

The DLL interface has 1 unique exported function, that looks like this:

See: `include/ts_interface.hpp`

```cpp
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
```

We'll decrypt all those comments and the whole function parameters to a better understanding

Starting from the parameters, the first parameter is a `const char*`, for sake of simplicity we'll abbreviate it to CString

A CString is a null-terminated ASCII string, the first parameter is one of those, it's taking the data to send over the pipe to MPV, for example: `{"command": ["get_property", "playback-time"]}`

The second parameter is a request id introduced into the json data from the first parameter.

Taking the same example as before, introducing a request id of `1234`, the data will look like this: `{"command": ["get_property", "playback-time"], "request_id": 1234}`.

This is the real data passed to the pipe.

The third parameter is the pipe to open to send the command, this is a CString too.

An example: `\\.\pipe\mpvsocket`, in another languages could not exist a raw string literal (`R"(...)"`), so here's the same example pipe name escaping backslashes: `\\\\.\\pipe\\mpvsocket`

The fourth parameter is a bool, defaultly set to false, this is to allow raw data to pass over the pipe, doesn't check if the data is a json-valid document, but with the drawback that doesn't wait for pipe to answer, this is useful for commands like `show-text ${playback-time}`

## Building

Soon there'll be a CI/CD job to automatically build the lib for windows

### Requirements for local build

* Git (To clone this repository)
* [Xmake](https://xmake.io)

To build, first you need to configure xmake to build to your desired target platform

The only platform allowed for now is Windows 64 bits

This command also will automatically download the needed 3rd-party requirements

```cmd
REM -p: Windows -a: Architecture (x64) -m: Mode (Release)
xmake config -p windows -a x64 -m release -y
```

After that, build the DLL

```cmd
xmake
```