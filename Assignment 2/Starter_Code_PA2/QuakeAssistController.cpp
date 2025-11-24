#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //TODO: Read the input file line by line and execute realtime.
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //TODO: Create a team array and initialize it with teams.

    if (numTeams < 0) return false;

    teamCount = numTeams;
    teams = new Team[teamCount];

    std::cout << "Initialized " << teamCount << " teams." << std::endl;

    for (int i = 0; i < teamCount; i++)
    {
        teams[i].setId(i);
    }
    
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //TODO: Find the index of the team using teamId.

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i].getId() == teamId) 
        {
            return i;
        }
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //TODO: Find the index of team in the array, update the capacity value of the team.

    int index = findTeamIndexById(teamId);

    if (index != -1)
    {
        teams[index].setMaxLoadCapacity(capacity);
        std::cout << "Team " << teamId << " capacity set to " << capacity << std::endl;

        return true;
    }
    return false;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //TODO: Remove request using request ID from request(SUPPLY, RESCUE) queue.

    if (supplyQueue.removeById(id))
    {
        return true;
    }
    else if (rescueQueue.removeById(id))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.

    int counter = 0;
    Request chosenRequest;
    Request supplyRequest;
    Request rescueRequest;
    int teamIdx = findTeamIndexById(teamId);

    if (teamIdx < 0) return false;

    while (counter < k)
    {
        //Su gerekli request neymiş bulalım
        if (supplyQueue.isEmpty() && rescueQueue.isEmpty()) break;

        if (supplyQueue.isEmpty() && !rescueQueue.isEmpty())
        {
            rescueQueue.peek(chosenRequest);
        }
        else if (!supplyQueue.isEmpty() && rescueQueue.isEmpty())
        {
            supplyQueue.peek(chosenRequest);
        }
        else
        {
            supplyQueue.peek(supplyRequest);
            rescueQueue.peek(rescueRequest);

            if (supplyRequest.computeEmergencyScore() > rescueRequest.computeEmergencyScore())
            {
                chosenRequest = supplyRequest;
            }
            else
            {
                chosenRequest = rescueRequest;
            }
        } 
        
        //Tamam,bence buldum.Simdi takıma atayalım

        if (teams[teamIdx].tryAssignRequest(chosenRequest))
        {
            if (chosenRequest.getType() == "SUPPLY")
            {
                supplyQueue.dequeue(chosenRequest);
            }
            else
            {
                rescueQueue.dequeue(chosenRequest);
            } 
        }
        else
        {
            teams[teamIdx].rollbackMission(supplyQueue, rescueQueue);
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            break;
        }
        counter++;
    }
    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //TODO: Print queues.

    int index;
    int frontIndexOfSupply = supplyQueue.getFrontIndex();
    int frontIndexOfRescue = rescueQueue.getFrontIndex();

    index = frontIndexOfSupply;
    std::cout << "SUPPLY QUEUE:" << std::endl;

    while (!supplyQueue.isEmpty())
    {
        std::cout << (supplyQueue.getData())[index].getId() << " ";
        std::cout << (supplyQueue.getData())[index].getCity() << " ";
        std::cout << (supplyQueue.getData())[index].getSupplyType() << " ";
        std::cout << (supplyQueue.getData())[index].getAmount() << " ";
        std::cout << (supplyQueue.getData())[index].getNumPeople() << std::endl;

        if (index == (frontIndexOfSupply + (supplyQueue.getCount() - 1)) % supplyQueue.getCapacity()) break;
        index = (index + 1) % supplyQueue.getCapacity();
    }

    index = frontIndexOfRescue;
    std::cout << "RESCUE QUEUE:" << std::endl;

    while (!rescueQueue.isEmpty())
    {
        std::cout << (rescueQueue.getData())[index].getId() << " ";
        std::cout << (rescueQueue.getData())[index].getCity() << " ";
        std::cout << (rescueQueue.getData())[index].getEmergencyLevel() << " ";
        std::cout << (rescueQueue.getData())[index].getRisk() << " ";
        std::cout << (rescueQueue.getData())[index].getNumPeople() << std::endl;

        if (index == (frontIndexOfSupply + (rescueQueue.getCount() - 1)) % rescueQueue.getCapacity()) break;
        index = (index + 1) % rescueQueue.getCapacity();
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //TODO: Print team data using teamId.

    int teamIdx = findTeamIndexById(teamId);

    std::cout << teams[teamIdx].getId() << " ";
    std::cout << teams[teamIdx].getCurrentWorkload() << " ";
    std::cout << teams[teamIdx].getMaxLoadCapacity() << std::endl;
}

void QuakeAssistController::clear() {
    supplyQueue.clear();
    rescueQueue.clear();

    // for (int i = 0; i < teamCount; i++)
    // {
    //     teams[i].getMissionStack().clear();
    // }

    delete[] teams;
    teamCount = 0;
}
