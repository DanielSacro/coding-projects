// ---------------------------------------------------
// Name: Daniel Sacro
// CMPUT 275, Winter 2021
//
// Major Assignment 1: Navigation System (Individual)
// ---------------------------------------------------
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <list>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "digraph.h"
#include "wdigraph.h"
#include "dijkstra.h"
#include <sys/types.h>
#include <cstring>
#include <sys/time.h>
#include <netdb.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>

#define LISTEN_BACKLOG 50
#define BUFFER_SIZE 1024

using namespace std;

struct Point {
  /*  Description: A structure that holds the latitude and longitude
         coordinates of a given vertex point.

    Contains: 
    lat (long long int): Latitude of the point
    lon (long long int): Longitude of the point

*/
  long long lat;
  long long lon;
};

long long mathattan(const Point &pt1, const Point &pt2) {
  /*  Description: Calculates the mathattan distance between 2
           given points.

      Arguments: 
        pt1 (reference to Point struct): The first point
        pt2 (reference to Point struct): The second point
  
      Returns:
        totDist (long long int): The mathattan distance between 2 given points
  */
  long long latDist = llabs(pt1.lat - pt2.lat);
  long long lonDist = llabs(pt1.lon - pt2.lon);
  long long totDist = latDist + lonDist;
  return totDist;
}

int findClosest(const Point &pt, const unordered_map<int, Point> &points) {
  /*  Description: Finds the vertex closest to any given reference point

      Arguments:
        pt (reference to Point struct): The given reference point
        points (reference to unordered_map of int-Point struct pair):
          -> A list containing every vertex and their points

      Returns:
        closestV (int): The closest vertex to the reference point
  */
  // Shortest distance is at the beginning of pointsDist since
  // a map is ordered and the keys are distances
  map<long long, int> pointsDist;

  // Compute the distance between pt and all vertices on the map
  for (auto it = points.begin(); it != points.end(); it++) {
    long long dist = mathattan(pt, it->second);
    pointsDist[dist] = it->first;
  }

  // Return vertex closest to the given point
  int closestV = pointsDist.begin()->second;
  return closestV;
}

void readGraph(string filename, WDigraph &graph, unordered_map<int, Point> &points) {
  /*  Description: Reads inputs from a text file and constructs a
        directed graph using these inputs and an empty graph

      Arguments:
        filename (string): Name of the text file containing the inputs
        graph (reference to WDigraph object): An empty graph
        points (reference to unordered map of int-Point struct pair):
        -> A list containing every vertex and their points

       Returns: Nothing.
  */
  ifstream file(filename);
  string inputLine;

  // read the text file line by line
  while (getline(file, inputLine)) {
    string mode = inputLine.substr(0, 1);
    // str1 is inputLine with the mode removed
    string str1 = inputLine.substr(2, inputLine.length() - 2);

    if (mode == "V") {
      // read and separate inputs
      int com1 = str1.find(",");
      // vStr is the vertex v
      string vStr = str1.substr(0, com1);

      // str2 contains only the latitude and longitude of the vertex
      string str2 = str1.substr(com1 + 1, str1.length() - com1 - 1);
      int com2 = str2.find(",");
      string latStr = str2.substr(0, com2);

      // str3 is only the longitude of the vertex
      string str3 = str2.substr(com2 + 1, str2.length() - com2 - 1);
      string lonStr = str3;

      // Convert and store vertex
      int v = stoi(vStr);
      graph.addVertex(v);

      // Convert and store lat and lon coords of vertex
      double coord1 = stod(latStr);
      double coord2 = stod(lonStr);
      long long lat = static_cast<long long>(coord1 * 100000);
      long long lon = static_cast<long long>(coord2 * 100000);

      Point coords;
      coords.lat = lat;
      coords.lon = lon;
      points[v] = coords;
    }
    else if (mode == "E") {
      int com1 = str1.find(",");
      // vStr is the first vertex v
      string vStr = str1.substr(0, com1);

      // str2 contains only the vertex u and edge's street name
      string str2 = str1.substr(com1 + 1, str1.length() - com1 - 1);
      int com2 = str2.find(",");

      // uStr is the second vertex u
      string uStr = str2.substr(0, com2);

      // Convert vertices
      int v = stoi(vStr);
      int u = stoi(uStr);

      // Calculate edge weight
      long long w = mathattan(points[v], points[u]);

      // Graph is directed from v to u with weight w
      graph.addEdge(v, u, w);
    }
  }
}

