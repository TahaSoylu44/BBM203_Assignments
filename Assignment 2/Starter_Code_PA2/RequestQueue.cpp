#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //TODO: Implement enqueue function as explained in the PDF.
    if (data == nullptr) return false;

    if(isFull()) resize(2 * capacity);

    if (isEmpty())
    {
        rear = 0;
        front = 0;
        data[rear] = req;
        count++;
        return true;
    }
    
    rear = nextIndex(rear);
    data[rear] = req;

    count++;
    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    //TODO: Implement dequeue function as explained in the PDF.
    if (data == nullptr) return false;

    if(isEmpty()) return false;

    peek(outReq);

    if(front == rear)
    {
        clear();
        return true;
    }

    front = nextIndex(front);
    count--;
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //TODO: Implement peek function as explained in the PDF.
    if (data == nullptr) return false;

    if(isEmpty()) return false;

    outReq = data[front];

    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //TODO: Implement removeById function as explained in the PDF.
    if (data == nullptr) return false;

    if(isEmpty()) return false;

    int index = front;
    bool isFound = false;

    while (true)
    {
        if (data[index].getId() == id)  //Gerçekten böyle bir id var mı?
        {
            isFound = true;
            while (true)
            {
                if (index == rear) break;   //rear a dokunma
                data[index] = data[nextIndex(index)];   //sola kaydır
                index = nextIndex(index);
            } 
            if (count == 1) //tek bir eleman var ve o da bizim aradığımız   
            {
                clear();
                return true;
            }
            if (rear == 0) rear = capacity - 1;
            else rear--;    //rear bir öne gelsin
            count--;    //eleman sayısı azaldı
            break;
        }
        if (index == rear) return false;    //rear a kadar aradım ama bulamadım,demek ki yok
        index = nextIndex(index);
    }
    if(isFound) return true;
    else return false;
}

bool RequestQueue::resize(int newCapacity) {
    //TODO: Implement resize function as explained in the PDF.

    if (newCapacity < size()) return false; //bi zahmet yeni kapasite fazla olsun

    Request* tmp = new (std::nothrow) Request[newCapacity];

    if(tmp == nullptr) return false;

    for (int i = 0; i < size(); i++)
    {
        tmp[i] = data[(front + i) % capacity];
    }

    delete[] data;
    data = tmp;

    front = 0;
    rear = size() - 1;
    capacity = newCapacity;
    
    return true;
}