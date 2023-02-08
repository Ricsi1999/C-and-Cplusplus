#ifndef STATISTICSCALCULATOR_HPP_INCLUDED
#define STATISTICSCALCULATOR_HPP_INCLUDED
#include "sortNumbers.hpp"
#include "filterRepetition.hpp"
#include <vector>

/*
   Author: Harada Richard
   Date: 2019. 10. 23.
*/

template <typename T> class statisticsCalculator {
private:
    int _lv1, _lv2, value;
    T mean, median, range, maximum, minimum;
    std::vector<T> v_modeoutput;
    std::vector<T> v_modenumbers;

public:
    T showMean(std::vector<T> data)
    {///double recommended
        mean = 0;
        for(_lv1 = 0; _lv1 < data.size(); _lv1++) {
            mean += data[_lv1];
        }
        return mean/data.size();
    }

    T showMedian(std::vector<T> data)
    {///double recommended
        sortNumbers<T> t;
        data = t.sort(data);
        if(data.size() % 2 == 0) {
            median = ((data[(data.size()/2)-1] + data[(data.size()/2)])/2);
        }
        else {
            median = data[((data.size()+1)/2)-1];
        }
        return median;
    }

    std::vector<T> showMode(std::vector<T> data)
    {
        ///vector only
        filterRepetition<T> t;
        v_modenumbers = data;
        v_modenumbers = t.select(v_modenumbers);
        maximum = 0;
        for(_lv1 = 0; _lv1 < v_modenumbers.size(); _lv1++) {
            value = 0;
            for(_lv2 = 0; _lv2 < data.size(); _lv2++) {
                if(data[_lv2] == v_modenumbers[_lv1]) {
                    value++;
                }
            }
            if(value > maximum) {
                maximum = value;
                v_modeoutput.clear();
                v_modeoutput.push_back(v_modenumbers[_lv1]);
            }
            else if(value == maximum) {
                v_modeoutput.push_back(v_modenumbers[_lv1]);
            }
        }
        return v_modeoutput;
    }

    T showRange(std::vector<T> data)
    {
        maximum = data[0], minimum = data[0];
        for(_lv1 = 1; _lv1 < data.size(); _lv1++) {
            if(data[_lv1] > maximum) {
                maximum = data[_lv1];
            }
            else if(data[_lv1] < minimum) {
                minimum = data[_lv1];
            }
        }
        return (maximum - minimum);
    }
};

#endif // STATISTICSCALCULATOR_HPP_INCLUDED
