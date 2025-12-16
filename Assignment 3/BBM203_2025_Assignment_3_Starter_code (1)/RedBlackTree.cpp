#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:

    if (node == nullptr) return;

    clear(node->left);
    clear(node->right);
    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{
    //TODO: search and find researcher by name

    if (node == nullptr) return nullptr;
    if (node->data.fullName == fullName) return node;

    if (fullName < node->data.fullName) return find(node->left, fullName);
    else return find(node->right, fullName);
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise

    ResearcherNode* newResearcher = new ResearcherNode(researcher);

    bool inserted = false;
    root = bstInsert(root, newResearcher, inserted);
    
    if (inserted) insertFixup(newResearcher);
    else delete newResearcher;
    return inserted;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).

    if (current == nullptr)
    {
        inserted = true;
        return node;
    }

    if (current->data.fullName == node->data.fullName)
    {
        inserted = false;
        return current;
    }

    if (node->data.fullName < current->data.fullName)
    {
        current->left = bstInsert(current->left, node, inserted);
        current->left->parent = current;
    }
    else
    {
        current->right = bstInsert(current->right, node, inserted);
        current->right->parent = current;
    }

    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    while (node != root && node->parent->color == RED)
    {
        ResearcherNode* parent = node->parent;
        ResearcherNode* grandParent = parent->parent;

        if (parent == grandParent->left)
        {
            ResearcherNode* uncle = grandParent->right;

            if (uncle != nullptr && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                node = grandParent; 
            }
            else
            {
                if (node == parent->right)
                {
                    node = parent; 
                    rotateLeft(node); 
                    parent = node->parent; 
                }
                parent->color = BLACK;       
                grandParent->color = RED;    
                rotateRight(grandParent);
            }
        }
        else
        {
            ResearcherNode* uncle = grandParent->left;

            if (uncle != nullptr && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                node = grandParent;
            }
            else
            {
                if (node == parent->left)
                {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                }

                parent->color = BLACK;
                grandParent->color = RED;
                rotateLeft(grandParent);
            }
        }
    }

    root->color = BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    ResearcherNode *z = findResearcher(fullName);

    if (z == nullptr) return false; 

    ResearcherNode *y = z; 
    ResearcherNode *x;     
    ResearcherNode *xParent; 
    
    Color yOriginalColor = y->color;

    if (z->left == nullptr)
    {
        x = z->right;
        xParent = z->parent;
        if (z->parent == nullptr) root = x;
        else if (z == z->parent->left) z->parent->left = x;
        else z->parent->right = x;
        
        if (x != nullptr) x->parent = z->parent;
    }
    else if (z->right == nullptr)
    {
        x = z->left;
        xParent = z->parent;

        if (z->parent == nullptr) root = x;
        else if (z == z->parent->left) z->parent->left = x;
        else z->parent->right = x;

        if (x != nullptr) x->parent = z->parent;
    }
    else
    {
        y = minimum(z->right); 
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) xParent = y;
        else
        {
            xParent = y->parent; 
            if (y->parent == nullptr) root = x; 
            else if (y == y->parent->left) y->parent->left = x;
            else y->parent->right = x;

            if (x != nullptr) x->parent = y->parent;

            y->right = z->right;
            y->right->parent = y;
        }

        if (z->parent == nullptr) root = y;
        else if (z == z->parent->left) z->parent->left = y;
        else z->parent->right = y;
        y->parent = z->parent;
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK) deleteFixup(x, xParent);

    return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    while (node != root && (node == nullptr || node->color == BLACK))
    {
        ResearcherNode* sibling;

        if (node == parent->left)
        {
            sibling = parent->right;

            if (sibling != nullptr && sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                sibling = parent->right; 
            }

            bool leftNephewBlack = (sibling->left == nullptr || sibling->left->color == BLACK);
            bool rightNephewBlack = (sibling->right == nullptr || sibling->right->color == BLACK);

            if (leftNephewBlack && rightNephewBlack)
            {
                sibling->color = RED;
                node = parent;       
                parent = node->parent; 
            }
            else
            {
                if (rightNephewBlack)
                {
                    if (sibling->left) sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = parent->right; 
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->right) sibling->right->color = BLACK;
                rotateLeft(parent);
                
                node = root;
            }
        }
        else
        {
            sibling = parent->left;

            if (sibling != nullptr && sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                sibling = parent->left;
            }

            bool leftNephewBlack = (sibling->left == nullptr || sibling->left->color == BLACK);
            bool rightNephewBlack = (sibling->right == nullptr || sibling->right->color == BLACK);

            if (leftNephewBlack && rightNephewBlack)
            {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (leftNephewBlack)
                {
                    if (sibling->right) sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->left) sibling->left->color = BLACK;
                rotateRight(parent);
                
                node = root;
            }
        }
    }

    if (node != nullptr) node->color = BLACK;
}

ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.

    ResearcherNode* tmp = node;

    while (tmp->left) tmp = tmp->left;
    return tmp;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.

    bool isRoot = (x == root);

    ResearcherNode* newRoot = x->right;

    x->right = newRoot->left;
    if (x->right) x->right->parent = x;
    newRoot->left = x;
    newRoot->parent = x->parent;
    x->parent = newRoot;

    if (newRoot->parent)
    {
        if (newRoot->parent->left == x) newRoot->parent->left = newRoot;
        else newRoot->parent->right = newRoot;
    }

    if (isRoot) root = newRoot;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.

    bool isRoot = (y == root);

    ResearcherNode* newRoot = y->left;

    y->left = newRoot->right;
    if (y->left) y->left->parent = y;
    newRoot->right = y;
    newRoot->parent = y->parent;
    y->parent = newRoot;

    if (newRoot->parent)
    {
        if (newRoot->parent->left == y) newRoot->parent->left = newRoot;
        else newRoot->parent->right = newRoot;
    }

    if (isRoot) root = newRoot;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.

    if (node == nullptr) return 0;

    return getResearcherCount(node->left) + getResearcherCount(node->right) + 1;
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.

    if (node == nullptr) return 0;

    return getTotalLoad(node->left) + getTotalLoad(node->right) + node->data.numAssigned;
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.

    if (node == nullptr) return;

    std::cout << node->data.fullName << " ";
    std::cout << node->data.capacity << " ";
    std::cout << node->data.numAssigned << '\n';

    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}