
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known

    int maxTokens = 5;
    std::string tokens[maxTokens];
    int count = tokenize(line, tokens, maxTokens);
    std::string cmd = tokens[0];

    if (!cmd.empty())
    {
        if (cmd == "ADD_ARTIFACT") handleAddArtifact(tokens, count);
        else if (cmd == "REMOVE_ARTIFACT") handleRemoveArtifact(tokens, count);
        else if (cmd == "HIRE_RESEARCHER") handleHireResearcher(tokens, count);
        else if (cmd == "FIRE_RESEARCHER") handleFireResearcher(tokens, count);
        else if (cmd == "REQUEST") handleRequest(tokens, count);
        else if (cmd == "RETURN") handleReturn(tokens, count);
        else if (cmd == "RETURN_ALL") handleReturnAll(tokens, count);
        else if (cmd == "RESEARCHER_LOAD") handleResearcherLoad(tokens, count);
        else if (cmd == "MATCH_RARITY") handleMatchRarity(tokens, count);
        else if (cmd == "PRINT_UNASSIGNED") handlePrintUnassigned(tokens, count);
        else if (cmd == "PRINT_STATS") handlePrintStats(tokens, count);
        else if (cmd == "CLEAR") handleClear(tokens, count);
        else std::cout << "Error: Unknown command '" << cmd << "'" << '\n';
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).

    if (count == 5)
    {
        int id = std::stoi(tokens[1]);
        int rarity = std::stoi(tokens[3]);
        int value = std::stoi(tokens[4]);
        std::string name = tokens[2];

        Artifact artifact = Artifact(id, name ,rarity, value);

        if (artifactTree.insertArtifact(artifact))
        {
            std::cout << "Artifact " << id << " added." << '\n';
        }
        else
        {
            std::cout << "The artifact already exists." << '\n';
        }
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    }
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.

    if (count == 2)
    {
        int id = std::stoi(tokens[1]);
        ArtifactNode* artifactNode = artifactTree.findArtifact(id);

        if (artifactNode)
        {
            std::string researcherName = artifactNode->data.assignedToName;

            if (researcherName != "")   //A Researcher examines it.
            {
                ResearcherNode* researcherNode = researcherTree.findResearcher(researcherName);
                researcherNode->data.removeArtifact(id);
            }

            if (artifactTree.removeArtifact(id))
            {
                std::cout << "Artifact " << id << " removed." << '\n';
            }
            else
            {
                std::cout << "Error: Artifact " << id << " cannot be removed." << '\n';
            }
        }
        else
        {
            std::cout << "Error: Artifact " << id << " not found." << '\n';
        }  
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    } 
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.

    if (count == 3)
    {
        std::string researcherName = tokens[1];
        int capacity = std::stoi(tokens[2]);
        Researcher researcher = Researcher(researcherName, capacity);

        if (researcherTree.insertResearcher(researcher))
        {
            std::cout << "Researcher " << researcherName << " hired." << '\n';
        }
        else
        {
            std::cout << "The researcher already exists." << '\n';
        } 
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    }
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.

    if (count == 2)
    {
        std::string researcherName = tokens[1];

        ResearcherNode*  researcherNode = researcherTree.findResearcher(researcherName);

        if (researcherNode)
        {
            Researcher& researcher = researcherNode->data;

            for (int i = 0; i < researcher.numAssigned; i++)
            {
                int artifactId = researcher.assignedArtifacts[i];
                artifactTree.clearAssignedTo(artifactId);
            } 

            if (researcherTree.removeResearcher(researcherName))
            {
                std::cout << "Researcher " << researcherName << " fired." << '\n';
            }
            else
            {
                std::cout << "Error: Researcher " << researcherName << " cannot be removed." << '\n';
            }
        }
        else
        {
            std::cout << "Error: Researcher " << researcherName << " not found." << '\n';
        }
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    } 
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."

    if (count == 3)
    {
        std::string researcherName = tokens[1];
        int artifactID = std::stoi(tokens[2]);

        ArtifactNode* artifactNode = artifactTree.findArtifact(artifactID);
        ResearcherNode* researcherNode = researcherTree.findResearcher(researcherName);

        if (researcherNode)
        {
            if (artifactNode)
            {
                Researcher& researcher = researcherNode->data;
                Artifact& artifact = artifactNode->data;

                if (artifact.assignedToName == "")
                {
                    if (researcher.numAssigned == researcher.capacity)
                    {
                        std::cout << "Error: Researcher " << researcherName << " is at full capacity." << '\n';
                    }
                    else
                    {
                        artifactTree.setAssignedTo(artifactID, researcherName);
                        researcher.addArtifact(artifactID);

                        std::cout << "Artifact " << artifactID << " assigned to " << researcherName << '\n';
                    } 
                }
                else
                {
                    std::cout << "Error: Artifact " << artifactID << " is already assigned." << '\n';
                }  
            }
            else
            {
                std::cout << "Error: Artifact" << artifactID << " cannot be found." << '\n';
            }
        }
        else
        {
            std::cout << "Error: Researcher" << researcherName << " cannot be found." << '\n';
        }
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    } 
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."

    if (count == 3)
    {
        std::string researcherName = tokens[1];
        int artifactID = std::stoi(tokens[2]);

        ResearcherNode* researcherNode = researcherTree.findResearcher(researcherName);
        ArtifactNode* artifactNode = artifactTree.findArtifact(artifactID);

        if (researcherNode)
        {
            if (artifactNode)
            {
                Researcher& researcher = researcherNode->data;
                Artifact& artifact = artifactNode->data;

                if (artifact.assignedToName == researcherName)
                {
                    researcher.removeArtifact(artifactID);
                    artifactTree.clearAssignedTo(artifactID);

                    std::cout << "Artifact " << artifactID << " returned by " << researcherName << '\n';
                }
                else
                {
                    std::cout << "Researcher " << researcherName << " does not have Artifact " << artifactID << '\n';
                }  
            }
            else
            {
                std::cout << "Error: Artifact" << artifactID << " cannot be found." << '\n';
            }
        }
        else
        {
            std::cout << "Error: Researcher" << researcherName << " cannot be found." << '\n';
        } 
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    }
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.

    if (count == 2)
    {
        std::string researcherName = tokens[1];
        ResearcherNode* researcherNode = researcherTree.findResearcher(researcherName);

        if (researcherNode)
        {
            Researcher& researcher = researcherNode->data;

            for (int i = 0; i < researcher.numAssigned; i++)
            {
                artifactTree.clearAssignedTo(researcher.assignedArtifacts[i]);
            }
            
            researcher.removeAllArtifacts();
            std::cout << "Researcher " << researcherName << "'s all artifacts returned." << '\n';
        }
        else
        {
            std::cout << "Error: Researcher" << researcherName << " cannot be found." << '\n';
        }  
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    }
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.

    if (count == 2)
    {
        std::string researcherName = tokens[1];
        ResearcherNode* researcherNode = researcherTree.findResearcher(researcherName);

        if (researcherNode)
        {
            std::cout << researcherName << " has " << researcherNode->data.numAssigned << " artifacts" << '\n';
        }
        else
        {
            std::cout << "Error: Researcher" << researcherName << " cannot be found." << '\n';
        }
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    } 
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.

    if (count == 2)
    {
        int minRarity = std::stoi(tokens[1]);
        artifactTree.inorderTraversalForMatchRarity(minRarity);
    }
    else
    {
        std::cout << "There is no enough parameters" << '\n';
    } 
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)

    int totalArtifacts = artifactTree.getArtifactCount();
    int totalResearchers = researcherTree.getResearcherCount();
    int totalRarity = artifactTree.getTotalRarity();
    int totalLoad = researcherTree.getTotalLoad();

    int averageArtifactRarity;
    int averageResearcherLoad;

    if (totalArtifacts != 0) averageArtifactRarity = totalRarity / totalArtifacts;
    else averageArtifactRarity = 0;

    if (totalResearchers != 0) averageResearcherLoad = totalLoad / totalResearchers;
    else averageResearcherLoad = 0;
    
    std::cout << "The number of total artifacts: " << totalArtifacts << '\n';
    std::cout << "The number of total researchers: " << totalResearchers << '\n';
    std::cout << "The number of total rarity: " << totalRarity << '\n';
    std::cout << "The number of total researcher load: " << totalLoad << '\n';
    std::cout << "The number of average artifact rarity: " << averageArtifactRarity << '\n';
    std::cout << "The number of average researcher load: " << averageResearcherLoad << '\n';

    researcherTree.traversePreOrderForStats();
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    artifactTree.clear();
    researcherTree.clear();

    std::cout << "All data cleared." << '\n';
}