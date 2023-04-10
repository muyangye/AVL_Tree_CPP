#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight () const;
    void setHeight (int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), height_(1)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the height of a AVLNode.
*/
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const
{
    return height_;
}

/**
* A setter for the height of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    height_ = height;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item);
    virtual void remove(const Key& key);
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void leftRotate(AVLNode<Key, Value>* node);
    void rightRotate(AVLNode<Key, Value>* node);
    // This function updates the height of a node and all its ancestors
    void updateHeights(AVLNode<Key, Value>* node);
    // This function calculates the difference in heights (i.e. balance factor)
    int calculateBF(AVLNode<Key, Value>* node);
    // This function finds x, y, z and returns an integer, balanceMode, representing how should we balance the tree
    int findXYZ(AVLNode<Key, Value>*& x, AVLNode<Key, Value>*& y, AVLNode<Key, Value>*& z, AVLNode<Key, Value>*& start);
    // Balance the tree according to the balanceMode
    void balance(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z, int balanceMode);
};

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* rightChild = node->getRight();
    AVLNode<Key, Value>* rightLeftChild = rightChild->getLeft(); // The left subtree of the right child's position need to change after left rotate
    // Specifically, it needs to be changed to the node to be rotated's right after left rotation
    if (rightLeftChild)
    {
        rightLeftChild->setParent(node);
        node->setRight(rightLeftChild);
    }
    else node->setRight(NULL);
    rightChild->setLeft(node); // Setting the node to be rotated's to its original right child's left
    // AVLNode<Key, Value>* parent = node->getParent() ? node->getParent() : NULL;
    AVLNode<Key, Value>* parent = node->getParent();
    if (!parent) rightChild->setParent(NULL); // If we are left rotating the root, nothing needed except for promoting the right child to root
    else // Otherwise we need to adjust the parent-child relationship of the node to be rotated's parent
    {
        rightChild->setParent(parent);
        // Depending on which direction is the node to be rotated from its parent
        if (parent->getLeft() == node) parent->setLeft(rightChild);
        else parent->setRight(rightChild);
    }
    node->setParent(rightChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* leftChild = node->getLeft();
    AVLNode<Key, Value>* leftRightChild = leftChild->getRight(); // The right subtree of the left child's position need to change after right rotate
    // Specifically, it needs to be changed to the node to be rotated's left after right rotation
    if (leftRightChild)
    {
        leftRightChild->setParent(node);
        node->setLeft(leftRightChild);
    }
    else node->setLeft(NULL);
    leftChild->setRight(node); // Setting the node to be rotated's to its original left child's right
    // AVLNode<Key, Value>* parent = node->getParent() ? node->getParent() : NULL;
    AVLNode<Key, Value>* parent = node->getParent();
    if (!parent) leftChild->setParent(NULL); // If we are right rotating the root, nothing needed except for promoting the left child to root
    else // Otherwise we need to adjust the parent-child relationship of the node to be rotated's parent
    {
        leftChild->setParent(parent);
        // Depending on which direction is the node to be rotated from its parent
        if (parent->getLeft() == node) parent->setLeft(leftChild);
        else parent->setRight(leftChild);
    }
    node->setParent(leftChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::updateHeights(AVLNode<Key, Value>* node)
{
    int leftHeight;
    int rightHeight;
    while (node) // Until we have reached the root (i.e. root's parent is NULL)
    {
        // Update the height as the max of left subtree and right subtree + 1, if no subtree, that subtree's height is 0
        leftHeight = node->getLeft() ? node->getLeft()->getHeight() : 0;
        rightHeight = node->getRight() ? node->getRight()->getHeight() : 0;
        node->setHeight(std::max(leftHeight, rightHeight) + 1);
        node = node->getParent();
    }
}

template<class Key, class Value>
int AVLTree<Key, Value>::calculateBF(AVLNode<Key, Value>* node)
{
    // If no subtree, that subtree's height is 0
    int leftHeight = node->getLeft() ? node->getLeft()->getHeight() : 0;
    int rightHeight = node->getRight() ? node->getRight()->getHeight() : 0;
    return std::abs(rightHeight - leftHeight);
}

template<class Key, class Value>
int AVLTree<Key, Value>::findXYZ(AVLNode<Key, Value>*& x, AVLNode<Key, Value>*& y, AVLNode<Key, Value>*& z, AVLNode<Key, Value>*& start)
{
    bool firstLeft = false; // This flag stores whether we first go left or right
    z = start; // start is the first unbalanced node, so is z
    int leftHeight = z->getLeft() ? z->getLeft()->getHeight() : 0;
    int rightHeight = z->getRight() ? z->getRight()->getHeight() : 0;
    // Let y be the subtree of z that is higher
    if (leftHeight > rightHeight)
    {
        firstLeft = true;
        y = z->getLeft();
    }
    else y = z->getRight();
    bool secondLeft = false; // This flag stores whether we then go left or right
    leftHeight = y->getLeft() ? y->getLeft()->getHeight() : 0;
    rightHeight = y->getRight() ? y->getRight()->getHeight() : 0;
    // Let x be the subtree of y that is higher
    if (leftHeight > rightHeight)
    {
        secondLeft = true;
        x = y->getLeft();
    }
    else if (leftHeight == rightHeight) // However, this time y's subtress may have the same height
    {
        // We need to break the ties to form a straight line
        if (firstLeft)
        {
            secondLeft = true;
            x = y->getLeft();
        }
        else x = y->getRight();
    }
    else x = y->getRight();
    // std::cout << x->getKey() << std::endl;
    // std::cout << y->getKey() << std::endl;
    // std::cout << z->getKey() << std::endl;
    if (firstLeft && secondLeft) return 2; // First left then left
    else if (!firstLeft && !secondLeft) return 1; // First right then right
    else if (firstLeft && !secondLeft) return 3; // First left then right
    else return 4; // First right then left
}

template<class Key, class Value>
void AVLTree<Key, Value>::balance(AVLNode<Key, Value>* x, AVLNode<Key, Value>* y, AVLNode<Key, Value>* z, int balanceMode)
{
    // If straight line, y is going to be the parent of x and z, so updating x and z and their ancestors' heights includes y
    if (balanceMode == 1) // Single left
    {
        leftRotate(z);
        if (z == this->root_) this->root_ = y;
        updateHeights(x);
        updateHeights(z);
    }
    else if (balanceMode == 2) // Single right
    {
        rightRotate(z);
        if (z == this->root_) this->root_ = y;
        updateHeights(x);
        updateHeights(z);
    }
    // If zigzag, x is going to be the parent of y and z, so updating y and z and their ancestors' heights include x
    else if (balanceMode == 3) // Left then right
    {
        leftRotate(y);
        rightRotate(z);
        if (z == this->root_) this->root_ = x;
        updateHeights(y);
        updateHeights(z);
    }
    else if (balanceMode == 4) // Right then left
    {
        rightRotate(y);
        leftRotate(z);
        if (z == this->root_) this->root_ = x;
        updateHeights(y);
        updateHeights(z);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // The same insert as bst
    if (!this->root_)
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;
    bool lastDirection; // 0 means left, 1 means right
    while (node)
    {
        parent = node;
        if (new_item.first < node->getKey()) // If smaller, go left
        {
            node = node->getLeft();
            lastDirection = 0;
        }
        else if (new_item.first == node->getKey()) // If same key, update value
        {
            node->setValue(new_item.second);
            return;
        }
        else // If larger, go right
        {
            node = node->getRight();
            lastDirection = 1;
        }
    }
    // Insert at the appropriate position
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (lastDirection) parent->setRight(newNode);
    else parent->setLeft(newNode);

    // Begin AVL-specific insert implementation
    AVLNode<Key, Value>* temp = parent; // Need to declare a variable because we are using pointers which are changed in other functions
    updateHeights(temp); // After inserting a node, we need to update all its ancestors' heights
    AVLNode<Key, Value>* x = NULL;
    AVLNode<Key, Value>* y = NULL;
    AVLNode<Key, Value>* z = NULL;
    temp = parent;
    int balanceMode = -1;
    while (temp)
    {
        if (calculateBF(temp) >= 2) // This is the first unbalanced node
        {
            balanceMode = findXYZ(x, y, z, temp);
            break; // We can break because the tree is guaranteed to be balanced after a single/double rotationss
        }
        temp = temp->getParent();
    }
    if (x && y && z) balance(x, y, z, balanceMode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* findRes = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (findRes) // Only remove node that exists in the tree
    {
        AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(findRes->getParent());
        AVLNode<Key, Value>* predParent = NULL;
        if (!findRes->getLeft() && !findRes->getRight()) // Leaf node
        {
            if (parent) // If findRes is not root (i.e. parent is not NULL)
            {
                // Find out which direction is the node and delete it
                if (parent->getLeft() == findRes) parent->setLeft(NULL);
                else parent->setRight(NULL);
            }
            else this->root_ = NULL; // If we are deleting the root, set it to NULL
            delete findRes;
        }
        else if (findRes->getLeft() && !findRes->getRight() || !findRes->getLeft() && findRes->getRight()) // 1 child
        {
            AVLNode<Key, Value>* child = findRes->getLeft() ? findRes->getLeft() : findRes->getRight();
            if (parent) // If findRes is not root (i.e. parent is not NULL)
            {
                // Find out which direction is the child and promote it
                if (parent->getLeft() == findRes)
                {
                    delete findRes;
                    parent->setLeft(child);
                }
                else
                {
                    delete findRes;
                    parent->setRight(child);
                }
                child->setParent(parent);
            }
            else // If we are deleting root with only one child, promote the child as the new root
            {
                child->setParent(NULL);
                this->root_ = child;
                delete findRes;
            }
        }
        else // 2 children
        {
            // If there are 2 children, swap the node with predecessor and delete it
            AVLNode<Key, Value>* predecessor = static_cast<AVLNode<Key, Value>*>(this->predecessor(findRes));
            nodeSwap(findRes, predecessor); // Now findRes is predecessor, predecessor is findRes
            predParent = findRes->getParent();
            // Delete the node after swap. Furthermore, after swap, it mustn't be the root. So no need to detect root anymore
            // To be a predecessor, a node must only have 0 or 1 child. So only need to consider these 2 cases
            // Copy the code from above
            if (!findRes->getLeft() && !findRes->getRight()) // Leaf node
            {
                if (predParent->getLeft() == findRes) predParent->setLeft(NULL);
                else predParent->setRight(NULL);
                delete findRes;
            }
            else if (findRes->getLeft() && !findRes->getRight() || !findRes->getLeft() && findRes->getRight()) // 1 child
            {
                AVLNode<Key, Value>* child = findRes->getLeft() ? findRes->getLeft() : findRes->getRight();
                if (predParent->getLeft() == findRes)
                {
                    delete findRes;
                    predParent->setLeft(child);
                }
                else
                {
                    delete findRes;
                    predParent->setRight(child);
                }
                child->setParent(predParent);
            }
        }

        // Begin AVL-specific remove implementation
        int balanceMode = -1;
        AVLNode<Key, Value>* temp = predParent ? predParent : parent; // The start node may be swapped, so need to determine
        updateHeights(temp); // After removing a node, we need to update all its ancestors' heights
        AVLNode<Key, Value>* x = NULL;
        AVLNode<Key, Value>* y = NULL;
        AVLNode<Key, Value>* z = NULL;
        while (temp) // We need a while loop because the tree might be unbalanced higher up if we remove
        {
            // std::cout << "Key: " << temp->getKey() << " Height: " << temp->getHeight() << std::endl;
            if (calculateBF(temp) >= 2) // If this node is unbalanced
            {
                balanceMode = findXYZ(x, y, z, temp);
                // break; // We can not break this time because the tree may be unbalanced higher up
                if (balanceMode == 1 || balanceMode == 2) temp = y; // The new root of the subtree is y if straight line
                else temp = x; // The new root of the subtree is x if zigzag
                balance(x, y, z, balanceMode);
            }
            temp = temp->getParent();
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
