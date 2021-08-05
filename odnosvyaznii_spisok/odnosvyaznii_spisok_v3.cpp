#include <iostream>
#include <stdlib.h>
#include <time.h>

#define noLoop -1
#define Empty 0

struct UserData
{
    int id;
    int developer_id;
    char code[12];
    char name[45];
    void printData()
    {
        std::cout << "Worker name: " << name << " developer id = " << developer_id << " code: " << code << '\n';
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
    int _listLenght;

public:
    SinglyLinkedList(const int count = 0, const UserData *_UserData = nullptr, const int LoopedOnID = noLoop) : _chainIndex(LoopedOnID)
    {
        _head = nullptr;
        if (count > 0)
        {
            ListItem *prevItem = nullptr;
            for (int i = 0; i < count; i++)
            {
                ListItem *newItem;
                this->createItem(newItem);
                if (!newItem)
                {
                    ListRemove();
                    return;
                }
                newItem->_UserData = _UserData[i];
                if (!_head)
                    _head = newItem;
                else
                    prevItem->next = newItem;
                prevItem = newItem;
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
            ListItem *newItem;
            this->createItem(newItem);
            if (!newItem)
            {
                ListRemove();
                return;
            }
            newItem->_UserData = curItem->_UserData;
            if (!_head)
                _head = newItem;
            else
                prevItem->next = newItem;
            prevItem = newItem;
        }
    }
    void ListRemove()
    {
        // if (_chainIndex != noLoop)
        //     for (ListItem *tmp = _head; _listLenght; _head = _head->next, delete tmp, tmp = _head, _listLenght--)
        //         ;
        // else
        //     for (ListItem *tmp = _head; _head; _head = _head->next, delete tmp, tmp = _head)
        //         ;
        for (ListItem *tmp = _head; _listLenght; _head = _head->next, delete tmp, tmp = _head, _listLenght--)
            ;
        _chainIndex = noLoop, _head = nullptr; //_listLenght = Empty
    }
    void ListAddItem(const int addIndex, const UserData *_UserData)
    {
        if (_listLenght >= addIndex)
        {
            ListItem *curItem = _head, *prevItem = nullptr;
            if (_head && addIndex)
            {
                for (int i = 0;; i++, prevItem = curItem, curItem = curItem->next)
                    if (i == addIndex)
                    {
                        // prevItem=&_head[addIndex-1], curItem=&_head[addIndex];
                        ListItem *newItem;
                        this->createItem(newItem);
                        if (newItem)
                        {
                            newItem->_UserData = *_UserData;
                            prevItem->next = newItem;
                            newItem->next = curItem;
                        }
                        break;
                    }
            }
            else
            {
                ListItem *newItem;
                this->createItem(newItem);
                if (newItem)
                {
                    if (_head)
                        newItem->next = _head;
                    else
                        newItem->next = nullptr;
                    newItem->_UserData = *_UserData;
                    _head = newItem;
                }
            }
        }
        else
            std::cerr << "\nError! Unsuccessful attempt ListAddItem\n";
    }
    void PushBack(const UserData *_UserData)
    {
        ListItem *newItem;
        this->createItem(newItem);
        if (newItem)
        {
            ListItem *curItem = _head;
            for (; curItem->next; curItem = curItem->next)
                ;
            curItem->next = newItem;
            newItem->next = nullptr;
            newItem->_UserData = *_UserData;
        }
    }
    void ListDeleteItem(const int delIndex, const int count = 1)
    {
        if (_listLenght >= delIndex + count && _head)
        {
            ListItem *curItem = _head, *prevItem = nullptr;
            for (int index = 0; index < delIndex; index++, prevItem = curItem, curItem = curItem->next)
                ;
            if (curItem != _head)
            {
                for (int i = 0; i < count; i++, _listLenght--, delete curItem)
                {
                    if (i > 0)
                    {
                        // if (prevItem->next)
                        //     curItem = prevItem->next;
                        // else
                        //     break;
                        curItem = prevItem->next;
                    }
                    prevItem->next = curItem->next;
                }
            }
            else
            {
                for (int i = 0; i < count; i++, _listLenght--, delete curItem)
                {
                    curItem = _head;
                    _head = _head->next;
                    // if (!_head)
                    //     break;
                }
            }
        }
        else
            std::cerr << "\nError! Unsuccessful attempt ListDeleteItem\n";
    }
    void PrintList() const
    {
        for (ListItem *curItem = _head; curItem; curItem = curItem->next)
            curItem->_UserData.printData();
    }
    void PrintListWithIndex(const int index) const
    {
        //_head[index]._UserData.printData(); использовал с перегрузкой оператора[]
        ListItem *curItem = this->appeal(index);
        if (curItem)
            curItem->_UserData.printData();
        else
            std::cerr << "\nError! Unsuccessful attempt PrintListWithIndex\n";
    }

    ListItem *appeal(const int index) const
    {
        if (_listLenght <= index)
            return nullptr;
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
    int getLenght() const
    {
        return _listLenght;
    }

protected:
    ListItem *newItem()
    {
        ListItem *newItem = new (std::nothrow) ListItem;
        return newItem;
    }
    void createItem(ListItem *&newItem)
    {
        if (!(newItem = this->newItem()))
        {
            std::cerr << "\nне удалось добавить элемент\n";
        }
        else
            _listLenght++;
    }
};

int main()
{
    UserData dataForTmp[4]{1, 1, "dave", "1", 2, 2, "relLik", "13vova", 3, 3, "victor", "victor", 4, 4, "timopheii", "motoevacuator"};
    SinglyLinkedList test;
    SinglyLinkedList tmp(4, dataForTmp);
    std::cout << "\nall workers data:\n";
    tmp.PrintList();
    std::cout << "\nmy favotite worker:\n";
    tmp.PrintListWithIndex(1);
    UserData newWorker{6, 6, ".net332", "kravl"};
    std::cout << "\nadd new worker, and fire old useless\n";
    tmp.ListAddItem(4, &newWorker);
    tmp.ListDeleteItem(1);
    std::cout << "\nall workers data:\n";
    tmp.PrintList();
    std::cout << "\nmy favotite worker:\n";
    tmp.PrintListWithIndex(1);
    //test.ListRemove();
    // std::cout << "\nadd new item in empty list:\n";
    // UserData newTest{10, 10, "hello", "world"};
    // test.ListAddItem(0,&newTest);
    UserData dataForListWithLoop[4]{11, 728, "dkwjqe312ed", "iordan", 12, 732, "00elda2e9iu", "kenny", 13, 733, "u29we29ikod", "reqi", 14, 671, "koaskdo201s", "greg"};
    SinglyLinkedList listWithLoop(4, dataForListWithLoop, 2);
    if (listWithLoop.searchCycle_v1() && listWithLoop.searchCycle_v2())
    {
        std::cout << "\nList looped on index " << listWithLoop.getIndexByAdress(listWithLoop.searchCycle_v1()) << ", cycle start on "
                  << listWithLoop.searchCycle_v1() << '\t' << " detected by searcbCycle_v1\n";
        std::cout << "\nList looped on index " << listWithLoop.getIndexByAdress(listWithLoop.searchCycle_v2()) << ", cycle start on "
                  << listWithLoop.searchCycle_v2() << '\t' << " detected by searcbCycle_v2\n";
    }
    else
    {
        std::cout << "\nList haven't loop\n";
        listWithLoop.PrintList();
    }
    return 0;
}