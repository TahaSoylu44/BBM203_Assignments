#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //TODO: Implement tryAssignRequest function as explained in the PDF.

    if ((currentWorkload + req.computeWorkloadContribution()) > maxLoadCapacity) return false;

    if (missionStack.push(req))
    {
        currentWorkload += req.computeWorkloadContribution();
        return true;
    }
    else
    {
        return false;
    }
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //TODO: Implement rollbackMission function as explained in the PDF.

    if (!hasActiveMission()) return;

    MissionStack tmpStack;
    Request req;

    while (hasActiveMission()) 
    {
        missionStack.pop(req);
        tmpStack.push(req);
    }

    int supplySize = supplyQueue.size();
    int rescueSize = rescueQueue.size();

    while (!tmpStack.isEmpty()) 
    {
        tmpStack.pop(req);
        if (req.getType() == "SUPPLY") 
        {
            supplyQueue.enqueue(req);
        } 
        else if (req.getType() == "RESCUE")
        {
            rescueQueue.enqueue(req);
        }
    }

    for (int i = 0; i < supplySize && !supplyQueue.isEmpty(); i++)
    {
        if (supplyQueue.dequeue(req)) supplyQueue.enqueue(req);
        else break;
    }

    for (int i = 0; i < rescueSize && !rescueQueue.isEmpty(); i++)
    {
        if (rescueQueue.dequeue(req)) rescueQueue.enqueue(req);
        else break;
    }
    
    clearMission();
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
