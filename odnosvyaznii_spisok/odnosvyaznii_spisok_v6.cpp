//#define LoopChance100 //генерирует цикл + проверка на 488-491 и проверка работы конструктора
#define LoopChance50 //генерирует цикл с вероятностью 50/50
//#define LoopChance0           //без цикла

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <array>

#define noLoop -1
#define Empty 0
#define lastItem (_listLength - 1)

struct UserData
{
    int id;
    int developer_id;
    char code[12];
    char name[45];
    // void printData() const
    // {
    //     std::cout << "Worker name: " << name << " developer id = " << developer_id << " code: " << code << '\n';
    // }
};

struct ListItem
{
    UserData _UserData;
    ListItem *next;
    ListItem &operator=(const ListItem &List)
    {
        this->_UserData = List._UserData;
        return *this;
    }
};

// std::ostream &operator<<(std::ostream &out, const ListItem *List)
// {
//     return (out << "Worker name: " << List->_UserData.name << " developer id = " << List->_UserData.developer_id << " code: " << List->_UserData.code);
// }
std::ostream &operator<<(std::ostream &out, const ListItem &List)
{
    return (out << "Worker name: " << List._UserData.name << " developer id = " << List._UserData.developer_id << " code: " << List._UserData.code);
}

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
                if (!(this->connectItem(newItem = this->createItem(&_UserData[i]), prevItem, nullptr)))
                {
                    ListRemove();
                    return;
                }
                prevItem = newItem;
            }
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
                            prevItem->next = this->appeal(LoopedOnID);
                    }
        }
    }
    ~SinglyLinkedList()
    {
        ListRemove();
    }
    SinglyLinkedList(const SinglyLinkedList &List) : _chainIndex(List._chainIndex)
    {
        _head = nullptr, _listLength = 0;
        ListItem *prevItem = nullptr;
        for (ListItem *curItem = List._head; _listLength != List._listLength; curItem = curItem->next)
        {
            ListItem *newItem;
            if (!(this->connectItem(newItem = this->createItem(&curItem->_UserData), prevItem, nullptr)))
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
        for (ListItem *tmp = _head; _listLength; _head = _head->next, delete tmp, tmp = _head, _listLength--)
            ;
        _chainIndex = noLoop, _head = nullptr;
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
                        this->connectItem(this->createItem(_UserData), prevItem, curItem);
                        break;
                    }
            }
            else
            {
                ListItem *newItem;
                this->connectItem(newItem = this->createItem(_UserData), nullptr, _head);
                _head = newItem;
            }
        }
        else
            std::cerr << "\nError! Unsuccessful attempt ListAddItem\n";
    }
    void PushBack(const UserData *_UserData)
    {
        this->connectItem(this->createItem(_UserData), this->appeal(lastItem), nullptr);
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
                        curItem = prevItem->next;
                    prevItem->next = curItem->next;
                }
            }
            else
            {
                for (int i = 0; i < count; i++, _listLength--, delete curItem)
                {
                    curItem = _head;
                    _head = _head->next;
                }
            }
        }
        else
            std::cerr << "\nError! Unsuccessful attempt ListDeleteItem\n";
    }
    SinglyLinkedList *swap(SinglyLinkedList &List) noexcept
    {
        ListItem *_head_tmp = List._head;
        int _chainIndex_tmp = List._chainIndex;
        int _listLength_tmp = List._listLength;
        List._head = this->_head;
        List._chainIndex = this->_chainIndex;
        List._listLength = this->_listLength;
        this->_head = _head_tmp;
        this->_chainIndex = _chainIndex_tmp;
        this->_listLength = _listLength_tmp;
        return this;
    }
    void PrintList() const
    {
        // for (ListItem *curItem = _head; curItem; curItem = curItem->next)
        // curItem->_UserData.printData();
        for (int i = 0; i < _listLength; i++)
            std::cout << *(*this)[i] << '\n';
        // std::cout << this->appeal(i);
    }
    void PrintListWithIndex(const int index) const
    {
        // ListItem *curItem = this->appeal(index);
        // if (curItem)
        //     // curItem->_UserData.printData();
        //     std::cout<<curItem;
        // else
        //     std::cerr << "\nError! Unsuccessful attempt PrintListWithIndex\n";
        if (_listLength > index)
            std::cout << *(*this)[index];
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
    ListItem *searchCycle_v2w1() const
    {
        if (_head && _head->next)
        {
            ListItem *slow = _head->next, *fast = _head->next->next;
            while (slow != fast)
                if (fast && fast->next)
                    slow = slow->next, fast = fast->next->next;
                else
                    return nullptr;
            ListItem *detecter = _head;
            while (detecter != slow)
            {
                slow = slow->next;
                if (slow == fast)
                    detecter = detecter->next;
            }
            return slow;
        }
        return nullptr;
    }
    ListItem *searchCycle_v2w2() const
    {
        if (_head && _head->next)
        {
            ListItem *slow = _head->next, *fast = _head->next->next;
            while (slow != fast)
                if (fast && fast->next)
                    slow = slow->next, fast = fast->next->next;
                else
                    return nullptr;
            for (slow = _head; slow != fast; slow = slow->next, fast = fast->next)
                ;
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
    SinglyLinkedList &operator=(const SinglyLinkedList &List)
    {
        if (this != &List)
            SinglyLinkedList(List).swap(*this);
        return *this;
    }
    ListItem *operator[](int index) const
    {
        if (_listLength <= index)
        {
            std::cerr << "\nATTENTION! Requesting outside the list\n";
            return nullptr;
        }
        ListItem *curItem = this->_head;
        for (int i = 0; i < index; i++, curItem = curItem->next)
            ;
        return curItem;
    }
    friend std::ostream &operator<<(std::ostream &out, const ListItem &List);

protected:
    // ListItem *createItem(const UserData *_UserData = nullptr) чтобы можно было создавать пустой
    ListItem *createItem(const UserData *_UserData)
    {
        ListItem *newItem = new (std::nothrow) ListItem;
        if (!newItem)
        {
            std::cerr << "\nATTANTION! Unable to add Item\n";
        }
        else
            newItem->_UserData = *_UserData;
        return newItem;
    }
    bool connectItem(ListItem *addedItem, ListItem *prevItem, ListItem *nextItem)
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
            {
                std::cerr << "\nATTENTION! Invalid call to the function connectItem\n";
                return false;
            }
            _listLength++;
            return true;
        }
        std::cerr << "\nATTENTION! Invalid call to the function connectItem\n";
        return false;
    }
};

