//#define LoopChance100         //генерирует цикл + проверка на 488-491 и проверка работы конструктора
#define LoopChance50 //генерирует цикл с вероятностью 50/50
//#define LoopChance0           //без цикла

#include <iostream>
#include <stdlib.h>
#include <time.h>

#define noLoop -1
#define Empty 0
#define lastItem (_listLength - 1)

struct UserData
{
    int id;
    int developer_id;
    char code[12];
    char name[45];
    void printData() const
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
    int _listLength;

public:
    SinglyLinkedList(const int count = 0, const UserData *_UserData = nullptr, const int LoopedOnID = noLoop) : _chainIndex(LoopedOnID)
    {
        _head = nullptr, _listLength = 0;
        if (count > 0)
        {
            ListItem *prevItem = nullptr;
            for (int i = 0; i < count; i++)
            {
                ListItem *newItem;
                // this->createItem(newItem);
                // if (!newItem)
                // {
                //     ListRemove();
                //     return;
                // }
                // newItem->_UserData = _UserData[i];
                // if (!_head)
                //     _head = newItem;
                // else
                //     prevItem->next = newItem;
                if (!(this->connectItem(newItem = this->createItem(&_UserData[i]), prevItem)))
                {
                    ListRemove();
                    return;
                }
                prevItem = newItem;
            }
            if (_head)
            {
#ifdef LoopChance50
                if ((rand() % 2) || (LoopedOnID > count) || (LoopedOnID < 0))
#endif
#ifdef LoopChance100
                    if (false || LoopedOnID == noLoop)
#endif
#ifdef LoopChance0
                        if (true)
#endif
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
                                prevItem->next = this->appeal(LoopedOnID);
                                // prevItem->next = &(_head[LoopedOnID]);
                            }
                            //_chainIndex = getIndexByAdress(prevItem->next);
                        }
            }
        }
    }
    ~SinglyLinkedList()
    {
        ListRemove();
    }
    SinglyLinkedList(const SinglyLinkedList &List) : _chainIndex(List._chainIndex)
    {
        _head = nullptr, _listLength = 0; // _chainIndex = List._chainIndex,
        ListItem *prevItem = nullptr;
        for (ListItem *curItem = List._head; _listLength != List._listLength; curItem = curItem->next)
        {
            ListItem *newItem;
            // this->createItem(newItem);
            // if (!newItem)
            // {
            //     ListRemove();
            //     return;
            // }
            // newItem->_UserData = curItem->_UserData;
            // if (!_head)
            //     _head = newItem;
            // else
            //     prevItem->next = newItem;
            // prevItem = newItem;
            if (!(this->connectItem(newItem = this->createItem(&curItem->_UserData), prevItem)))
            {
                ListRemove();
                return;
            }
            prevItem = newItem;
        }
        if (_chainIndex == noLoop)
        {
            prevItem->next = nullptr;
        }
        else
        {
            if (_chainIndex + 1 == _listLength)
                prevItem->next = prevItem;
            else
                prevItem->next = this->appeal(_chainIndex);
        }
    }
    void ListRemove()
    {
        // if (_chainIndex != noLoop)
        //     for (ListItem *tmp = _head; _listLength; _head = _head->next, delete tmp, tmp = _head, _listLength--)
        //         ;
        // else
        //     for (ListItem *tmp = _head; _head; _head = _head->next, delete tmp, tmp = _head)
        //         ;
        for (ListItem *tmp = _head; _listLength; _head = _head->next, delete tmp, tmp = _head, _listLength--)
            ;
        _chainIndex = noLoop, _head = nullptr; //_listLength = Empty
    }
    void ListAddItem(const int addIndex, const UserData *_UserData)
    {
        if (_listLength >= addIndex)
        {
            ListItem *curItem = _head, *prevItem = nullptr;
            if (_head && addIndex)
            {
                for (int i = 0;; i++, prevItem = curItem, curItem = curItem->next)
                    if (i == addIndex)
                    {
                        // prevItem=&_head[addIndex-1], curItem=&_head[addIndex];
                        // ListItem *newItem;
                        // this->createItem(newItem);
                        // if (newItem)
                        // {
                        //     newItem->_UserData = *_UserData;
                        //     prevItem->next = newItem;
                        //     newItem->next = curItem;
                        // }
                        this->connectItem(this->createItem(_UserData), prevItem, curItem);
                        break;
                    }
            }
            else
            {
                ListItem *newItem;
                // this->createItem(newItem);
                // if (newItem)
                // {
                //     if (_head)
                //         newItem->next = _head;
                //     else
                //         newItem->next = nullptr;
                //     newItem->_UserData = *_UserData;
                //     _head = newItem;
                // }
                this->connectItem(newItem = this->createItem(_UserData), nullptr, _head);
                _head = newItem;
            }
        }
        else
            std::cerr << "\nError! Unsuccessful attempt ListAddItem\n";
    }
    void PushBack(const UserData *_UserData)
    {
        // ListItem *newItem;
        // this->createItem(newItem);
        // if (newItem)
        // {
        //     ListItem *curItem = _head;
        //     for (; curItem->next; curItem = curItem->next)
        //         ;
        //     curItem->next = newItem;
        //     newItem->next = nullptr;
        //     newItem->_UserData = *_UserData;
        // }
        // ListItem *curItem = _head;
        //     for (; curItem->next; curItem = curItem->next)
        //         ;
        this->connectItem(this->createItem(_UserData), this->appeal(lastItem));
    }
    void ListDeleteItem(const int delIndex, const int count = 1)
    {
        if (_listLength >= delIndex + count && _head)
        {
            ListItem *curItem = _head, *prevItem = nullptr;
            for (int index = 0; index < delIndex; index++, prevItem = curItem, curItem = curItem->next)
                ;
            if (curItem != _head)
            {
                for (int i = 0; i < count; i++, _listLength--, delete curItem)
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
                for (int i = 0; i < count; i++, _listLength--, delete curItem)
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
        if (_listLength <= index)
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
            for (ListItem *curItem = _head; i != _listLength; curItem = curItem->next, i++)
                if (curItem == findItem)
                    return i;
        }
        return -1;
    }
    int getChainIndex() const
    {
        return _chainIndex;
    }
    int getLength() const
    {
        return _listLength;
    }
    bool isEmpty() const
    {
        return !((bool)_head);
    }
    SinglyLinkedList operator=(const SinglyLinkedList &List)
    {
        SinglyLinkedList tmp(List);
        return tmp;
    }
    ListItem *operator[](const int index)
    {
        if (_listLength <= index)
            return nullptr;
        ListItem *curItem = this->_head;
        for (int i = 0; i < index; i++, curItem = curItem->next)
            ;
        return curItem;
    }
    // ListItem *operator<<(ListItem *List)
    // {
    //     List->_UserData.printData();
    //     return List;
    // }
    // void operator<<(const SinglyLinkedList &List)

protected:
    ListItem *newItem()
    {
        ListItem *newItem = new (std::nothrow) ListItem;
        return newItem;
    }
    ListItem *createItem(const UserData *_UserData)
    {
        ListItem *newItem;
        if (!(newItem = this->newItem()))
        {
            std::cerr << "\nне удалось добавить элемент\n";
        }
        else
            // _listLength++;
            newItem->_UserData = *_UserData;
        return newItem;
    }
    bool connectItem(ListItem *addedItem, ListItem *prevItem, ListItem *nextItem = nullptr)
    {
        if (addedItem)
        {
            if (!_head)
                _head = addedItem;
            else if (prevItem || nextItem)
            {
                if (prevItem)
                    prevItem->next = addedItem;
                addedItem->next = nextItem;
            }
            else
                return false;
            _listLength++;
            return true;
        }
        return false;
    }
};

