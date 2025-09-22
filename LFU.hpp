#include <iostream>
#include <unordered_map>
#include <list>
#include <optional> 
#include <string>


template <typename Key , typename Value>
class LFUCache {

public:

    // ============================================================
    // Конструктор.
    // ============================================================
    LFUCache (size_t capacity) : capacity_ (capacity) , minFreq_ (0) {}


    // ============================================================
    // get(key): получить значение по ключу.
    // Если ключ есть → возвращает std::optional<Value> с данным.
    // Если ключа нет → возвращает std::nullopt.
    // При каждом обращении увеличивает частоту использования (touch).
    // ============================================================
    std::optional<Value> get (const Key& key)
    {

        auto it = cache_.find (key);
        if (it == cache_.end ()) return std::nullopt;
        touch (it);                  
        return it->second.value;    
    }



    // ============================================================
    // put(key, value): вставка или обновление значения по ключу.
    // - Если ключ уже есть → обновляется значение и частота.
    // - Если ключа нет и кэш полон → удаляется LFU элемент.
    // - Затем добавляется новый элемент с частотой 1.
    // ============================================================
    void put (const Key& key , const Value& value)
    {
        if (capacity_ == 0) return;

        auto it = cache_.find (key);
        if (it != cache_.end ()) {

            
            it->second.value = value;
            touch (it);
            return;
        }

        
        if (cache_.size () == capacity_) {

            auto &list = freqList_[minFreq_];   // список ключей с минимальной частотой
            Key oldKey = list.back ();           // выбрать "наименее недавно использованный" среди них
            list.pop_back ();
            cache_.erase (oldKey);
            if (list.empty ()) freqList_.erase (minFreq_);
        }

        // вставить новый элемент с частотой 1
        freqList_[1].push_front (key);
        // Вставляем Node: value , freq=1 , iterator на начало списка freqList_[1]
        cache_.insert ({key , Node{value , 1 , freqList_[1].begin ()}});
        minFreq_ = 1;
    }


private:

    struct Node {
        Value value;
        size_t freq;
        typename std::list<Key>::iterator it = {}; // итератор в списке freqList_[freq]
    };

    size_t capacity_ = 0;
    size_t minFreq_ = 0;
    std::unordered_map<Key , Node> cache_ = {};          
    std::unordered_map<size_t , std::list<Key>> freqList_ = {}; 

    using MapIt = typename std::unordered_map<Key , Node>::iterator;

    // ============================================================
    // touch(it): увеличить частоту использования элемента.
    // - удаляет элемент из старого freq-списка
    // - если список опустел → обновляем minFreq
    // - переносим элемент в список freq+1
    // ============================================================
    void touch (MapIt it)
    {
        size_t freq = it->second.freq;
        auto listIt = it->second.it;
        // удалить из старого списка
        freqList_[freq].erase (listIt);
        if (freqList_[freq].empty ()) {

            freqList_.erase (freq);
            if (minFreq_ == freq) minFreq_++; // если этот список был минимальным — увеличить minFreq
        }

        // вставить в список частоты freq+1 в начало (делаем push_front => recent = front)
        it->second.freq++;
        freqList_[it->second.freq].push_front (it->first);
        it->second.it = freqList_[it->second.freq].begin ();
    }
};


