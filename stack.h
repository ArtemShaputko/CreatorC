#ifndef STACK_H
#define STACK_H

#include <exception>
#include <string>
using namespace std;

class StackException : public exception {
private:
    string message;

public:
    StackException(string msg) : message(msg) {}
    const char * what () const noexcept override
    {
        return message.c_str();
    }
};

template<class T>
class Stack
{
    class Node;
public:
    class Iterator;
    Stack() : head(nullptr), tail(nullptr), size(0) {}

    bool isEmpty() const;
    T getTop() const;
    int getSize() const;
    void push(T element);
    T pop();
    Iterator begin() const;
    Iterator end() const;
    void clear();
    void remove(Iterator end);

    ~Stack();

private:
    Node * head;
    Node * tail;
    int size;

private:
    class Node
    {
        friend class Stack;
    private:
        Node * next;
        Node * prev;
        T value;
    public:
        Node( T _value) : next(nullptr), prev(nullptr), value(_value) {}
        ~Node() = default;
    };

public:
    class Iterator
    {
        friend class Stack;
    private:
        Node * current;
    public:
        explicit Iterator(Node * _current = nullptr): current(_current){}
        Iterator(const Iterator& right)
        {
            this->current = right.current;
        }

        T base() const
        {
            return this->current->value;
        }

        Iterator operator+(int n)
        {
            Iterator buf = *this;
            for(int i = 0; i < n; i++) buf++;
            return buf;
        }

        Iterator operator-(int n)
        {
            Iterator buf = *this;
            for(int i = 0; i < n; i++) buf--;
            return buf;
        }

        Iterator& operator++()
        {
            Iterator prev = *this;
            if(current==nullptr) throw StackException("Iterator out of range");
            current = current->next;
            return prev;
        }

        Iterator& operator--()
        {
            Iterator prev = *this;
            if(current==nullptr) throw StackException("Iterator out of range");
            current = current->prev;
            return prev;
        }

        Iterator& operator++(int)
        {
            if(current==nullptr) throw StackException("Iterator out of range");
            current = current->next;
            return *this;
        }

        Iterator& operator--(int)
        {
            if(current==nullptr) throw StackException("Iterator out of range");
            current = current->prev;
            return *this;
        }

        Iterator& operator=(const Iterator& right)
        {
            if(this==&right) return *this;
            this->current = right.current;
            return *this;
        }

        T* operator->()
        {
            return &(this->current->value);
        }

        T& operator*()
        {
            return &(this->current->value);
        }

        bool operator==(const Iterator& right)
        {
            return (this->current == right.current);
        }

        bool operator!=(const Iterator& right)
        {
            return (this->current != right.current);
        }
    };
};

template<class T>
bool Stack<T>::isEmpty() const
{
    if(head==tail) return true;
    return false;
}

template<class T>
T Stack<T>::getTop() const
{
    if(head == nullptr) throw StackException("Stack is empty");
    return head->value;
}

template<class T>
int Stack<T>::getSize() const
{
    return size;
}

template<class T>
void Stack<T>::push(T element)
{
    Node * buf = new Node(element);
    buf->next = head;
    if(isEmpty())
    {
        tail = new Node(element);
        buf->next = tail;
    }
    head = buf;
    head->next->prev = buf;
    size++;
}

template<class T>
T Stack<T>::pop()
{
    if(head == tail)
        throw StackException("Stack is empty");
    Node * buf;
    T bufValue;
    buf = head;
    head = head->next;
    head->prev = nullptr;
    bufValue = buf->value;
    if(head == tail)
    {
        delete tail;
        head = tail = nullptr;
    }
    delete buf;
    size--;
    return bufValue;
}

template<class T>
typename Stack<T>::Iterator Stack<T>::begin() const
{
    return Iterator(head);
}

template<class T>
typename Stack<T>::Iterator Stack<T>::end() const
{
    return Iterator(tail);
}

template<class T>
void Stack<T>::clear()
{
    Node * cur = head;
    Node * buf;
    while(cur != nullptr)
    {
        buf = cur;
        cur = cur->next;
        delete buf;
    }
    head = tail = nullptr;
}

template<class T>
void Stack<T>::remove(Iterator end)
{
    if(end == this->begin()) return;
    if(end==this->end())
    {
        clear();
        return;
    }
    auto it = this->begin()+1;
    for(; it !=end+1;it++)
    {
        delete it.current->prev;
    }
    head = end.current;
}

template<class T>
Stack<T>::~Stack()
{
    clear();
}

#endif // STACK_H
