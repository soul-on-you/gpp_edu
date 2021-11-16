#include <iostream>
#define v1
#ifdef v1
struct ListItem
{
    short value;
    ListItem *next;
};

class MyLinkedList
{
    ListItem *head;
    ListItem *end;
    int length;

public:
    MyLinkedList()
    {
        length = 0;
        head = nullptr;
    }

    int get(int index)
    {
        if (index < length && length > 0 && head)
        {
            ListItem *curItem = this->head;
            for (int i = 0; i < index; i++, curItem = curItem->next)
                ;
            return curItem->value;
        }
        else
            return -1;
    }

    void addAtHead(int val)
    {
        if (!length)
        {
            head = new ListItem;
            head->value = val;
            head->next = nullptr;
            length++;
            end = head;
        }
        else
        {
            ListItem *newItem = new ListItem;
            length++;
            newItem->value = val;
            newItem->next = head;
            head = newItem;
        }
    }

    void addAtTail(int val)
    {
        if (!length)
        {
            head = new ListItem;
            head->value = val;
            head->next = nullptr;
            length++;
            end = head;
        }
        else
        {
            ListItem *newItem = new ListItem;
            length++;
            newItem->value = val;
            newItem->next = nullptr;
            end->next = newItem;
            end = newItem;
        }
    }

    void addAtIndex(int index, int val)
    {
        if (!length && !index)
        {
            head = new ListItem;
            head->value = val;
            head->next = nullptr;
            length++;
            end = head;
        }
        else if (index == 0)
        {
            addAtHead(val);
        }
        else if (index == length)
        {
            addAtTail(val);
        }
        else if (index < length)
        {
            ListItem *curItem = head;
            index -= 1;
            for (int i = 0; i < index; i++, curItem = curItem->next)
                ;
            ListItem *newItem = new ListItem;
            length++;
            newItem->value = val;
            newItem->next = curItem->next;
            curItem->next = newItem;
        }
    }

    void deleteAtIndex(int index)
    {
        if (index < length)
        {
            if (index == 0)
            {
                // std::cout << '+';
                if (length > 1)
                {
                    // ListItem *prev = head;
                    // std::cout << "prev:" << head << '\n';
                    // head = head->next;
                    // std::cout << "cur:" << head << '\n';
                    // delete prev;
                    // std::cout << "reply cur:" << head << '\n';
                    ListItem *next = this->head->next;
                    delete this->head;
                    this->head = next;
                }
                else
                {
                    delete this->head;
                    this->head = nullptr;
                }
            }
            if (index < length && index >= 1)
            {
                ListItem *curItem = head, *prevItem = head;
                for (int i = 0; i < index; i++, prevItem = curItem, curItem = curItem->next)
                    ;
                prevItem->next = curItem->next;
                if (index + 1 == length)
                    end = prevItem;
                delete curItem;
            }
            length--;
            // std::cout << "Length now: " << length << '\n';
        }
    }
};

int main()
{
    // ["MyLinkedList","addAtHead","addAtTail","addAtIndex","get","deleteAtIndex","get"]
    // [[],[1],[3],[1,2],[1],[1],[1]]

    MyLinkedList *obj = new MyLinkedList();
    obj->addAtHead(1);
    obj->addAtTail(3);
    obj->addAtIndex(1, 2);
    std::cout << obj->get(0) << "->" << obj->get(1) << "->" << obj->get(2) << "->" << obj->get(3) << '\n';
    int param_1 = obj->get(1);
    obj->deleteAtIndex(1);
    int param_2 = obj->get(1);
    std::cout << param_1 << '\t' << param_2 << '\t' << '\n';
    std::cout << obj->get(0) << "->" << obj->get(1) << "->" << obj->get(2) << "->" << obj->get(3) << '\n';

    // ["MyLinkedList","addAtHead","addAtHead","addAtHead","addAtIndex","deleteAtIndex","addAtHead","addAtTail","get","addAtHead","addAtIndex","addAtHead"]
    // [[],[7],[2],[1],[3,0],[2],[6],[4],[4],[4],[5,0],[6]]
    MyLinkedList *obj2 = new MyLinkedList();
    obj2->addAtHead(7);
    obj2->addAtHead(2);
    obj2->addAtHead(1);
    obj2->addAtIndex(3, 0);
    obj2->deleteAtIndex(2);
    obj2->addAtHead(6);
    obj2->addAtTail(4);
    std::cout << obj2->get(0) << "->" << obj2->get(1) << "->" << obj2->get(2) << "->" << obj2->get(3) << '\n';
    std::cout << obj2->get(4) << '\n';
    obj2->addAtHead(4);
    obj2->addAtIndex(5, 0);
    obj2->addAtHead(6);
    std::cout << obj2->get(0) << "->" << obj2->get(1) << "->" << obj2->get(2) << "->" << obj2->get(3) << obj2->get(4) << "->" << obj2->get(5) << "->" << obj2->get(6) << "->" << obj2->get(7) << '\n';

    MyLinkedList *obj3 = new MyLinkedList();
    obj3->addAtHead(1);
    obj3->deleteAtIndex(0);
    std::cout << obj3->get(0) << '\n';

    MyLinkedList *obj4 = new MyLinkedList();
    // [ "MyLinkedList", "addAtIndex", "addAtIndex", "addAtIndex", "get" ]
    //     [[], [ 0, 10 ], [ 0, 20 ], [ 1, 30 ], [0]]
    obj4->addAtIndex(0, 10);
    obj4->addAtIndex(0, 20);
    obj4->addAtIndex(1, 30);
    std::cout << obj4->get(0) << '\n';
    std::cout << obj4->get(0) << "->" << obj4->get(1) << "->" << obj4->get(2) << "->" << obj4->get(3) << '\n';

    //  "addAtHead", "addAtHead", "addAtTail", "get", "deleteAtIndex", "deleteAtIndex"]
    // [[], [2], [1], [2], [7], [3], [2], [5], [5], [5], [6], [4]]
    MyLinkedList *obj5 = new MyLinkedList();
    obj5->addAtHead(2);
    obj5->deleteAtIndex(1);
    obj5->addAtHead(2);
    obj5->addAtHead(7);
    obj5->addAtHead(3);
    obj5->addAtHead(2);
    obj5->addAtHead(5);
    obj5->addAtTail(5);
    obj5->get(5);
    ;
    obj5->deleteAtIndex(6);
    obj5->deleteAtIndex(4);

    // ["MyLinkedList", "addAtHead", "addAtIndex", "addAtTail", "addAtHead", "addAtIndex", "addAtTail", "addAtTail", "addAtIndex", "deleteAtIndex", "deleteAtIndex", "addAtTail"]
    // [[], [0], [1, 4], [8], [5], [4, 3], [0], [5], [6, 3], [7], [5], [4]]

    return 0;
}
#else

