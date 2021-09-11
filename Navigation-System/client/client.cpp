// ---------------------------------------------------
// Name: Daniel Sacro
// CMPUT 275, Winter 2021
//
// Major Assignment 1: Navigation System (Individual)
// ---------------------------------------------------
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <string>

#define BUFFER_SIZE 1024
#define MSG_SIZE 1024

using namespace std;

int create_and_open_fifo(const char *pname, int mode) {
    // creating a fifo special file in the current working directory
    // with read-write permissions for communication with the plotter
    // both processes must open the fifo before they can perform
    // read and write operations on it
    if (mkfifo(pname, 0666) == -1) {
        cout << "Unable to make a fifo. Ensure that this pipe does not exist already!" << endl;
        exit(-1);
    }

    // opening the fifo for read-only or write-only access
    // a file descriptor that refers to the open file description is
    // returned
    int fd = open(pname, mode);

    if (fd == -1) {
        cout << "Error: failed on opening named pipe." << endl;
        exit(-1);
    }

    return fd;
}

int main(int argc, char const *argv[]) {
    const char *inpipe = "inpipe";
    const char *outpipe = "outpipe";

    int in = create_and_open_fifo(inpipe, O_RDONLY);
    cout << "inpipe opened..." << endl;
    int out = create_and_open_fifo(outpipe, O_WRONLY);
    cout << "outpipe opened..." << endl;

    // Student code starts here ---------------------------------------------------------

    // Socket and pipe creation and communcation implemented using code from lec. 16
    struct sockaddr_in my_addr, peer_addr;
    ;
    memset(&my_addr, '\0', sizeof my_addr);
    int socket_desc;
    char inbound[BUFFER_SIZE] = {};

    // Build socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        cerr << "Listening socket creation failed!\n";
        return 1;
    }

    int PORT = atoi(argv[1]);
    const char *SERVER_IP = argv[2];

    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(PORT);
    peer_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&peer_addr, sizeof peer_addr) == -1) {
        cerr << "Cannot connect to the host!\n";
        close(socket_desc);
        return 1;
    }

    char line1[MSG_SIZE];
    char line2[MSG_SIZE];
    while (true) {
        // Make sure buffer/line is empty first.
        memset(line1, 0, MSG_SIZE);
        memset(line2, 0, MSG_SIZE);

        // Read start point from inpipe

        int bytesread1 = read(in, line1, MSG_SIZE);
        if (strcmp("Q\n", line1) == 0) {
            send(socket_desc, line1, MSG_SIZE, 0);
            break;
        }

        // Read end point from inpipe
        int bytesread2 = read(in, line2, MSG_SIZE);
        if (strcmp("Q\n", line2) == 0) {
            send(socket_desc, line2, MSG_SIZE, 0);
            break;
        }

        if (bytesread1 == -1 || bytesread2 == -1)
            cerr << "Error: read operation failed!" << endl;

        // Prepare to send start and end points to server as a request
        vector<string> lVec1, lVec2;
        string tempStr;
        for (int i = 0; i < MSG_SIZE; i++) {
            if (line1[i] == ' ') {
                lVec1.push_back(tempStr);
                tempStr.erase();
            }
            else if (line1[i] == '\n') {
                // Assume newline is the end of the string
                lVec1.push_back(tempStr);
                break;
            }
            else {
                tempStr += line1[i];
            }
        }

        tempStr = "";
        for (int j = 0; j < MSG_SIZE; j++) {
            if (line2[j] == ' ') {
                lVec2.push_back(tempStr);
                tempStr.erase();
            }
            else if (line2[j] == '\n') {
                // Assume newline is the end of the string
                lVec2.push_back(tempStr);
                break;
            }
            else {
                tempStr += line2[j];
            }
        }

        // Server requires the coordinates in degrees of 100000
        long long tempLong;
        tempLong = static_cast<long long>(stod(lVec1[0]) * 100000);
        lVec1[0] = to_string(tempLong);
        tempLong = static_cast<long long>(stod(lVec1[1]) * 100000);
        lVec1[1] = to_string(tempLong);
        tempLong = static_cast<long long>(stod(lVec2[0]) * 100000);
        lVec2[0] = to_string(tempLong);
        tempLong = static_cast<long long>(stod(lVec2[1]) * 100000);
        lVec2[1] = to_string(tempLong);

        // Create request and send it to server
        string clientRequest = "R " + lVec1[0] + " " + lVec1[1] + " " + lVec2[0] + " " + lVec2[1] + "\n";
        send(socket_desc, clientRequest.c_str(), clientRequest.length() + 1, 0);

        // Wait and receive the total number of waypoints (Format: "N #")
        int rec_size = recv(socket_desc, inbound, BUFFER_SIZE, 0);

        string ack = "A";
        string exitStr = "E\n";
        // Error if exitStr is sent as a string. Needs to be a char. array
        char endStr[exitStr.length()];
        strcpy(endStr, exitStr.c_str());
        while (true) {
            // If server finds no waypoints, wait for new points from plotter
            if (strcmp("N 0", inbound) == 0) {
                // write E\n to the plotter
                write(out, endStr, sizeof endStr);
                break;
            }

            // Otherwise if there are waypoints, send acknowledgement and begin reading
            send(socket_desc, ack.c_str(), ack.length() + 1, 0);
            rec_size = recv(socket_desc, inbound, BUFFER_SIZE, 0);

            // Split waypoint string into its coordinates
            // Has little to no effect if "E\n" is received
            tempStr = "";
            vector<string> WPvec;
            for (int l = 0; l < BUFFER_SIZE; l++) {
                if (inbound[l] == ' ') {
                    WPvec.push_back(tempStr);
                    tempStr.erase();
                }
                else if (inbound[l] == '\n') {
                    // Assume newline is the end of the string
                    WPvec.push_back(tempStr);
                    break;
                }
                else {
                    tempStr += inbound[l];
                }
            }

            // No waypoints to relay if "E\n" was received
            if (WPvec[0] == "E") {
                write(out, endStr, sizeof endStr);
                break;
            }

            // Prepare for waypoint coordinates to be sent to plotter
            long long llLat = stoll(WPvec[0]);
            long long llLon = stoll(WPvec[1]);
            double doubLat = static_cast<double>(llLat) / 100000;
            double doubLon = static_cast<double>(llLon) / 100000;
            string strLat = to_string(doubLat);
            string strLon = to_string(doubLon);

            // Create waypoint string to be sent to plotter
            string WPstr = strLat + " " + strLon + "\n";

            // Error if string is sent. Must convert char. array
            char outputStr[WPstr.length()];
            strcpy(outputStr, WPstr.c_str());
            write(out, outputStr, sizeof outputStr);
        }
    }
    cout << "inpipe closed...\n";
    cout << "outpipe closed...\n";
    close(socket_desc);
    // Student code ends here ------------------------------------------------------------

    close(in);
    close(out);
    unlink(inpipe);
    unlink(outpipe);
    return 0;
}