int main(int argc, char *argv[]) {
  WDigraph graph;
  unordered_map<int, Point> points;

  // build the graph
  // filename edited so that it could be read from the server directory.
  readGraph("./server/edmonton-roads-2.0.1.txt", graph, points);

  // Socket creation and communication built with help from code in lec. 16

  // Built socket and its description
  struct sockaddr_in my_addr, peer_addr;
  memset(&my_addr, '\0', sizeof my_addr);
  int lstn_socket_desc, conn_socket_desc;
  char buffer[BUFFER_SIZE] = {};

  // Listening socket creation
  lstn_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (lstn_socket_desc == -1) {
    cerr << "Listening socket creation failed!\n";
    return 1;
  }

  int PORT = atoi(argv[1]);
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(lstn_socket_desc, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
    cerr << "Binding failed!\n";
    close(lstn_socket_desc);
    return 1;
  }

  if (listen(lstn_socket_desc, LISTEN_BACKLOG) == -1) {
    cerr << "Cannot listen to the specified socket!\n";
    close(lstn_socket_desc);
    return 1;
  }

  socklen_t peer_addr_size = sizeof my_addr;

  // Allow multiple connections
  while (true) {
    // Connect
    conn_socket_desc = accept(lstn_socket_desc, (struct sockaddr *)&peer_addr, &peer_addr_size);
    if (conn_socket_desc == -1) {
      cerr << "Connection socket creation failed!\n";
      return 1;
    }
    cout << "Connection established!\n";

    // Mode implemented to check for the input "Q"
    string mode = "";
    while (true) {
      // When a request is given, read it
      int rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
      if (rec_size == -1) {
        cout << "Timeout occurred... still waiting!\n";
        continue;
      }

      // If "Q\n" was received, close the server
      if (strcmp("Q\n", buffer) == 0) {
        mode = "Quit";
        cout << "Connection closing...\n";
        break;
      }

      // Split input by space and assume "\n" is the end
      string tempStr;
      vector<string> clientRequest;
      for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == ' ') {
          clientRequest.push_back(tempStr);
          tempStr.erase();
        }
        else if (buffer[i] == '\n') {
          clientRequest.push_back(tempStr);
          break;
        }
        else {
          tempStr += buffer[i];
        }
      }

      // Error check request
      assert(clientRequest[0] == "R");

      // Convert lat and lon strings into long long ints
      Point sPoint, ePoint;
      sPoint.lat = stoll(clientRequest[1]);
      sPoint.lon = stoll(clientRequest[2]);
      ePoint.lat = stoll(clientRequest[3]);
      ePoint.lon = stoll(clientRequest[4]);

      // find closest points
      int start = findClosest(sPoint, points), end = findClosest(ePoint, points);

      // run dijkstra's alg.
      unordered_map<int, PIL> tree;
      dijkstra(graph, start, tree);

      // no path
      if (tree.find(end) == tree.end()) {
        string noPathMsg = "N 0";
        send(conn_socket_desc, noPathMsg.c_str(), noPathMsg.length() + 1, 0);
      }
      else {
        list<int> path;
        while (end != start) {
          path.push_front(end);
          end = tree[end].first;
        }
        path.push_front(start);

        // send the number of total path
        string pathMsg = "N " + to_string(path.size());
        send(conn_socket_desc, pathMsg.c_str(), pathMsg.length() + 1, 0);

        for (int v : path) {
          // Wait for acknowledgement before sending each waypoint
          int rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
          if (rec_size == -1) {
            cout << "Timeout occurred... still waiting!\n";
            continue;
          }

          // Make sure an acknowledgement was received
          if (strcmp("A", buffer) != 0) {
            cerr << "Acknowledgement failed... \n";
            return 1;
          }

          // Prepare for waypoint to be sent to client
          string strLat = to_string(points[v].lat);
          string strLon = to_string(points[v].lon);

          // Send waypoint
          string waypoint = strLat + " " + strLon + "\n";
          send(conn_socket_desc, waypoint.c_str(), waypoint.length() + 1, 0);
        }

        // Await acknowledgement before telling client there are no more waypoints
        int rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
        if (rec_size == -1) {
          cout << "Timeout occurred... still waiting!\n";
          continue;
        }

        // Make sure an acknowledgement was received
        if (strcmp("A", buffer) != 0) {
          cerr << "Acknowledgement failed... \n";
          return 1;
        }

        // Tell client there are no more data points
        string endStr = "E\n";
        send(conn_socket_desc, endStr.c_str(), endStr.length() + 1, 0);
      }
    }
    // If client quits, server quits too
    if (mode == "Quit")
      break;
  }
  // Before program ends and exits, close sockets
  close(lstn_socket_desc);
  close(conn_socket_desc);
  return 0;
}
