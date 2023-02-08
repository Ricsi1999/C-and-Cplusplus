#ifndef SORTNUMBERS_HPP_INCLUDED
#define SORTNUMBERS_HPP_INCLUDED
#include <vector>

/*
   Author: Harada Richard
   Date: 2019. 10. 22.
*/

template <typename T> class sortNumbers {
private:
    T minimum;
    int _lv1, _lv2, index, ns_length;
    std::vector<T> sorted;

public:
    std::vector<T> sort(std::vector<T> non_sorted)
    {
        ns_length = non_sorted.size();
        for(_lv1 = 0; _lv1 < ns_length; _lv1++) {
            minimum = non_sorted[0];
            index = 0;
            for(_lv2 = 1; _lv2 < non_sorted.size(); _lv2++) {
                if(non_sorted[_lv2] < minimum) {
                    index = _lv2;
                    minimum = non_sorted[_lv2];
                }
            }
            sorted.push_back(minimum);
            non_sorted.erase(non_sorted.begin() + index);
        }
        return sorted;
    }
};

#endif // SORTNUMBERS_HPP_INCLUDED
