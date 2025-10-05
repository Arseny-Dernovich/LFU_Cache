#include <iostream>
#include <vector>
#include "LFU.hpp"
#include "perfect_cache.hpp"


int main ()
{
    int capacity = 0 , n = 0;
    std::vector<int> seq = {};

    if (!Read_Input (capacity , n , seq)) {
        return 1;
    }

    int lfuHits = Run_LFU (capacity , seq);
    int optHits = Run_OPT (capacity , seq);

    std::cout << "Capacity=" << capacity
              << " | Requests=" << n
              << " | LFU hits=" << lfuHits
              << " | OPT hits=" << optHits
              << "\n";

    return 0;
}




bool Read_Input (int& capacity , int& n , std::vector<int>& seq)
{
    if (!(std::cin >> capacity >> n)) {

        std::cerr << "Ошибка: неверный ввод  (capacity , n)\n";
        return false;
    }

    seq.resize(n);
    for (int i = 0; i < n; i++) {

        if (! (std::cin >> seq[i])) {

            std::cerr << "Ошибка: не хватает элементов во входных данных\n";
            return false;
        }
    }
    return true;
}


int Run_LFU (int capacity , const std::vector<int>& seq)
{
    LFUCache<int ,int> lfu (capacity);
    int hits = 0;
    for (int key : seq) {

        auto v = lfu.get(key);
        if  (v) {
            hits++;
        } else {
            lfu.put(key , key);
        }
    }
    return hits;
}


int Run_OPT (int capacity , const std::vector<int>& seq)
{
    OptimalCache<int> opt (capacity , seq);
    return opt.run();
}


