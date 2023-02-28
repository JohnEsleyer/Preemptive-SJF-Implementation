
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include "limits.h"

using namespace std;

struct Process{
	int pid;
	int burst_time;
	int arrival_time;
	int completion_time;
	int turn_around_time;
	int waiting_time;
};

void sjf_preemptive(vector<Process>* processes) {
    int n = processes->size();
    vector<int> remaining_time(n);
    for (int i = 0; i < n; i++) {
        remaining_time[i] = (*processes)[i].burst_time;
    }

    int cur_time = 0;
    int completed = 0;
    while (completed < n) {
        int shortest_job = -1;
        int shortest_time = INT_MAX;

        for (int i = 0; i < n; i++) {
            if ((*processes)[i].arrival_time <= cur_time && remaining_time[i] < shortest_time && remaining_time[i] > 0) {
                shortest_job = i;
                shortest_time = remaining_time[i];
            }
        }

        if (shortest_job == -1) {
            cur_time++;
            continue;
        }

        remaining_time[shortest_job]--;

        if (remaining_time[shortest_job] == 0) {
            completed++;
            (*processes)[shortest_job].completion_time = cur_time + 1;
            (*processes)[shortest_job].turn_around_time = (*processes)[shortest_job].completion_time - (*processes)[shortest_job].arrival_time;
            (*processes)[shortest_job].waiting_time = (*processes)[shortest_job].turn_around_time - (*processes)[shortest_job].burst_time;
        }

        cur_time++;
    }
}


void print_gantt_chart(vector<Process> p) {
    int n = p.size();
    int i, j;
    int last = p[n - 1].burst_time + (n == 1 ? 0 : p[n - 1].completion_time);
    // printing top bar
    cout << " ";
    for (i = 0; i < n; i++) {
        for (j = 0; j < p[i].burst_time; j++) cout << "--";
        cout << " ";
    }
    cout << "\n|";
    // middle position
    for (i = 0; i < n; i++) {
        for (j = 0; j < p[i].burst_time - 1; j++) cout << " ";
        cout << "p" << p[i].pid;
        for (j = 0; j < p[i].burst_time - 1; j++) cout << " ";
        cout << "|";
    }
    cout << "\n ";
    // bottom bar
    for (i = 0; i < n; i++) {
        for (j = 0; j < p[i].burst_time; j++) cout << "--";
        cout << " ";
    }
    cout << "\n";

    // printing completion time
    int minus = 0;
    for (i = 0; i < n; i++) {
        if (p[i].completion_time > 9) cout << " ";

        if (i == 0){
            cout << 0;
        }else{
            cout << p[i-1].completion_time;
        }


        if (p[i + 1].completion_time > 9) {
            minus = 1;
        }
        if (i + 1 == n) {
            if (last > 9) {
                minus = 1;
            }
        }
        for (j = 0; j < p[i].burst_time - minus; j++) cout << "  ";
    }
    if (last > 9) {
        cout << " ";
    }
    cout << p[n-1].completion_time << "\n";
}





bool sortByCompletionTime(const Process &p1, const Process &p2) {
    return p1.completion_time < p2.completion_time;
}


int main() {
    // Create a vector of Process pointers
    vector<Process> processes;

    // Ask for input
    int n;
    cout << "How many process you want to enter?:";
    cin >> n;
    for (int i=0;i<n;i++){
        cout << "===========================================" << endl;
        Process p;
        cout << "Enter arrival time for process " << i + 1 << ":";
        cin >> p.arrival_time;
        cout << "Enter burst time for process " << i + 1 << ":";
        cin >> p.burst_time;

        p.pid = i+1;
        processes.push_back(p);
    }

    vector<Process>* process_ptr = &processes;

    sjf_preemptive(process_ptr);

    // Print the table
    cout << "Proccess" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(20) << "Completion Time" << setw(20) << "Turnaround Time" << setw(15) << "Waiting Time" << endl;
    for (int i=0;i<processes.size();i++){
        cout << processes[i].pid << setw(15) << processes[i].arrival_time << setw(15) << processes[i].burst_time<< setw(20) << processes[i].completion_time << setw(20) << processes[i].turn_around_time << setw(15) << processes[i].waiting_time << endl;
    }

    //Average waiting-time
    int sum = 0;
    for (int i=0;i<processes.size();i++){
        sum += processes[i].waiting_time;
    }
    cout << "Average waiting time: " << sum << "/" << processes.size() << endl;


    //Average turn-around time
    sum = 0;
    for (int i=0;i<processes.size();i++){
        sum += processes[i].turn_around_time;
    }
    cout << "Average turn-around time " << sum << "/" << processes.size() << endl;


    //Sort by completion time
    sort(processes.begin(), processes.end(), sortByCompletionTime);

    print_gantt_chart(processes);

    return 0;
}
