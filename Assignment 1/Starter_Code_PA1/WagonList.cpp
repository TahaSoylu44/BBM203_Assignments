#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.
    if(front == nullptr)
    {
        totalWeight = 0;
        return;
    }

    Wagon* current = front;

    while (current)
    {
        Wagon* next = current->next;
        totalWeight -= current->getWeight();
        delete current;
        current = next;
    }
    front = nullptr;
    rear = nullptr;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state

    if(this == &other) return;
    this->front = other.front;
    this->rear = other.rear;
    this->totalWeight = other.totalWeight;

    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.

    if (this != &other)
    {
        clear();

        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;

        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
    }
    return *this;
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.

    Wagon* tmp = front;

    while (tmp)
    {
        if (tmp->getID() == id)
        {
            return tmp;
        }
        tmp = tmp->next;   
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight

    if(front == nullptr)
    {
        front = w;
        rear = w;
        totalWeight = w->getWeight();
        return;
    }

    Wagon* tmp = front;

    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->setNext(w);
    w->setPrev(tmp);

    rear = w;
    totalWeight += w->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    // TODO: Return true if the list has no wagons.
    return front == nullptr;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.

    if (front == nullptr)
    {
        front = wagon;
        rear = wagon;
        totalWeight = wagon->getWeight();
        return;
    }
    
    Wagon* tmp = front;

    if ((wagon->getWeight()) >= (tmp->getWeight()))
    {
        wagon->setNext(tmp);
        tmp->setPrev(wagon);

        totalWeight += wagon->getWeight();

        front = wagon;
        return;
    }

    while (tmp->next)
    {
        if ((wagon->getWeight()) >= ((tmp->getNext())->getWeight()))
        {
            wagon->setNext(tmp->next);
            (tmp->next)->setPrev(wagon);
            wagon->setPrev(tmp);
            tmp->setNext(wagon);

            totalWeight += wagon->getWeight();
            
            return;
        }
        tmp = tmp->next;
    }
    //If the wagon escapes the while loop,it means it is the smallest one

    tmp->setNext(wagon);
    wagon->setPrev(tmp);
    wagon->setNext(nullptr);

    rear = wagon;
    totalWeight += wagon->getWeight();
}

void WagonList::appendList(WagonList &&other)
{
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list

   if(other.front == nullptr) return;

   if (this->front == nullptr)
   {
    *this = std::move(other);
    return;
   }

//    bool isOtherHazardous = (other.getFront())->getCargoType() == parseCargo("HAZARDOUS");
//    bool isThisHazardous = (this->getFront())->getCargoType() == parseCargo("HAZARDOUS");

   bool isOtherGreater = other.maxWeight() > this->maxWeight();

//    if (isOtherHazardous)
//    {
//     (this->rear)->setNext(other.front); //"This" is first
//     (other.front)->setPrev(this->rear);
//     (this->rear) = other.rear;
//    }
//    else if (isThisHazardous)
//    {
//     (other.rear)->setNext(this->front); //Other is first
//     (this->front)->setPrev(other.rear);
//     (this->front) = other.front;
//    }
   if (isOtherGreater)
   {
    (other.rear)->setNext(this->front); //Other is first
    (this->front)->setPrev(other.rear);
    (this->front) = other.front;
   }
   else
   {
    (this->rear)->setNext(other.front); //"This" is first
    (other.front)->setPrev(this->rear);
    (this->rear) = other.rear;
   }
   this->totalWeight += other.getTotalWeight();
   other.front = nullptr;
   other.rear = nullptr;
   other.totalWeight = 0;
}

Wagon *WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.

    Wagon* tmp = front;

    while (tmp)
    {
        if (tmp->getID() == id)
        {
            break;
        }
        tmp = tmp->next;
    }

    if (tmp)
    {
        if (tmp == front)
        {
            if(tmp->next) (tmp->next)->setPrev(nullptr);
            front = tmp->next;
            tmp->setNext(nullptr);
        }
        else if (tmp == rear)
        {
            if(tmp->prev) (tmp->prev)->setNext(nullptr);
            rear = tmp->prev;
            tmp->setPrev(nullptr);
        }
        else
        {
            (tmp->prev)->setNext(tmp->next);
            (tmp->next)->setPrev(tmp->prev);
        }
        totalWeight -= tmp->getWeight();
        std::cout << "Wagon " << tmp->getID() << " detached from Wagon List. " << std::endl;
        return tmp;
    }
    else
    {
        return nullptr;
    }
}

WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.

    Wagon* tmp = front;
    int weight = 0;

    while (tmp)
    {
        if (tmp->getID() == id)
        {
            break;
        }
        weight += tmp->getWeight();
        tmp = tmp->next;
    }
    if (tmp)
    {
        if (tmp == front)
        {
            newList.front = front;
            newList.rear = rear;
            newList.totalWeight = totalWeight;

            totalWeight = 0;
            front = nullptr;
            rear = nullptr;
        }
        else if (tmp == rear)
        {
            newList.front = rear;
            newList.rear = rear;
            newList.totalWeight = tmp->getWeight();

            if (tmp->prev)
            {
                (tmp->prev)->setNext(nullptr);
                rear = tmp->prev;
            }
            
            tmp->setPrev(nullptr);

            totalWeight -= tmp->getWeight();
        }
        else
        {
            newList.front = tmp;
            newList.rear = rear;
            newList.totalWeight = totalWeight - weight;

            if (tmp->prev)
            {
                (tmp->prev)->setNext(nullptr);
                rear = tmp->prev;
            }
            
            tmp->setPrev(nullptr);

            totalWeight = weight;
        }
    }
    return newList; // moved, not copied
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}

/**
 * @brief Find the heaviest wagon
 * 
 * @return int 
 */
int WagonList::maxWeight() const
{
    int max = 0;
    Wagon* tmp = front;
    while (tmp)
    {
        if (tmp->getWeight() > max)
            max = tmp->getWeight();
        tmp = tmp->getNext();
    }
    return max;
}