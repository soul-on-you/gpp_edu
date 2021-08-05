#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

enum LoopInfo
{
    noLoop = 0,
    curItemLoop,
    oldItemLoop
};

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
    SinglyLinkedList(const int count = 0, UserData *_UserData = nullptr, const int LoopedOnID = -1) : _chainIndex(LoopedOnID)
    {
        _head = nullptr;
        if (count > 0)
        {
            ListItem *prevItem;
            for (int i = 0; i < count; i++)
            {
                ListItem *nextItem = new (std::nothrow) ListItem;
                if (!nextItem)
                {
                    ListRemove();
                    _chainIndex = -1, _head = nullptr;
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
                if ((rand() % 2) || (LoopedOnID > count) || (LoopedOnID < 0))
                    prevItem->next = nullptr;
                else
                {
                    if (LoopedOnID == count)
                        prevItem->next = prevItem;
                    else
                    {
                        // ListItem *curItem = _head;
                        // for (int i = 0; i < LoopedOnID; i++, curItem = curItem->next);
                        prevItem->next = &(_head[LoopedOnID]);
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
        _head = nullptr;
    }
    //void ListRemoveItem(ListItem*& _head, int pos, int count)
    void ListRemove()
    {
        for (ListItem *tmp = _head; _head; _head = _head->next, delete tmp, tmp = _head)
            ;
    }
    void ListAddItem(const int addIndex, UserData *_UserData = nullptr)
    {
        ListItem *curItem = _head, *prevItem;
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
    void ListDeleteItem(const int delIndex, const int count = 1)
    {
        ListItem *curItem = _head, *prevItem;
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
    void PrintList()
    {
        for (ListItem *curItem = _head; curItem; curItem = curItem->next)
            curItem->_UserData.printData();
    }
    void PrintListWithIndex(const int index)
    {
        //_head[index]._UserData.printData(); использовал с перегрузкой оператора[]
        this->appeal(index)->_UserData.printData();
    }

    ListItem *appeal(const int index)
    {
        ListItem *curItem = this->_head;
        for (int i = 0; i < index; i++, curItem = curItem->next)
            ;
        return curItem;
    }
    ListItem *operator[](const int index)
    {
        ListItem *curItem = _head;
        for (int i = 0; i < index; i++, curItem = curItem->next)
            ;
        return curItem;
    }
    ListItem *searchCycle()
    {
        for (ListItem *curItem = _head; curItem; curItem = curItem->next)
        {
            if (curItem == curItem->next)
                return curItem;
            for (ListItem *checkItem = _head; checkItem != curItem; checkItem = checkItem->next)
                if (checkItem = curItem->next)
                    return checkItem;
        }
        return nullptr;
    }
    int getIndexByAdress(const ListItem *findItem)
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
    int getChainIndex()
    {
        return _chainIndex;
    }
};

int main()
{
    UserData data[4]{1, 1, "dave", "1", 2, 2, "killer", "13vova", 3, 3, "victor", "victor", 4, 4, "timopheii", "motoevacuator"};
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
    std::cout << '\n'
              << testWithLoop.getChainIndex() << '\n';
    if (testWithLoop.searchCycle())
        std::cout << "Список имеет цикл, начинающийся с " << testWithLoop.getChainIndex();
    else
    {
        std::cout << "Список не имеет цикл";
        testWithLoop.PrintList();
    }
    return 0;
}