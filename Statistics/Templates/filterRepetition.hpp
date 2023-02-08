#ifndef FILTERREPETITION_HPP_INCLUDED
#define FILTERREPETITION_HPP_INCLUDED
#include <vector>

/*
   Author: Harada Richard
   Date: 2019. 10. 22.
*/

template <typename T> class filterRepetition {
private:
    int _lv1, _lv2;
    std::vector<T> filtered;

public:
    std::vector<T> select(std::vector<T> unfiltered)
    {
        filtered.push_back(unfiltered[0]);
        for(_lv1 = 1; _lv1 < unfiltered.size(); _lv1++) {
            _lv2 = 0;
            while(_lv2 < filtered.size() && filtered[_lv2] != unfiltered[_lv1]) {
                _lv2++;
            }
            if(_lv2 == filtered.size()) {
                filtered.push_back(unfiltered[_lv1]);
            }
        }
        return filtered;
    }
};

#endif // FILTERREPETITION_HPP_INCLUDED
