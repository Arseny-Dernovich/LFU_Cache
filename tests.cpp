#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include "LFU.hpp"
#include "perfect_cache.hpp"  

struct TestResult {
    std::string filename;
    int capacity;
    int requests;
    int lfuHits;
    int optHits;
};

TestResult RunTest (const std::string& filename)
{
    std::ifstream fin (filename);
    if (!fin) {
        std::cerr << "Ошибка: не могу открыть файл " << filename << "\n";
        return {"",0,0,-1,-1};
    }

    int capacity = 0 , n = 0;
    fin >> capacity >> n;

    std::vector<int> seq (n);
    for (int i = 0; i < n; i++) fin >> seq[i];

    
    LFUCache<int,int> cache (capacity);
    int lfuHits = 0;
    for (int key : seq) {

        auto v = cache.get (key);
        if (v) {
            lfuHits++;
        } 
        
        else {
            cache.put (key , key);
        }
    }

    
    OptimalCache<int> opt (capacity , seq);
    int optHits = opt.run ();

    return {filename , capacity , n , lfuHits , optHits};
}

int main ()
{
    std::vector<std::string> files = {"test1.dat" , "test2.dat" , "test3.dat" , "test4.dat" ,
                                      "test5.dat" , "test6.dat" , "test7.dat" , "test8.dat" , 
                                      "test9.dat" , "test10.dat"};
 
    for (const auto& f : files) {

        auto result = RunTest (f);
        if (result.lfuHits >= 0) {
                    
            std::cout << "Файл: " << result.filename
                      << " | Capacity=" << result.capacity
                      << " | Requests=" << result.requests
                      << " | LFU hits=" << result.lfuHits
                      << " | OPT hits=" << result.optHits
                      << "\n";
        }
    }
    return 0;   
}

