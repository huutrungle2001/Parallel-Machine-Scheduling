#include "Instance.h"
#include "Solution.h"
#include "Solver.h"

using namespace std;
std::mt19937 mt = std::mt19937(0);

int main() {
    vector<double> process_time = {11.8, 12.6, 19.8, 12.5, 14.4,
                                   9.9, 9.9, 7.3, 14.0, 14.4,
                                   17.4, 14.5, 19.0, 7.1, 10.4,
                                   7.8, 17.8, 29.2, 8.4, 9.9,
                                   10.4, 8.2, 12.1, 19.4, 9.6,
                                   19.7, 15.6, 11.0, 16.1, 10.9,
                                   19.9, 13.3, 8.1, 12.2, 13.8,
                                   15.9, 15.4, 0.1, 0.2, 9.7,
                                   29.2, 7.7, 7.9, 15.0, 11.7,
                                   19.1, 8.5, 10.5, 16.4, 13.1,
                                   99.8, 9, 9, 7, 8,
                                   5, 220, 100, 7.9, 9.7};

    double sum_time = 0.0;
    int numMachine = 5;
    int numJob = process_time.size();
    for (auto &i : process_time)
        sum_time += i;
    cout << "Thoi gian trung binh: " << sum_time / 5.0 << "\n";
    Instance instance(numMachine, numJob, process_time);

    Solver Solver(&instance);
    Solver.Construction();

    Solver.Run();
    Solver.display();

    return 0;
}
