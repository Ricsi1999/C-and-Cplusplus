#ifndef ARRAY_ACCUMULATER_H
#define ARRAY_ACCUMULATER_H

#include <iostream>
#include <vector>

template <typename T, typename O = std::plus<T>>
class array_accumulater
{
    public:
        array_accumulater(T* items, const int length)
        {   
            sum = items[0];
            original.push_back(items[0]);
            a.push_back(&items[0]);

            for(int i = 1; i < length; i++)
            {
                original.push_back(items[i]);
                a.push_back(&items[i]);
                sum = o(sum, items[i]);
                *(a[i]) = sum;
            }
        }
 
        ~array_accumulater()
        {       
            for(unsigned int i = 0; i < a.size(); i++)
            {
                *(a[i]) = original[i];
            }
        }

        void add(T* items, const int length)
        {   
            for(int i = 0; i < length; i++)
            {
                original.push_back(items[i]);
                a.push_back(&items[i]);
                sum = o(sum, items[i]);
                *(a.back()) = sum;
            }
        }

        unsigned int size() const
        {
            return a.size();
        }

    private:
        O o;
        T sum;
        std::vector<T> original;
        std::vector<T*> a;
};

#endif