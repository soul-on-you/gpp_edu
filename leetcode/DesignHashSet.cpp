#include <iostream>
#include <cstring>
class MyHashSet
{
public:
    MyHashSet()
    {
        keysMap = new (std::nothrow) int *[10];
        size = 10;
        for (int i = 0; i < 10; i++)
            *keysMap = new (std::nothrow) int[64];
    }

    void add(int key)
    {
        // for (int i = 1; i < size; i++)
        //     if (key <)
    }

    void remove(int key)
    {
    }

    bool contains(int key)
    {
        return false;
    }

private:
    int **keysMap;
    int size;
};

class MyHashSetUsingMemset
{
    int arrayOfBites[1024];

public:
    MyHashSetUsingMemset()
    {
        memset(arrayOfBites, false, sizeof(arrayOfBites));
    }

    void add(int key)
    {
        arrayOfBites[key] = true;
    }

    void remove(int key)
    {
        arrayOfBites[key] = false;
    }

    bool contains(int key)
    {
        return arrayOfBites[key];
    }
};

int main()
{
    MyHashSetUsingMemset hashSet;
    hashSet.add(1);
    hashSet.add(2);
    std::cout << hashSet.contains(3) << '\n';
    std::cout << hashSet.contains(2) << '\n';
    hashSet.remove(2);
    std::cout << hashSet.contains(2) << '\n';
    return 0;
}

/**
 * Your MyHashSet object will be instantiated and called as such:
 * MyHashSet* obj = new MyHashSet();
 * obj->add(key);
 * obj->remove(key);
 * bool param_3 = obj->contains(key);
 */