void check(SinglyLinkedList list)
{
    if (!list.isEmpty())
    {
        if (list.searchCycle_v1() && list.searchCycle_v2())
        {
            std::cout << "\nList looped on index " << list.getIndexByAdress(list.searchCycle_v1()) << ", cycle start on "
                      << list.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList looped on index " << list.getIndexByAdress(list.searchCycle_v2()) << ", cycle start on "
                      << list.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList haven't loop\n";
#ifndef LoopChance0
            list.PrintList();
#endif
        }
    }
    else
    {
        std::cout << "\nList is Empty\n";
    }
}

int main()
{
    srand(time(NULL));
    SinglyLinkedList v1;
    if (!v1.isEmpty())
    {
        if (v1.searchCycle_v1() && v1.searchCycle_v2())
        {
            std::cout << "\nList v1 looped on index " << v1.getIndexByAdress(v1.searchCycle_v1()) << ", cycle start on "
                      << v1.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList v1 looped on index " << v1.getIndexByAdress(v1.searchCycle_v2()) << ", cycle start on "
                      << v1.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList haven't loop\n";
            v1.PrintList();
        }
    }
    else
    {
        std::cout << "\nList v1 is Empty\n";
    }

    UserData dataFor_v2[4]{1, 1, "dave", "1", 2, 2, "relLik", "13vova", 3, 3, "victor", "victor", 4, 4, "timopheii", "motoevacuator"};
    SinglyLinkedList v2(4, dataFor_v2);
    if (!v2.isEmpty())
    {
        if (v2.searchCycle_v1() && v2.searchCycle_v2())
        {
            std::cout << "\nList v2 looped on index " << v2.getIndexByAdress(v2.searchCycle_v1()) << ", cycle start on "
                      << v2.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList v2 looped on index " << v2.getIndexByAdress(v2.searchCycle_v2()) << ", cycle start on "
                      << v2.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList v2 haven't loop\n";
            v2.PrintList();
        }
    }
    else
    {
        std::cout << "\nList v2 is Empty\n";
    }

    UserData dataFor_v3{6, 6, ".net332", "kravl"};
    SinglyLinkedList v3(1, &dataFor_v3, 0);
    if (!v3.isEmpty())
    {
        if (v3.searchCycle_v1() && v3.searchCycle_v2())
        {
            std::cout << "\nList v3 looped on index " << v3.getIndexByAdress(v3.searchCycle_v1()) << ", cycle start on "
                      << v3.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList v3 looped on index " << v3.getIndexByAdress(v3.searchCycle_v2()) << ", cycle start on "
                      << v3.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList v3 haven't loop\n";
            v3.PrintList();
        }
    }
    else
    {
        std::cout << "\nList v3 is Empty\n";
    }

    UserData dataFor_v4[4]{11, 728, "dkwjqe312ed", "iordan", 12, 732, "00elda2e9iu", "kenny", 13, 733, "u29we29ikod", "reqi", 14, 671, "koaskdo201s", "greg"};
    SinglyLinkedList v4(4, dataFor_v4, 3);
    if (!v4.isEmpty())
    {
        if (v4.searchCycle_v1() && v4.searchCycle_v2())
        {
            std::cout << "\nList v4 looped on index " << v4.getIndexByAdress(v4.searchCycle_v1()) << ", cycle start on "
                      << v4.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList v4 looped on index " << v4.getIndexByAdress(v4.searchCycle_v2()) << ", cycle start on "
                      << v4.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList v4 haven't loop\n";
            v4.PrintList();
        }
    }
    else
    {
        std::cout << "\nList v4 is Empty\n";
    }

    // UserData dataFor_v5[5]{/29, 039, "she2203cWWxR", "pixie", /39, '9', "oPPOrkk92+fdI", "menelay", 034, 377, "3o2rgir6tcK2", "tryto66", /377, 665, "D44oFouiDDie", "devilJIn", 01, 02, "emwii9$99go3", "piNklove66"};
    UserData dataFor_v5[5]{291, 265, "she203cWWxR", "pixie", 39, 9, "oPrkk92+fdI", "menelay", 34, 374, "3o2rgir6tK2", "tryto66", 377, 665, "D44FouiDDie", "devilJIn", 8, 16, "emwii9$9go3", "piNklove66"};
    SinglyLinkedList v5(5, dataFor_v5, 3);
    if (!v5.isEmpty())
    {
        if (v5.searchCycle_v1() && v5.searchCycle_v2())
        {
            std::cout << "\nList v5 looped on index " << v5.getIndexByAdress(v5.searchCycle_v1()) << ", cycle start on "
                      << v5.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList v5 looped on index " << v5.getIndexByAdress(v5.searchCycle_v2()) << ", cycle start on "
                      << v5.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList v5 haven't loop\n";
            v5.PrintList();
        }
    }
    else
    {
        std::cout << "\nList v5 is Empty\n";
    }

    // UserData dataFor_v6[3]{276, 637, "49fhirejFJ", "unXe8ston", /37, 49, "s,394OOex-w23", "ieSsaiLAjdRcK", 812, 812,"ief31xcz????", "Ulu992w"};
    UserData dataFor_v6[3]{276, 637, "49fhirejFJ5", "unXe8ston", 137, 49, "s,94Oex-w23", "ieSsaiLAjdRcK", 812, 812, "ie31xcz????", "Ulu992w"};
    SinglyLinkedList v6(3, dataFor_v6, 0);
    if (!v6.isEmpty())
    {
        if (v6.searchCycle_v1() && v6.searchCycle_v2())
        {
            std::cout << "\nList v6 looped on index " << v6.getIndexByAdress(v6.searchCycle_v1()) << ", cycle start on "
                      << v6.searchCycle_v1() << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList v6 looped on index " << v6.getIndexByAdress(v6.searchCycle_v2()) << ", cycle start on "
                      << v6.searchCycle_v2() << '\t' << " detected by searchCycle_v2\n";
        }
        else
        {
            std::cout << "\nList v6 haven't loop\n";
            v6.PrintList();
        }
    }
    else
    {
        std::cout << "\nList v6 is Empty\n";
    }
    std::cout << "\nLov66 " << v6[1];

        // тут я понял, что метод searchCycle_v2() работает неверно в большинстве случаев, а мне пару раз повезло.
#ifdef LoopChance100
    std::cout << '\n'
              << '\n';
    UserData specialTestData[14]{0, 0, "0", "0", 1, 1, "1", "1", 2, 2, "2", "2", 3, 3, "3", "3", 4, 4, "4", "4", 5, 5, "5", "5", 6, 6, "6", "6", 7, 7, "7", "7",
                                 8, 8, "8", "8", 9, 9, "9", "9", 10, 10, "10", "10", 11, 11, "11", "11", 12, 12, "12", "12", 13, 13, "13", "13"};
    for (int i = 0; i < 14; i++)
    {
        check(SinglyLinkedList(14, specialTestData, i));
    }
    // проверка работы конструктора копирования
    for (int i = 0; i < 14; i++)
    {
        SinglyLinkedList tmp(14, specialTestData, i);
        check(tmp);
    }
#endif
    return 0;
}