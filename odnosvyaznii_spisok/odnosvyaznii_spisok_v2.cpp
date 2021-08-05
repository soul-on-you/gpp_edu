#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#define noLoop -1

struct UserData
{
    int id;
    int developer_id;
    char code[12];
    char name[45];
    void printData()
    {
        std::cout << "Worker name: " << name << " developer id = " << developer_id << " code:" << code << '\n';
    }
};

struct ListItem
{
    UserData _UserData;
    ListItem *next;
};

class SinglyLinkedList
{
    ListItem *_head;
    int _chainIndex;

public:
    SinglyLinkedList(const int count = 0, const UserData *_UserData = nullptr, const int LoopedOnID = noLoop) : _chainIndex(LoopedOnID)
    {
        _head = nullptr;
        if (count > 0)
        {
            ListItem *prevItem = nullptr;
            for (int i = 0; i < count; i++)
            {
                ListItem *nextItem = new (std::nothrow) ListItem;
                if (!nextItem)
                {
                    ListRemove();
                    return;
                }
                nextItem->_UserData = _UserData[i];
                if (!_head)
                    _head = nextItem;
                else
                    prevItem->next = nextItem;
                prevItem = nextItem;
            }
            if (_head)
            {
                if ((rand() % 2) || (LoopedOnID > count) || (LoopedOnID < 0)) //генерирует цикл с вероятностью 50/50
                // if (false || LoopedOnID == noLoop)                         //генерирует цикл
                // if (true)                                                  //без цикла
                {
                    _chainIndex = noLoop;
                    prevItem->next = nullptr;
                }
                else
                {
                    if (LoopedOnID == count)
                        prevItem->next = prevItem;
                    else
                    {
                        ListItem *curItem = _head;
                        for (int i = 0; i <= LoopedOnID; i++, curItem = curItem->next)
                            prevItem->next = curItem;
                        // prevItem->next = &(_head[LoopedOnID]);
                    }
                    _chainIndex = getIndexByAdress(prevItem->next);
                }
            }
        }
    }
    ~SinglyLinkedList()
    {
        ListRemove();
    }
    SinglyLinkedList(const SinglyLinkedList &List)
    {
        if (_chainIndex == noLoop)
            _head = nullptr;
        for (ListItem *curItem = List._head, *prevItem = nullptr; curItem; curItem = curItem->next)
        {
            ListItem *nextItem = new (std::nothrow) ListItem;
            if (!nextItem)
            {
                ListRemove();
                _chainIndex = noLoop, _head = nullptr;
                return;
            }
            nextItem->_UserData = curItem->_UserData;
            if (!_head)
                _head = nextItem;
            else
                prevItem->next = nextItem;
            prevItem = nextItem;
        }
    }
    //void ListRemoveItem(ListItem*& _head, int pos, int count)
    void ListRemove()
    {
        if (_chainIndex != noLoop)
            for (ListItem *tmp = _head; _chainIndex != -1; _head = _head->next, delete tmp, tmp = _head, _chainIndex--)
                ;
        else
            for (ListItem *tmp = _head; _head; _head = _head->next, delete tmp, tmp = _head)
                ;
        _chainIndex = noLoop, _head = nullptr;
    }
    void ListAddItem(const int addIndex, const UserData *_UserData)
    {
        ListItem *curItem = _head, *prevItem = nullptr;
        for (int i = 0; i < addIndex; i++, prevItem = curItem, curItem = curItem->next)
            ;
        //prevItem=&_head[addIndex-1], curItem=&_head[addIndex];
        ListItem *newItem = new (std::nothrow) ListItem;
        if (!newItem)
        {
            std::cerr << "не удалось добавить элемент\n";
            return;
        }
        newItem->_UserData = *_UserData;
        prevItem->next = newItem;
        newItem->next = curItem;
    }
    void PushBack(const UserData *_UserData)
    {
        ListItem *newItem = new (std::nothrow) ListItem;
        if (!newItem)
        {
            std::cerr << "не удалось добавить элемент\n";
            return;
        }
        ListItem *curItem = _head;
        for (; curItem->next; curItem = curItem->next)
            ;
        curItem->next = newItem;
        newItem->next = nullptr;
        newItem->_UserData = *_UserData;
    }
    void ListDeleteItem(const int delIndex, const int count = 1)
    {
        ListItem *curItem = _head, *prevItem = nullptr;
        for (int i = 0; i < delIndex; i++, prevItem = curItem, curItem = curItem->next)
            ;
        for (int i = 0; i < count; i++)
        {
            if (curItem)
            {
                if (i > 0)
                    curItem = prevItem->next;
                prevItem->next = curItem->next;
            }
            else
            {
                if (i > 0)
                    curItem = _head;
                _head = _head->next;
            }
            delete curItem;
        }
    }
    void PrintList() const
    {
        for (ListItem *curItem = _head; curItem; curItem = curItem->next)
            curItem->_UserData.printData();
    }
    void PrintListWithIndex(const int index) const
    {
        //_head[index]._UserData.printData(); использовал с перегрузкой оператора[]
        this->appeal(index)->_UserData.printData();
    }

