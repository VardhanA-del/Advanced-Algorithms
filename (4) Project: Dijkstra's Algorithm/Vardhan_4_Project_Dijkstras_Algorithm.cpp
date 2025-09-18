#include <iostream>
#include <limits>
using namespace std;

int main() {
    int vrdn_nodes = 5; // node count A-E

    // making graph empty first
    int vrdn_graph[5][5]; 
    for (int xq = 0; xq < vrdn_nodes; xq++) {
        for (int zz = 0; zz < vrdn_nodes; zz++) {
            vrdn_graph[xq][zz] = -1; // no edge
        }
    }

    // putting edge weight inside
    vrdn_graph[0][1] = 2; 
    vrdn_graph[1][0] = 2; // A-B
    
    vrdn_graph[0][2] = 5; 
    vrdn_graph[2][0] = 5; // A-C
    
    vrdn_graph[1][2] = 1; 
    vrdn_graph[2][1] = 1; // B-C
    
    vrdn_graph[1][3] = 3;
    vrdn_graph[3][1] = 3; // B-D
    
    vrdn_graph[2][4] = 2; 
    vrdn_graph[4][2] = 2; // C-E
    
    vrdn_graph[3][4] = 4; 
    vrdn_graph[4][3] = 4; // D-E
    
    vrdn_graph[1][4] = 7; 
    vrdn_graph[4][1] = 7; // B-E

    // asking for start end node
    char vrdn_start_char, vrdn_end_char;
    cout << "Enter start node (A-E): ";
    cin >> vrdn_start_char;
    cout << "Enter end node (A-E): ";
    cin >> vrdn_end_char;

    // letter change to number
    int vrdn_start = vrdn_start_char - 'A';
    int vrdn_end = vrdn_end_char - 'A';

    // array for dist, used, prev
    int vrdn_dist[5];
    bool vrdn_used[5];
    int vrdn_prev[5];

    // set used false when distance is big
    for (int yy = 0; yy < vrdn_nodes; yy++) {
        vrdn_dist[yy] = numeric_limits<int>::max();
        vrdn_used[yy] = false;
        vrdn_prev[yy] = -1;
    }

    // start node cost 0
    vrdn_dist[vrdn_start] = 0;

    // dijkstra loop
    for (int kk = 0; kk < vrdn_nodes; kk++) {
        int vrdn_min = -1;

        // find node not used yet
        for (int mm = 0; mm < vrdn_nodes; mm++) {
            if (!vrdn_used[mm] && (vrdn_min == -1 || vrdn_dist[mm] < vrdn_dist[vrdn_min])) {
                vrdn_min = mm;
            }
        }

        // break if no node
        if (vrdn_dist[vrdn_min] == numeric_limits<int>::max()) break;

        // used bnide marked
        vrdn_used[vrdn_min] = true;

        // check all neighbor node
        for (int tt = 0; tt < vrdn_nodes; tt++) {
            if (vrdn_graph[vrdn_min][tt] != -1) {
                int new_dist = vrdn_dist[vrdn_min] + vrdn_graph[vrdn_min][tt];
                if (new_dist < vrdn_dist[tt]) {
                    vrdn_dist[tt] = new_dist;
                    vrdn_prev[tt] = vrdn_min;
                }
            }
        }
    }

    // show answer here
    if (vrdn_dist[vrdn_end] == numeric_limits<int>::max()) {
        cout << "No path found.\n";
    } else {
        cout << "Shortest cost: " << vrdn_dist[vrdn_end] << "\n";

        // make path back
        int vrdn_path[100];
        int vrdn_size = 0;
        for (int qq = vrdn_end; qq != -1; qq = vrdn_prev[qq]) {
            vrdn_path[vrdn_size++] = qq;
        }

        // print path good order
        cout << "Path: ";
        for (int rr = vrdn_size - 1; rr >= 0; rr--) {
            cout << char('A' + vrdn_path[rr]) << " ";
        }
        cout << "\n";
    }

    return 0;
}