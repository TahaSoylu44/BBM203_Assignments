#include "AVLTree.h"

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
    //TODO

    if (node == nullptr) return;

    clear(node->left);
    clear(node->right);
    delete node;
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root

    ArtifactNode* newRoot = x->right;
    x->right = newRoot->left;
    newRoot->left = x;

    //height check
    x->height = ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right)) + 1;
    newRoot->height = ((height(newRoot->left) > height(newRoot->right)) ? height(newRoot->left) : height(newRoot->right)) + 1;

    return newRoot;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).

    ArtifactNode* newRoot = y->left;
    y->left = newRoot->right;
    newRoot->right = y;

    //height check
    y->height = ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right)) + 1;
    newRoot->height = ((height(newRoot->left) > height(newRoot->right)) ? height(newRoot->left) : height(newRoot->right)) + 1;

    return newRoot;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.

    ArtifactNode* tmp = node;

    while (tmp->left) tmp = tmp->left;

    return tmp;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.

    if (node == nullptr)
    {
        inserted = true;
        return new ArtifactNode(artifact);
    }

    if (node->data.artifactID == artifact.artifactID)
    {
        inserted = false;
        return node;
    }

    if (artifact.artifactID < node->data.artifactID)
    {
        node->left = insert(node->left, artifact, inserted);
    }
    else
    {
        node->right = insert(node->right, artifact, inserted);
    }

    if (inserted)
    {
        int balanceValue = getBalance(node);

        if (!(balanceValue <= 1 && balanceValue >= -1))
        {
            if (balanceValue < 0)   //right 
            {
                if (artifact.artifactID < node->right->data.artifactID)
                {
                    //RL
                    node->right = rotateRight(node->right);
                    node = rotateLeft(node);
                }
                else
                {
                    //RR
                    node = rotateLeft(node);
                }   
            }
            else    //left
            {
                if (artifact.artifactID < node->left->data.artifactID)
                {
                    //LL
                    node = rotateRight(node);
                }
                else
                {
                    //LR
                    node->left = rotateLeft(node->left);
                    node = rotateRight(node);
                } 
            }
        }
    }

    node->height = ((height(node->left) > height(node->right)) ? height(node->left) : height(node->right)) + 1;

    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.

    if (node == nullptr)
    {
        removed = false;
        return node;
    }

    if (node->data.artifactID == artifactID)
    {
        removed = true;

        if (node->left && node->right)
        {
            ArtifactNode* tmpNode = findMinNode(node->right);
            node->data = tmpNode->data;
            node->right = remove(node->right, node->data.artifactID, removed);
        }
        else if (node->left)
        {
            ArtifactNode* leftChild = node->left;
            delete node;
            return leftChild;
        }
        else if (node->right)
        {
            ArtifactNode* rightChild = node->right;
            delete node;
            return rightChild;
        }
        else
        {
            delete node;
            return nullptr;
        }
    }
    else if (artifactID < node->data.artifactID)
    {
        node->left = remove(node->left, artifactID, removed);
    }
    else
    {
        node->right = remove(node->right, artifactID, removed);
    }

    if (removed)
    {
        node->height = ((height(node->left) > height(node->right)) ? height(node->left) : height(node->right)) + 1;

        int balanceValue = getBalance(node);

        if (!(balanceValue <= 1 && balanceValue >= -1))
        {
            int childBalanceValue;

            if (balanceValue < 0)   //right 
            {
                childBalanceValue = getBalance(node->right);

                if (childBalanceValue <= 0)
                {
                    //RR
                    node = rotateLeft(node);    
                }
                else
                {
                    //RL
                    node->right = rotateRight(node->right);
                    node = rotateLeft(node);
                }  
            }
            else    //left
            {
                childBalanceValue = getBalance(node->left);

                if (childBalanceValue >= 0)
                {
                    //LL
                    node = rotateRight(node);
                }
                else
                {
                    //LR
                    node->left = rotateLeft(node->left);
                    node = rotateRight(node);
                } 
            }
        }
    }
    
    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO

    if (node == nullptr) return nullptr;

    if (artifactID < node->data.artifactID) return find(node->left, artifactID);
    else return find(node->right, artifactID);
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{
    //TODO: assign artifact to researcher

    ArtifactNode* beAssigning = findArtifact(artifactID);
    beAssigning->data.assignedToName = researcherName;
    beAssigning->data.updateValueBasedOnUsage();
}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO

    ArtifactNode* beUnassigning = findArtifact(artifactID);
    beUnassigning->data.assignedToName = "";
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>

    if (node == nullptr) return;

    printUnassigned(node->left);

    if (node->data.name == "")
    {
        std::cout << node->data.artifactID << " ";
        std::cout << node->data.name << " ";
        std::cout << node->data.rarityLevel << " ";
        std::cout << node->data.researchValue << '\n';
    }
    
    printUnassigned(node->right);
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.

    if (node == nullptr) return 0;

    return getArtifactCount(node->left) + getArtifactCount(node->right) + 1;
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.

    if (node == nullptr) return 0;

    return getTotalRarity(node->left) + getTotalRarity(node->right) + node->data.rarityLevel;
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.

    if (node == nullptr) return;

    traversePostOrderForStats(node->left);
    traversePostOrderForStats(node->right);

    std::cout << node->data.artifactID << " ";
    std::cout << node->data.name << " ";
    std::cout << node->data.rarityLevel << " ";
    std::cout << node->data.researchValue << '\n';
}