struct ListItem
{
    short value;
    ListItem *next;
};

class MyLinkedList
{
    ListItem *head;
    int length;

public:
    MyLinkedList()
    {
        length = 0;
        head = nullptr;
    }

    int get(int index)
    {
        if (index < length)
        {
            ListItem *curItem = head;
            for (int i = 0; i < index; i++, curItem = curItem->next)
                ;
            return curItem->value;
        }
        else
            return -1;
    }

    void addAtHead(int val)
    {
        if (!length)
        {
            head = new ListItem;
            head->value = val;
            head->next = nullptr;
            length++;
        }
        else
        {
            ListItem *newItem = new ListItem;
            length++;
            newItem->value = val;
            newItem->next = head;
            head = newItem;
        }
    }

    void addAtTail(int val)
    {
        if (!length)
        {
            head = new ListItem;
            head->value = val;
            head->next = nullptr;
            length++;
        }
        else
        {
            ListItem *curItem = head;
            for (int i = 0; i < length; i++, curItem = curItem->next)
                ;
            ListItem *newItem = new ListItem;
            length++;
            newItem->value = val;
            newItem->next = nullptr;
        }
    }

    void addAtIndex(int index, int val)
    {
        if (!length)
        {
            head = new ListItem;
            head->value = val;
            head->next = nullptr;
            length++;
        }
        else if (index + 1 < length)
        {
            ListItem *curItem = head;
            index -= 1;
            for (int i = 0; i < index; i++, curItem = curItem->next)
                ;
            ListItem *newItem = new ListItem;
            length++;
            newItem->value = val;
            newItem->next = curItem->next;
            curItem->next = newItem;
        }
    }

    void deleteAtIndex(int index)
    {
        if (index < length)
        {
            ListItem *curItem = head, *prevItem = head;
            for (int i = 0; i < index; i++, prevItem = curItem, curItem = curItem->next)
                ;
            prevItem->next = curItem->next;
            delete curItem;
            length--;
        }
    }
};

int main()
{
    // ["MyLinkedList","addAtHead","addAtTail","addAtIndex","get","deleteAtIndex","get"]
    // [[],[1],[3],[1,2],[1],[1],[1]]

    MyLinkedList *obj = new MyLinkedList();
    obj->addAtHead(1);
    std::cout << '+';
    obj->addAtTail(3);
    std::cout << '+';
    obj->addAtIndex(1, 2);
    std::cout << '+';
    std::cout << obj->get(0) << "->" << obj->get(1) << "->" << obj->get(2) << "->" << obj->get(3) << '\n';
    int param_1 = obj->get(1);
    obj->deleteAtIndex(1);
    int param_2 = obj->get(1);
    std::cout << param_1 << '\t' << param_2 << '\n';

    // ["MyLinkedList","addAtHead","addAtHead","addAtHead","addAtIndex","deleteAtIndex","addAtHead","addAtTail","get","addAtHead","addAtIndex","addAtHead"]
    // [[],[7],[2],[1],[3,0],[2],[6],[4],[4],[4],[5,0],[6]]
    MyLinkedList *obj2 = new MyLinkedList();
    obj2->addAtHead(7);
    obj2->addAtHead(2);
    obj2->addAtHead(1);
    obj2->addAtIndex(3, 0);
    obj2->deleteAtIndex(2);
    obj2->addAtHead(6);
    obj2->addAtTail(4);
    std::cout << obj2->get(0) << "->" << obj2->get(1) << "->" << obj2->get(2) << "->" << obj2->get(3) << '\n';
    std::cout << obj2->get(4) << '\n';
    obj2->addAtHead(4);
    obj2->addAtIndex(5, 0);
    obj2->addAtHead(6);
    std::cout << obj2->get(0) << "->" << obj2->get(1) << "->" << obj2->get(2) << "->" << obj2->get(3) << obj2->get(4) << "->" << obj2->get(5) << "->" << obj2->get(6) << "->" << obj2->get(7) << '\n';
    return 0;
}
#endif