#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/
    bool isTrainExists = false;
    CargoType cargoType;
    Wagon* hazardousWagon;

    Train* newTrain = new Train(trainName, dest);

    int destination = static_cast<int>(dest);

    for (int i = 0; i < NUM_CARGOTYPES_INT; i++)
    {
        WagonList& block = getBlockTrain(destination, i);

        if (!block.isEmpty())
        {
            cargoType = block.getFront()->getCargoType();
            if (cargoTypeToString(cargoType) == "HAZARDOUS")
            {
                hazardousWagon = block.detachById(block.getFront()->getID());
                newTrain->addWagonToRear(hazardousWagon);
                //If I come across a hazardous block,I detach the front.
                //I add the detached one to the rear.     
            }
            else
            {
                newTrain->appendWagonList(block);
            }
            isTrainExists = true;
        }
    }
    if (isTrainExists)
    {
        return newTrain;
    }
    else
    {
        return nullptr;
    }
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int k = 0; k < NUM_CARGOTYPES_INT; k++)
        {
            if (!blockTrains[i][k].isEmpty())
            {
                return false;
            }
        }
    }
    return true;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        for (int k = 0; k < NUM_CARGOTYPES_INT; k++)
        {
            blockTrains[i][k].clear();
        }
    } 
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}