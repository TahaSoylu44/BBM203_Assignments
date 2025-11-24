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

    if (missionStack.push(req))
    {
        currentWorkload += req.computeWorkloadContribution();

        if (currentWorkload > maxLoadCapacity)
        {
            return false;
        } 
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    // (void)req;
    // return false;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //TODO: Implement rollbackMission function as explained in the PDF.

    while(!missionStack.isEmpty())
    {
        Request popedRequest;

        missionStack.pop(popedRequest);

        if (popedRequest.getType() == "SUPPLY")
        {
           supplyQueue.enqueue(popedRequest);
        }
        else if (popedRequest.getType() == "RESCUE")
        {
            rescueQueue.enqueue(popedRequest);
        }
    }

    // (void)supplyQueue;
    // (void)rescueQueue;
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
