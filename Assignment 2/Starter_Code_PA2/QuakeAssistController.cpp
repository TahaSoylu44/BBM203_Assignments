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

    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "INIT_TEAMS")
    {
        int numTeams;

        if (!(ss >> numTeams))
        {
            std::cout << "Error: Invalid INIT_TEAMS parameters.\n";
        } 
        //Go on

        if (teamCount != 0) delete[] teams; //daha önceden var olanlari temizle

        supplyQueue.clear();    //Prepare queues to get requests
        rescueQueue.clear();

        initializeTeams(numTeams);
    }
    else if (cmd == "SET_TEAM_CAPACITY")
    {
        int teamID;
        int capacity;

        if (!(ss >> teamID >> capacity))
        {
            std::cout << "Error: Invalid SET_TEAM_CAPACITY parameters.\n";
        }
        //Go on

        if (!handleSetTeamCapacity(teamID, capacity))
        {
            std::cout << "An error occured while handling capacity" << std::endl;
        } 
    }
    else if (cmd == "ADD_SUPPLY")
    {
        std::string id;
        std::string city;
        std::string supplyType;
        int amount;
        int emergencyLevel;

        if (!(ss >> id >> city >> supplyType >> amount >> emergencyLevel))
        {
            std::cout << "Error: Invalid ADD_SUPPLY parameters.\n";
        }
        //Go on

        if (handleAddSupply(id, city, supplyType, amount, emergencyLevel))
        {
            std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
        }
        else
        {
            std::cout << "An error occured while adding supply" << std::endl;
        } 
    }
    else if (cmd == "ADD_RESCUE")
    {
        std::string id;
        std::string city;
        int numPeople;
        std::string risk;
        int emergencyLevel;

        if (!(ss >> id >> city >> numPeople >> risk >> emergencyLevel))
        {
            std::cout << "Error: Invalid ADD_RESCUE parameters.\n";
        }
        //Go on

        if (handleAddRescue(id, city, numPeople, risk, emergencyLevel))
        {
            std::cout << "Request " << id << " added to RESCUE queue." << std::endl;
        }
        else
        {
            std::cout << "An error occured while adding rescue" << std::endl;
        }
    }
    else if (cmd == "REMOVE_REQUEST")
    {
        std::string id;

        if (!(ss >> id))
        {
            std::cout << "Error: Invalid REMOVE_REQUEST parameters.\n";
        }   
        //Go on

        if (handleRemoveRequest(id))
        {
            std::cout << "Request " << id << " removed from queues." << std::endl;
        }
        else
        {
            std::cout << "Error: Request " << id << " not found." << std::endl;
        }
    }
    else if (cmd == "HANDLE_EMERGENCY")
    {
        int teamId;
        int k;

        if (!(ss >> teamId >> k))
        {
            std::cout << "Error: Invalid HANDLE_EMERGENCY parameters.\n";
        }
        //Go on

        if (!handleHandleEmergency(teamId, k))
        {
            std::cout << "There is an issue in handling emergency" << std::endl;
        }
    }
    else if (cmd == "PRINT_TEAM")
    {
        int teamId;

        if (!(ss >> teamId))
        {
            std::cout << "Error: Invalid PRINT_TEAM parameters.\n";
        }
        //Go on

        printTeam(teamId);
    }
    else if (cmd == "DISPATCH_TEAM")
    {
        int teamId;

        if (!(ss >> teamId))
        {
            std::cout << "Error: Invalid DISPATCH_TEAM parameters.\n";
        }
        //Go on

        handleDispatchTeam(teamId);
    }
    else if (cmd == "PRINT_QUEUES")
    {
        printQueues();
    }
    else if (cmd == "CLEAR")
    {
        clear();
    }
    else
    {
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl; 
    }
    return true;
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
        std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;

        return true;
    }
    return false;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //TODO: Create new supply request, and add it to the SUPPLY queue.

    Request req = Request(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    supplyQueue.enqueue(req);
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //TODO: Create new rescue request, and add it to the RESCUE queue.

    Request req = Request(id, cityStr, numPeople, riskStr, emergencyLevel);
    rescueQueue.enqueue(req);
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

    int counter = 1;
    Request chosenRequest;
    Request supplyRequest;
    Request rescueRequest;
    int teamIdx = findTeamIndexById(teamId);

    int supplyCounter = 0;
    int rescueCounter = 0;

    if (teamIdx < 0) return false;

    while (counter <= k)
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
                supplyCounter++;
            }
            else
            {
                rescueQueue.dequeue(chosenRequest);
                rescueCounter++;
            } 
        }
        else
        {
            teams[teamIdx].rollbackMission(supplyQueue, rescueQueue);
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            return true;
        }
        counter++;
    }
    std::cout << "Team " << teamId << " assigned " << (supplyCounter + rescueCounter) 
    << " requests (" << supplyCounter << " SUPPLY, " << rescueCounter 
    << " RESCUE), total workload " << teams[teamIdx].getCurrentWorkload() << "." << std::endl;

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
        const Request& req = (supplyQueue.getData())[index];

        std::cout << req.getId() << " ";
        std::cout << req.getCity() << " ";
        std::cout << req.getSupplyType() << " ";
        std::cout << req.getAmount() << " ";
        std::cout << req.getEmergencyLevel() << std::endl;

        if (index == (frontIndexOfSupply + (supplyQueue.getCount() - 1)) % supplyQueue.getCapacity()) break;
        index = (index + 1) % supplyQueue.getCapacity();
    }

    index = frontIndexOfRescue;
    std::cout << "RESCUE QUEUE:" << std::endl;

    while (!rescueQueue.isEmpty())
    {
        const Request& req = (rescueQueue.getData())[index];

        std::cout << req.getId() << " ";
        std::cout << req.getCity() << " ";
        std::cout << req.getNumPeople() << " ";
        std::cout << req.getRisk() << " ";
        std::cout << req.getEmergencyLevel() << std::endl;

        if (index == (frontIndexOfRescue + (rescueQueue.getCount() - 1)) % rescueQueue.getCapacity()) break;
        index = (index + 1) % rescueQueue.getCapacity();
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //TODO: Print team data using teamId.

    int teamIdx = findTeamIndexById(teamId);

    if (teamIdx < 0) return;

    const MissionStack& ms = teams[teamIdx].getMissionStack();

    std::cout << "TEAM "<< teamId << " STACK:" << std::endl;

    for (int i = ms.getTopIndex(); i >= 0; i--)
    {
        const Request& req = (ms.getData())[i];

        std::cout << req.getId() << " ";
        std::cout << req.getCity() << " ";

        if (req.getType() == "SUPPLY")
        {
            std::cout << req.getSupplyType() << " ";
            std::cout << req.getAmount() << " ";
        }
        else
        {
            std::cout << req.getNumPeople() << " ";
            std::cout << req.getRisk() << " ";
        }
        std::cout << req.getEmergencyLevel() << std::endl;
    }
}

void QuakeAssistController::clear() {
    supplyQueue.clear();
    rescueQueue.clear();

    for (int i = 0; i < teamCount; i++)
    {
        teams[i].clearMission();
    }

    std::cout << "System cleared." << std::endl;
}
