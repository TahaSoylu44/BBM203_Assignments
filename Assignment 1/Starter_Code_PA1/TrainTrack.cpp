#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::~TrainTrack()
{
    // TODO: If track is deconstucting, 
    // depart all of the trains
    // Handle pointers as necessary
}

// Given to you ready
std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    // TODO: Add a train to the end (rear) of this track

    // TODO: Hadle Auto-dispatch rule:
    //   If adding this train causes AUTO_DISPATCH_LIMIT to overflow
    //   and auto-dispatch is enabled, repeatedly dispatch (depart) trains
    //   from the front until there is enough capacity.
    //      use: std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";

}

Train *TrainTrack::departTrain()
{
    // TODO: Remove the first train (front of the track) and return it.
    // use: std::cout << "Train " << removed->name << " departed from Track " << destinationToString(destination) << "." << std::endl;
    return nullptr;
}

bool TrainTrack::isEmpty() const
{
    // TODO: Return true if there are no trains on this track.
    return false;
}


Train *TrainTrack::findTrain(const std::string &name) const
{
    // TODO: Search for a train by name.
    // Return pointer to train if found, nullptr otherwise.
    return nullptr;
}

// Given to you ready
void TrainTrack::printTrack() const
{

    if (isEmpty())
        return;

    Train *current = firstLocomotive;

    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}