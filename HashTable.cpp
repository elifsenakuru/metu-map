#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <iomanip>

const int KeyedHashTable::PRIME_LIST[PRIME_TABLE_COUNT] =
{
     2,    3,   5,   7,  11,  13,  17,  19,
     23,  29,  31,  37,  41,  43,  47,  53,
     59,  61,  67,  71,  73,  79,  83,  89,
     97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311,
    313, 317, 331, 337, 347, 349, 353, 359,
    367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457,
    461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541
};

int KeyedHashTable::Hash(const std::string& key) const
{
    int hash = 0, k = 0, size = key.length();
    while(k < size)
    {
        hash += key[k] * PRIME_LIST[k];
        k++;
    }
    hash = hash % tableSize;
    return hash;
    // TODO
}

void KeyedHashTable::ReHash()
{
    int newsize, hashh;
    newsize = FindNearestLargerPrime(tableSize*2);
    std::vector<HashData> vectooor;
    int i = 0;
    for (; i < tableSize; i++)
    { 
        if(table[i].key != "")
            vectooor.push_back(table[i]);
    }
    delete [] table;
    table == NULL;
    tableSize = newsize;
    occupiedElementCount = 0;
    table = new HashData[newsize];
    for(i = 0; i < vectooor.size(); i++)
    {
        Insert(vectooor[i].key, vectooor[i].intArray);
    }
    // TODO
}

int KeyedHashTable::FindNearestLargerPrime(int requestedCapacity)
{
    int i = 0;
    while(PRIME_LIST[i] <= requestedCapacity && i < PRIME_TABLE_COUNT) i++;
    return PRIME_LIST[i];
    // TODO
}

KeyedHashTable::KeyedHashTable()
{
    table = new HashData[2];
    tableSize = 2;
    occupiedElementCount = 0;
    // TODO
}

KeyedHashTable::KeyedHashTable(int requestedCapacity)
{
    int prime;
    prime = FindNearestLargerPrime(requestedCapacity);
    table = new HashData[prime];
    tableSize = prime;
    occupiedElementCount = 0;
    // TODO
}

KeyedHashTable::KeyedHashTable(const KeyedHashTable& other)
{
    int size = other.tableSize;
    table = new HashData[size]   ;
    for(int i=0; i < size; i++)
    {
        table[i].key = other.table[i].key;
        table[i].intArray = other.table[i].intArray;
    }
    tableSize = size;
    occupiedElementCount = other.occupiedElementCount;
    // TODO
}

KeyedHashTable& KeyedHashTable::operator=(const KeyedHashTable& other)
{
    delete [] table;
    tableSize = 0;
    occupiedElementCount = 0;
    int size = other.tableSize;
    table = new HashData[size]   ;
    for(int i=0; i < size; i++)
    {
        table[i].key = other.table[i].key;
        table[i].intArray = other.table[i].intArray;
    }
    tableSize = size;
    occupiedElementCount = other.occupiedElementCount;
    // TODO
}

KeyedHashTable::~KeyedHashTable()
{
    delete [] table;
    tableSize = 0;
    occupiedElementCount = 0;
    // TODO
}

bool KeyedHashTable::Insert(const std::string& key,
                            const std::vector<int>& intArray)
{
    int pos, i;
    pos = Hash(key);
    i = 0;
    int old_pos=pos;
    while(table[pos].key != "")
    {
        if(key == table[pos].key) return false;
        pos = old_pos;
        pos = (pos+i*i) % tableSize;
        i++;
    }
    table[pos].key = key;
    table[pos].intArray = intArray;
    occupiedElementCount++;
    if(occupiedElementCount * EXPAND_THRESHOLD >= tableSize)
    {
        ReHash();
    }
    return true;
    // TODO
}

bool KeyedHashTable::Remove(const std::string& key)
{
    int pos, i, exist = false;
    std::vector<int> lol;
    exist = Find(lol, key);
    if(!exist) return false;
    for(i = 0; i < tableSize;i++)
    {
        if(key == table[i].key)
        {
            pos = i;
        }
    }
    table[pos].key = "";
    table[pos].intArray.clear();
    occupiedElementCount--;
    // TODO
}

void KeyedHashTable::ClearTable()
{
    int i;
    for(i = 0; i < tableSize; i++)
    {
        if(table[i].key == "") continue;
        table[i].intArray.clear();
        table[i].key = "";
        occupiedElementCount--;
    }
   // TODO
}

bool KeyedHashTable::Find(std::vector<int>& valueOut,
                          const std::string& key) const
{
    int i = 0, j;
    for(i = 0; i < tableSize; i++)
    {
        if(key == table[i].key)
        {
            valueOut = table[i].intArray;
            return true;
        }
    }
    return false;
    // TODO
}

void KeyedHashTable::Print() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    std::cout << "HT:";
    if(occupiedElementCount == 0)
    {
        std::cout << " Empty";
    }
    std::cout << "\n";
    for(int i = 0; i < tableSize; i++)
    {
        if(table[i].key == "") continue;

        std::cout << "[" << std::setw(3) << i << "] ";
        std::cout << "[" << table[i].key << "] ";
        std::cout << "[";
        for(size_t j = 0; j < table[i].intArray.size(); j++)
        {
            std::cout << table[i].intArray[j];
            if((j + 1) != table[i].intArray.size())
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
    std::cout.flush();
}