    ListItem *appeal(const int index) const
    {
        ListItem *curItem = this->_head;
        for (int i = 0; i < index; i++, curItem = curItem->next)
            ;
        return curItem;
    }
    // перегрузка оператора работает не правильно
    // ListItem *operator[](const int index)
    // {
    //     ListItem *curItem = _head;
    //     for (int i = 0; i < index; i++, curItem = curItem->next)
    //         ;
    //     return curItem;
    // }
    ListItem *searchCycle_v1() const
    {
        for (ListItem *curItem = _head; curItem; curItem = curItem->next)
        {
            if (curItem == curItem->next)
                return curItem;
            for (ListItem *checkItem = _head; checkItem != curItem; checkItem = checkItem->next)
                if (checkItem == curItem->next)
                    return checkItem;
        }
        return nullptr;
    }
    ListItem *searchCycle_v2() const
    {
        if (_head && _head->next)
        {
            ListItem *slow = _head->next, *fast = _head->next->next;
            while (slow != fast)
                if (fast && fast->next)
                    slow = slow->next, fast = fast->next->next;
                else
                    return nullptr;
            return slow;
        }
        return nullptr;
    }
    // ListItem *searchCycle_v3() const
    // {

    // }
    int getIndexByAdress(const ListItem *findItem) const
    {
        if (findItem)
        {
            int i(0);
            for (ListItem *curItem = _head; curItem != findItem; curItem = curItem->next, i++)
                ;
            return i;
        }
        return -1;
    }
    int getChainIndex() const
    {
        return _chainIndex;
    }
};

int main()
{
    UserData data[4]{1, 1, "dave", "1", 2, 2, "relLik", "13vova", 3, 3, "victor", "victor", 4, 4, "timopheii", "motoevacuator"};
    SinglyLinkedList test;
    SinglyLinkedList t(4, data);
    std::cout << "\nall workers data:\n";
    t.PrintList();
    std::cout << "\nmy favotite worker:\n";
    t.PrintListWithIndex(1);
    UserData newWorker{6, 6, ".net332", "kravl"};
    std::cout << "\nadd new worker, and fire old useless\n";
    t.ListAddItem(4, &newWorker);
    t.ListDeleteItem(1);
    std::cout << "\nall workers data:\n";
    t.PrintList();
    std::cout << "\nmy favotite worker:\n";
    t.PrintListWithIndex(1);
    // test.ListRemove();
    // std::cout << "\nadd new item in empty list:\n";
    // UserData newTest{10, 10, "hello", "world"};
    // test.ListAddItem(0,&newTest);
    UserData data2[4]{11, 728, "dkwjqe312ed", "iordan", 12, 732, "00elda2e9iu", "kenny", 13, 733, "u29we29ikod", "reqi", 14, 671, "koaskdo201s", "greg"};
    SinglyLinkedList testWithLoop(4, data2, 2);
    if (testWithLoop.searchCycle_v1() && testWithLoop.searchCycle_v2())
    {
        std::cout << "\nList looped on index " << testWithLoop.getChainIndex() << ", cycle start on "
                  << testWithLoop.searchCycle_v1() << '\t' << " detected by searcbCycle_v1\n";
        std::cout << "\nList looped on index " << testWithLoop.getChainIndex() << ", cycle start on "
                  << testWithLoop.searchCycle_v2() << '\t' << " detected by searcbCycle_v2\n";
    }
    else
    {
        std::cout << "\nList haven't loop\n";
        testWithLoop.PrintList();
    }
    return 0;
}