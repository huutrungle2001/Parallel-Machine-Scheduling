#ifndef SOLUTION_H
#define SOLUTION_H
#include <bits/stdc++.h>
using namespace std;

class Solution {
   public:
    vector<vector<int>> machines;
    double maxTime;
    int maxMachine;
    vector<double> totalTime;
    vector<int> address;
    friend class Solver;

    Solution(int NM, int NJ) {
        for (int i = 0; i < NM; ++i) {
            machines.push_back(vector<int>());
        }

        maxTime = 0;
        maxMachine = 0;
        for (int i = 0; i < NM; ++i) {
            totalTime.push_back(0);
        }

        for (int i = 0; i < NJ; ++i) {
            address.push_back(-1);
        }
    }

    int getShortestMachine() {
        int shortest = 0;
        for (int i = 1; i < machines.size(); ++i) {
            if (totalTime[i] < totalTime[shortest])
                shortest = i;
        }
        return shortest;
    }

    void deleteJob(int index_mch, int index_job, double time_job) {
        machines[index_mch].erase(machines[index_mch].begin() + index_job);
        totalTime[index_mch] -= time_job;
        address[index_job] = -1;
        updateMaxTime();
    }
    void insertJob(int index_mch, int index_job, int id_job, double time_job) {
        machines[index_mch].insert(machines[index_mch].begin() + index_job, id_job);
        totalTime[index_mch] += time_job;
        address[index_job] = index_mch;
        updateMaxTime();
    }

    void updateMaxTime() {
        maxMachine = 0;
        for (int i = 1; i < machines.size(); ++i) {
            if (totalTime[i] > totalTime[maxMachine])
                maxMachine = i;
        }
        maxTime = totalTime[maxMachine];
    }

    void display() {
        cout << "Số lượng máy: " << machines.size() << "\n";
        for (int i = 0; i < machines.size(); ++i) {
            cout << "Máy thứ " << i + 1 << ":\n\t";
            cout << "Tổng thời gian: " << totalTime[i] << "\n";
            cout << "\tDanh sách các công việc: ";

            for (auto it = machines[i].begin(); it != machines[i].end(); ++it) {
                cout << *it << " ";
            }
            cout << "\n";
        }
    }

    double get_score() {
        return maxTime;
    }

    double get_range() {
        double minTime = totalTime[0];
        for (int time : totalTime) {
            if (minTime > time)
                minTime = time;
        }
        return maxTime - minTime;
    }
};

#endif 
