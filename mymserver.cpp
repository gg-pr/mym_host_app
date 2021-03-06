//// SERVER PROGRAM /////
 
#include <iostream>
#include <tchar.h>
#include <windows.h>
using namespace std;
 
HANDLE pipe;


//int host_write_to_pipe(int msg_id, char *data, HANDLE pipe)
int host_write_to_pipe(int msg_id, char *data)
{
    cout << "Waiting for a client to connect to the pipe..." << endl;
     
    // This call blocks until a client process connects to the pipe
    BOOL result = ConnectNamedPipe(pipe, NULL);
    if (!result) {
        cout << "Failed to make connection on named pipe." << endl;
        // look up error code here using GetLastError()
        //CloseHandle(pipe); // close the pipe
        system("pause");
        return 1;
    }

    cout << "Sending data to pipe..." << endl;
     
    // This call blocks until a client process reads all the data
    //const wchar_t *data = L"*** Hello Pipe World ***";
    DWORD numBytesWritten = 0;
    result = WriteFile(
                       pipe, // handle to our outbound pipe
                       data, // data to send
                       //wcslen(data) * sizeof(wchar_t), // length of data to send (bytes)
                       strlen(data),
                       &numBytesWritten, // will store actual amount of data sent
                       NULL // not using overlapped IO
             );
     
    if (result) {
        cout << "Number of bytes sent: " << numBytesWritten << endl;
    } else {
        cout << "Failed to send data." << endl;
            // look up error code here using GetLastError()
    }
        // Close the pipe (automatically disconnects client too)
}

//int host_create_pipe(HANDLE *pipe)
int host_create_pipe()
{
	
    cout << "Creating an instance of a named pipe..." << endl;
     
        // Create a pipe to send data
     pipe = CreateNamedPipe(
            //L"\\\\.\\pipe\\my_pipe", // name of the pipe
			L"\\\\.\\pipe\\mypipe",
			PIPE_ACCESS_OUTBOUND, // 1-way pipe -- send only
			//PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE, // send data as a byte stream
            1, // only allow 1 instance of this pipe
            0, // no outbound buffer
            0, // no inbound buffer
            0, // use default wait time
            NULL // use default security attributes
        );
     
        if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
            cout << "Failed to create outbound pipe instance.";
            // look up error code here using GetLastError()
            system("pause");
            return 1;
        }

		/*
        wcout << "Waiting for a client to connect to the pipe..." << endl;
     
        // This call blocks until a client process connects to the pipe
        BOOL result = ConnectNamedPipe(pipe, NULL);
        if (!result) {
            wcout << "Failed to make connection on named pipe." << endl;
            // look up error code here using GetLastError()
            CloseHandle(pipe); // close the pipe
            system("pause");
            return 1;
        }
     
        wcout << "Sending data to pipe..." << endl;
     
        // This call blocks until a client process reads all the data
        const wchar_t *data = L"*** Hello Pipe World ***";
        DWORD numBytesWritten = 0;
        result = WriteFile(
            pipe, // handle to our outbound pipe
            data, // data to send
            wcslen(data) * sizeof(wchar_t), // length of data to send (bytes)
            &numBytesWritten, // will store actual amount of data sent
            NULL // not using overlapped IO
        );
     
        if (result) {
            wcout << "Number of bytes sent: " << numBytesWritten << endl;
        } else {
            wcout << "Failed to send data." << endl;
            // look up error code here using GetLastError()
        }
        // Close the pipe (automatically disconnects client too)
 //       CloseHandle(pipe);
     
 //   }
 */
 
    cout << "Pipe create Done." << endl;
 
   // system("pause");
    return 0;
}
