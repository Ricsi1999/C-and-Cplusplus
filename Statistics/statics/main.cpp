#include <iostream>
#include <vector>
#include "../Templates/statisticsCalculator.hpp"

using namespace std;

int main()
{
    vector<double> vec = {1, 2, 2, 4, 4, 3, 3, 4, 5};
    statisticsCalculator<double> t;

    cout << "Mean: " << t.showMean(vec) << endl;
    cout << "Median " << t.showMedian(vec) << endl;
    cout << "Range: " << t.showRange(vec) << endl;
    vec = t.showMode(vec);

    cout << "Mode: ";
    for(int i = 0; i < vec.size(); i++){
        cout << vec[i] << " ";
    }

    return 0;
}
