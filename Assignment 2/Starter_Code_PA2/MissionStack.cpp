#include "MissionStack.h"
#include <new>     // for std::nothrow

MissionStack::MissionStack()
    : data(nullptr),
      capacity(0),
      top(-1) {
    resize(4);
}

MissionStack::MissionStack(int initialCapacity)
    : data(nullptr),
      capacity(0),
      top(-1) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

MissionStack::~MissionStack() {
    delete[] data;
}

bool MissionStack::isEmpty() const {
    return top == -1;
}

int MissionStack::size() const {
    return top + 1;
}

bool MissionStack::push(const Request& req) {
    //TODO: Implement push function as explained in the PDF.
    if (data == nullptr) return false;

    if (size() == capacity)
    {
        if (!resize(2 * capacity))
        {
            return false;   //hani oldu da resize yapamadım,ne olur ne olmaz
        }
    }
    
    data[++top] = req;
    return true;
}

bool MissionStack::pop(Request& outReq) {
    //TODO: Implement pop function as explained in the PDF.
    if (data == nullptr) return false;

    if(isEmpty()) return false;

    outReq = data[top--];
    return true;
}

bool MissionStack::peek(Request& outReq) const {
    //TODO: Implement peek function as explained in the PDF.

    if (data == nullptr) return false;

    if(isEmpty()) return false;

    outReq = data[top];

    return true;
}

void MissionStack::clear() {
    top = -1;
}

bool MissionStack::resize(int newCapacity) {
    //TODO: Implement resize function as explained in the PDF.

    if (newCapacity < size()) return false;

    Request* tmp = new (std::nothrow) Request[newCapacity];

    if (tmp == nullptr) return false;

    for (int i = 0; i <= top; i++)
    {
        tmp[i] = data[i];
    }

    delete[] data;

    data = tmp;
    capacity = newCapacity;
    
    return true;
}
