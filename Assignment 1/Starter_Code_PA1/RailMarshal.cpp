#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.

    Destination destination;

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        destination = static_cast<Destination> (i);
        departureYard[i] = TrainTrack(destination);
    }
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.

    std::stringstream ss(line);
    std::string smt;
    ss >> smt;

    if (smt == "ADD_WAGON")
    {
        int id;
        std::string cargoType;
        std::string destination;
        int weight; 
        int maxCouplerLoad; // Max weight a wagon can pull

        if (!(ss >> id >> cargoType >> destination >> weight >> maxCouplerLoad))
        {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }
        CargoType wagonCargo = parseCargo(cargoType);
        Destination wagonDestination = parseDestination(destination);

        Wagon* newWagon = new Wagon(id, wagonCargo, wagonDestination, weight, maxCouplerLoad);

        classificationYard.insertWagon(newWagon);
        std::cout << "Wagon " << *newWagon << " added to yard." << std::endl;
    }
    else if (smt == "REMOVE_WAGON")
    {
        try
        {
            int id;
            if (!(ss >> id))
            {
                throw std::runtime_error("Error: Invalid REMOVE_WAGON parameters.");
            }
            
            for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
            {
                for (int k = 0; k < NUM_CARGOTYPES_INT; k++)
                {
                    if ((classificationYard.getBlockTrain(i,k)).detachById(id))
                    {
                        std::cout << "Wagon " << id << " removed." << std::endl;
                        return;
                    }
                }
            } 
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
            return;
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else if (smt == "ASSEMBLE_TRAIN")
    {
        std::string destination;
        ss >> destination;

        Destination trainDestination = parseDestination(destination);
        int destIdx = static_cast<int>(trainDestination);

        if (destinationToString(trainDestination) == "OTHERS")
        {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
        }
        else
        {
            Train* train = classificationYard.assembleTrain(trainDestination, departureYard[destIdx].generateTrainName());

            if (train)
            {
                Train* splitTrain = train;
                int splitCounter = 0;
                while (splitTrain)
                {
                    ++splitCounter;
                    splitTrain = train->verifyCouplersAndSplit(splitCounter);
                    if (splitTrain)
                    {
                        std::cout << "Train " << splitTrain->getName() << " assembled after split with " 
                        << splitTrain->getWagons()<< " wagons." << std::endl;
                    }
                }
                std::cout << "Train " << train->getName() << " assembled with " << train->getWagons() << " wagons." << std::endl;
                
                // std::cout << "[main] departureYard[" << destIdx 
                // << "] address = " << &departureYard[destIdx] << std::endl;

                departureYard[destIdx].addTrain(train);
            }
            else
            {
                std::cout << "No wagons to assemble for " << destination << std::endl;
            }
        }
    }
    else if (smt == "DISPATCH_TRAIN")
    {
        std::string destination;
        ss >> destination;
        TrainTrack trainTrack;

        Destination trainDestination = parseDestination(destination);

        if (destinationToString(trainDestination) == "OTHERS")
        {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
        }
        else
        {
            dispatchFromTrack(trainDestination);
        }
    }
    else if (smt == "PRINT_YARD")
    {
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();
    }
    else if (smt == "PRINT_TRACK")
    {
        std::string destination;
        ss >> destination;
        TrainTrack trainTrack;

        Destination trainDestination = parseDestination(destination);

        if (destinationToString(trainDestination) == "OTHERS")
        {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
        }
        else
        {
            for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
            {
                trainTrack = departureYard[i];

                if (!trainTrack.isEmpty())
                {
                    if (trainTrack.getFirst()->getDestination() == trainDestination)
                    {
                        trainTrack.printTrack();
                        break;
                    }
                }
            }  
        }
    }
    else if (smt == "AUTO_DISPATCH")
    {
        std::string myswitch;
        ss >> myswitch;

        if (myswitch == "ON")
        {
            //! ON
            for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
            {
                departureYard[i].autoDispatch = true; 
            }
            std::cout << "Auto dispatch enabled" << std::endl;
        }
        else if (myswitch == "OFF")
        {
            //! OFF
            for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
            {
                departureYard[i].autoDispatch = false;
            }
            std::cout << "Auto dispatch disabled" << std::endl; 
        }
        else
        {
            std::cout << "Unknown AUTO_DISPATCH command" << std::endl;
        }
    }
    else if (smt == "CLEAR")
    {
        classificationYard.clear();

        for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
        {
            departureYard[i] = TrainTrack();
        }
        std::cout << "System cleared." << std::endl;
    }
    else
    {
        std::cout << "Error: Unknown command '" << smt << "'" << std::endl;
    }
    
    // if ADD_WAGON
    //! Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;

    // if REMOVE_WAGON
    //! Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;

    // if ASSEMBLE_TRAIN
    //!  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;

    // if DISPATCH_TRAIN
    //!  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;

    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";

    // if PRINT_TRACK
    //!  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
    TrainTrack trainTrack;
    int destIndex = static_cast<int>(track);
    Train* beDeparted = nullptr;

    for (int i = 0; i < NUM_DESTINATIONS_INT; i++)
    {
        trainTrack = departureYard[i];

        if (!trainTrack.isEmpty())
        {
            if (trainTrack.getFirst()->getDestination() == track)
            {
                beDeparted = trainTrack.departTrain();

                std::cout << "Train " <<beDeparted->getName()
                << " departed from Track " << destIndex
                << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";

                std::cout << "Dispatching " << beDeparted->getName() << " (" << beDeparted->getTotalWeight() << " tons)." << std::endl;

                return;
            }
        }
    }
    std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); i++) //was ++i
    {
        departureYard[i].printTrack();
    }
}