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
    if(isFull()) return false;

    if(rear != 0) rear = nextIndex(rear);
        
    data[rear] = req;
    count++;
    return true;

    // (void)req; // suppress unused warning until implemented
    // return false;
}

bool RequestQueue::dequeue(Request& outReq) {
    //TODO: Implement dequeue function as explained in the PDF.

    if(isEmpty()) return false;

    peek(outReq);

    if(front == rear)
    {
        clear();
        return true;
    }


    front = nextIndex(front);
    return true;

    // (void)outReq; // suppress unused warning until implemented
    // return false;
}

bool RequestQueue::peek(Request& outReq) const {
    //TODO: Implement peek function as explained in the PDF.

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

    if(isEmpty()) return false;

    int index = front;
    bool isFound = false;

    while (true)
    {
        if (data[index].getId() == id)
        {
            isFound = true;
            while (true)
            {
                if (index == rear) break;
                data[index] = data[nextIndex(index)];
                index = nextIndex(index);
            } 
            if (count == 1)
            {
                clear();
                return true;
            }
            if (rear == 0) rear = capacity - 1;
            else rear--;
            count--;
            break;
        }
        if (index == rear) return false;
        index = nextIndex(index);
    }
    if(isFound) return true;
    else return false;
    
    // (void)id; // suppress unused warning until implemented
    // return false;
}

bool RequestQueue::resize(int newCapacity) {
    //TODO: Implement resize function as explained in the PDF.
    return true;
}
