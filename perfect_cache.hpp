#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <iostream>



size_t VALUE_WILL_UNUSED= std::numeric_limits<size_t>::max (); // значение ключа по умолчанию или если он в будущем не встретится

template <typename Key>
class OptimalCache {
    
public:

    OptimalCache (size_t capacity , const std::vector<Key>& sequence)
        : capacity_ (capacity) , sequence_ (sequence) {}

    
    size_t run () {

        std::unordered_set<Key> cache = {};
        size_t hits = 0;

        for (size_t i = 0; i < sequence_.size (); i++) {
            Key key = sequence_[i];

            // Попадание
            if (cache.count (key)) {

                hits++;
                continue;
            }

            
            if (cache.size () < capacity_) {
                cache.insert (key);
            } 
            
            else {
                
                Key toRemove = findFarthest (cache , i);
                cache.erase (toRemove);
                cache.insert (key);
            }
        }

        return hits;
    }

private:

    size_t capacity_ = 0;
    const std::vector<Key>& sequence_ = {};

    // ============================================================
    // findFarthest(cache, pos):
    // - находит элемент в cache, который дальше всего встречается
    //   в sequence после позиции pos
    // - если элемент больше не встречается → удаляем его сразу
    // ============================================================
    Key findFarthest (const std::unordered_set<Key>& cache , size_t pos)
    {
        std::unordered_map<Key , size_t> nextUse = {};
        for (auto k : cache) nextUse[k] = VALUE_WILL_UNUSED;

        // ищем следующее использование каждого ключа
        for (size_t j = pos + 1; j < sequence_.size (); j++) {

            if (nextUse.count (sequence_[j]) && nextUse[sequence_[j]] == VALUE_WILL_UNUSED) {
                nextUse[sequence_[j]] = j;
            }
        }

        // выбираем тот , у кого индекс максимальный (или не встречается вовсе)
        Key candidate = *cache.begin ();
        size_t farthest = 0;

        for (auto [k , idx] : nextUse) {

            if (idx == VALUE_WILL_UNUSED) {
                return k; // элемент больше не используется → его точно удаляем
            }

            if (idx > farthest) {

                farthest = idx;
                candidate = k;
            }
        }
        return candidate;
    }
};
