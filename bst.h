#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);
    virtual void remove(const Key& key);
    void clear();
    bool isBalanced() const;
    void print() const;
    bool empty() const;
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const;
    Node<Key, Value> *getSmallestNode() const; 
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    int getHeight(Node<Key, Value>* node) const;
    Node<Key, Value>* internalFindHelper(const Key& k, Node<Key, Value>* node) const;
    bool isBalancedHelper(Node<Key, Value>* node) const;
    void clearHelper(Node<Key, Value>* node);

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/


/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_ (NULL)
{
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->current_ != rhs.current_;
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    if (!current_) return *this;
    if (current_->getRight()) // If current node has right subtree
    {
        current_ = current_->getRight();
        while (current_->getLeft()) current_ = current_->getLeft(); // Find the successor
        return *this;
    }
    // If current node has no right subtree and is the right child of parent, return its parent's parent
    // If current node has no right subtree and is the left child of parent, return its parent
    // While loop is to avoid infinite loop if the tree is all the way to the right like a linked list
    while (current_->getParent() && current_->getParent()->getRight() == current_) current_ = current_->getParent(); // If right child, go back one node
    current_ = current_->getParent();
    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

template<class Key, class Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* node) const
{
    if (!node) return 0;
    int leftHeight = getHeight(node->getLeft());
    int rightHeight = getHeight(node->getRight());
    return std::max(leftHeight, rightHeight) + 1;
}

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL)
{
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    this->clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    if (!root_)
    {
        root_ = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        return;
    }
    Node<Key, Value>* node = root_;
    Node<Key, Value>* parent = NULL;
    bool lastDirection; // 0 means left, 1 means right
    while (node)
    {
        parent = node;
        if (keyValuePair.first < node->getKey()) // If smaller, go left
        {
            node = node->getLeft();
            lastDirection = 0;
        }
        else if (keyValuePair.first == node->getKey()) // If same key, update value
        {
            node->setValue(keyValuePair.second);
            return;
        }
        else // If larger, go right
        {
            node = node->getRight();
            lastDirection = 1;
        }
    }
    // Insert at the appropriate position
    Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, parent);
    if (lastDirection) parent->setRight(newNode);
    else parent->setLeft(newNode);
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    Node<Key, Value>* findRes = internalFind(key);
    if (findRes) // Only remove node that exists in the tree
    {
        Node<Key, Value>* parent = findRes->getParent();
        if (!findRes->getLeft() && !findRes->getRight()) // Leaf node
        {
            if (parent) // If findRes is not root (i.e. parent is not NULL)
            {
                // Find out which direction is the node and delete it
                if (parent->getLeft() == findRes) parent->setLeft(NULL);
                else parent->setRight(NULL);
            }
            else root_ = NULL;
            delete findRes;
        }
        else if (findRes->getLeft() && !findRes->getRight() || !findRes->getLeft() && findRes->getRight()) // 1 child
        {
            Node<Key, Value>* child = findRes->getLeft() ? findRes->getLeft() : findRes->getRight();
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
                root_ = child;
                delete findRes;
            }
        }
        else // 2 children
        {
            // If there are 2 children, swap the node with predecessor and delete it
            Node<Key, Value>* predecessor = this->predecessor(findRes);
            nodeSwap(findRes, predecessor); // Now findRes is predecessor, predecessor is findRes
            Node<Key, Value>* predParent = findRes->getParent();
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
                Node<Key, Value>* child = findRes->getLeft() ? findRes->getLeft() : findRes->getRight();
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
    }
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    Node<Key, Value>* predecessor = current->getLeft(); // Go left
    if (predecessor == NULL) return NULL;
    while (predecessor->getRight()) predecessor = predecessor->getRight(); // Then go all the way right
    return predecessor;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelper(Node<Key, Value>* node)
{
    if (node)
    {
        clearHelper(node->getLeft());
        clearHelper(node->getRight());
    }
    delete node;
}

/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    clearHelper(root_);
    root_ = NULL; // Need to reset root to NULL to prevent users from accessing the tree again using the root pointer after it is deleted
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    if (!root_) return NULL;
    Node<Key, Value>* node = root_;
    while(node->getLeft()) node = node->getLeft(); // Go all the way left
    return node;
}

// Helper function for internalFind()
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFindHelper(const Key& key, Node<Key, Value>* node) const
{
    if (node)
    {
        const Key& curKey = node->getKey();
        if (curKey == key) return node;
        else if (curKey > key) return internalFindHelper(key, node->getLeft());
        else return internalFindHelper(key, node->getRight());
    }
    return node; // Only reached if no item with that key exists, return NULL
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    return internalFindHelper(key, root_);
}

// Helper function for isBalanced()
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalancedHelper(Node<Key, Value>* node) const
{
    if (!node) return true;
    int leftHeight = getHeight(node->getLeft());
    int rightHeight = getHeight(node->getRight());
    if (abs(leftHeight - rightHeight) > 1) return false;
    return isBalancedHelper(node->getLeft()) && isBalancedHelper(node->getRight());
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    return isBalancedHelper(root_);
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