void check(const SinglyLinkedList &list)
{
    if (!list.isEmpty())
    {
        ListItem *res1, *res2;
        if ((res1 = list.searchCycle_v1()) && (res2 = list.searchCycle_v2w2()))
        {
            std::cout << "\nList looped on index " << list.getIndexByAdress(res1) << ", cycle start on "
                      << res1 << '\t' << " detected by searchCycle_v1";
            std::cout << "\nList looped on index " << list.getIndexByAdress(res2 = list.searchCycle_v2w2()) << ", cycle start on "
                      << res2 << '\t' << " detected by searchCycle_v2w2";
#ifdef LoopChance100
            std::cout << "\nList looped on index " << list.getIndexByAdress(res2 = list.searchCycle_v2()) << ", cycle start on "
                      << res2 << '\t' << " detected by searchCycle_v2(ERR)";
            std::cout << "\nList looped on index " << list.getIndexByAdress(res2 = list.searchCycle_v2w1()) << ", cycle start on "
                      << res2 << '\t' << " detected by searchCycle_v2w1";
#endif
            std::cout << '\n';
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
    check(v1);

    UserData dataFor_v2[4]{1, 1, "dave", "1", 2, 2, "relLik", "13vova", 3, 3, "victor", "victor", 4, 4, "timopheii", "motoevacuator"};
    SinglyLinkedList v2(4, dataFor_v2);
    check(v2);

    UserData dataFor_v3{6, 6, ".net332", "kravl"};
    SinglyLinkedList v3(1, &dataFor_v3, 0);
    check(v3);

    UserData dataFor_v4[4]{11, 728, "dkwjqe312ed", "iordan", 12, 732, "00elda2e9iu", "kenny", 13, 733, "u29we29ikod", "reqi", 14, 671, "koaskdo201s", "greg"};
    SinglyLinkedList v4(4, dataFor_v4, 3);
    check(v4);

    UserData dataFor_v5[5]{291, 265, "she203cWWxR", "pixie", 39, 9, "oPrkk92+fdI", "menelay", 34, 374, "3o2rgir6tK2", "tryto66", 377, 665, "D44FouiDDie", "devilJIn", 8, 16, "emwii9$9go3", "piNklove66"};
    SinglyLinkedList v5(5, dataFor_v5, 3);
    check(v5);

    UserData dataFor_v6[3]{276, 637, "49fhirejFJ5", "unXe8ston", 137, 49, "s,94Oex-w23", "ieSsaiLAjdRcK", 812, 812, "ie31xcz????", "Ulu992w"};
    SinglyLinkedList v6(3, dataFor_v6, 0);
    check(v6);

    std::cout << "\nLov66 "
              << *v6[1] << '\n';
    SinglyLinkedList v7, v8, v9 = v5;
    v7 = v9 = v8 = v6;
    std::cout << *v7[0] << '\n';
    v7.PrintListWithIndex(2);
    std::cout << '\n';
    (v9 = v3).PrintList();
    std::cout << '\n'
              << *v7[2] << '\n'
              << *v9[0] << '\n'
              << (*v7[2] = *v9[0]) << '\n';
    // std::cout << '\n';
    // v6[2]->_UserData.printData();

    // тут я понял, что метод searchCycle_v2() работает неверно в большинстве случаев, а мне пару раз повезло.
#ifdef LoopChance100
    std::cout << '\n';
    UserData specialTestData[14]{0, 0, "0", "0", 1, 1, "1", "1", 2, 2, "2", "2", 3, 3, "3", "3", 4, 4, "4", "4", 5, 5, "5", "5", 6, 6, "6", "6", 7, 7, "7", "7",
                                 8, 8, "8", "8", 9, 9, "9", "9", 10, 10, "10", "10", 11, 11, "11", "11", 12, 12, "12", "12", 13, 13, "13", "13"};
    for (int i = 0; i < 14; i++)
    {
        check(SinglyLinkedList(14, specialTestData, i));
    }
#endif
    return 0;
}