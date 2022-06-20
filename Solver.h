#ifndef SOLVER_H
#define SOLVER_H

#include <bits/stdc++.h>

#include "Instance.h"
#include "Solution.h"
using namespace std;

class Solver {
   private:
    Instance *instance;
    Solution *bestSolution;

   public:
    Solver() {}

    Solver(Instance *instance) {
        this->instance = instance;
    }

    void Run() {
        Construction();
        for (int iter = 0; iter < 1000; ++iter) {
            Solution tmp = *bestSolution;
            // dot bien
            Mutate(tmp);

            // dung local search de sua
            LocalSearch(tmp);

            // kiem tra xem co tot hay khong
            if (tmp.get_score() < bestSolution->get_score() ||
                (tmp.get_score() == bestSolution->get_score() && tmp.get_range() < bestSolution->get_range())) {
                *bestSolution = tmp;
            }
        }
    }

    void Mutate(Solution &s) {
        int step = rand() % 20 + 5;
        for (int i = 0; i < step; i++) {
            int ind[2], indexJob[2];
            ind[0] = rand() % s.machines.size();
            ind[1] = rand() % s.machines.size();
            indexJob[0] = rand() % s.machines[ind[0]].size();
            indexJob[1] = rand() % s.machines[ind[1]].size();
            int id_job_0 = s.machines[ind[0]][indexJob[0]];
            int id_job_1 = s.machines[ind[1]][indexJob[1]];

            s.machines[ind[0]][indexJob[0]] = id_job_1;
            s.machines[ind[1]][indexJob[1]] = id_job_0;

            s.totalTime[ind[0]] += getJobTime(id_job_1) - getJobTime(id_job_0);
            s.totalTime[ind[1]] += getJobTime(id_job_0) - getJobTime(id_job_1);
            s.address[s.machines[ind[0]][indexJob[0]]] = ind[0];
            s.address[s.machines[ind[1]][indexJob[1]]] = ind[1];
        }
        s.updateMaxTime();
        check(&s);
    }

    void LocalSearch(Solution &s) {
        bool improved = true;
        while (improved) {
            improved = false;
            Relocate(s, improved);
            Swap(s, improved);
        }
    }

    void Swap(Solution &s, bool &improved) {
        long step = 0;
    here:
        int id_may_to = s.maxMachine;
        vector<int> job_may_to;
        for (auto i : s.machines[id_may_to]) {
            job_may_to.push_back(i);
        };
        sort(job_may_to.begin(), job_may_to.end(), [this](const int &a, const int &b) -> bool {
            return getJobTime(a) > getJobTime(b);
        });

        for (int &job : s.machines[id_may_to]) {
            for (int may_khac = 0; may_khac < instance->numMachine; ++may_khac) {
                if (may_khac == id_may_to) continue;
                for (int &job_ : s.machines[may_khac]) {
                    double t_may_to = s.maxTime - getJobTime(job) + getJobTime(job_);
                    double t_may_khac = s.totalTime[may_khac] - getJobTime(job_) + getJobTime(job);

                    if (t_may_to < (s.maxTime - 0.0001) && t_may_khac < (s.maxTime - 0.0001)) {
                        job += job_;
                        job_ = job - job_;
                        job -= job_;
                        s.address[job] = may_khac;
                        s.address[job_] = id_may_to;
                        s.totalTime[may_khac] = t_may_khac;
                        s.totalTime[id_may_to] = t_may_to;
                        s.updateMaxTime();
                        check(&s);
                        improved = true;
                        goto here;
                    }
                }
            }
        }
    }

    void Relocate(Solution &s, bool &improved) {
    here_1:
        for (int i = 0; i < s.machines.size(); i++) {
            for (int j = 0; j < s.machines.size(); j++) {
                if (i == j) continue;
                for (int k = 0; k < s.machines[i].size(); ++k) {
                    int idJob = s.machines[i][k];
                    double score = s.get_score();
                    double range = s.get_range();

                    deleteJob(&s, k, i);
                    insertJob(&s, idJob, j);
                    if ((s.get_score() < score - 0.001) || (s.get_score() == score && s.get_range() < range - 0.001)) {
                        check(&s);
                        improved = true;
                        goto here_1;
                    }
                    deleteJob(&s, 0, j);
                    insertJob(&s, idJob, i);
                }
            }
        }
        improved = false;
    }

    void insertJob(Solution *solution, int idJob, int indexMachine) {
        solution->insertJob(indexMachine, 0, idJob, getJobTime(idJob));
    }

    void deleteJob(Solution *solution, int indexJob, int indexMachine) {
        solution->deleteJob(indexMachine, indexJob, getJobTime(solution->machines[indexMachine][indexJob]));
    }

    void Construction() {
        vector<pair<double, int>> jobsWithTime;
        for (int i = 0; i < instance->numJob; i++) {
            jobsWithTime.push_back(pair<double, int>(instance->times[i], i));
        }
        sort(jobsWithTime.rbegin(), jobsWithTime.rend());

        this->bestSolution = new Solution(instance->numMachine, instance->numJob);

        for (auto job : jobsWithTime) {
            int indexMachine = bestSolution->getShortestMachine();
            insertJob(bestSolution, job.second, indexMachine);
        }
    }

    void display() {
        bestSolution->display();
    }

    void check(Solution *s) {
        // test total time
        for (int machineID = 0; machineID < instance->numMachine; ++machineID) {
            double sumTime = 0;
            for (int jobID : s->machines[machineID]) {
                sumTime += instance->getTime(jobID);
            }
            assert(-0.0001 < (sumTime - s->totalTime[machineID]));
            assert((sumTime - s->totalTime[machineID]) < 0.0001);
        }
    }

    double getJobTime(int i) {
        return instance->getTime(i);
    }
};

#endif
