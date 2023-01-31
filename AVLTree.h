#include <iostream>
using namespace std;

template <typename TYPE>
class AVLTree
{
private:
    class Node
    {
    public:
        TYPE data;
        Node* parent;
        Node* left;
        Node* right;
        int height;
    };
    int (*compare)(const TYPE& item1, const TYPE& item2);
    Node* root;
    int numNodes;
    TYPE lastInsertedData;

    void updateHeights(Node* beginNode, int beginHeight);
    void checkBalanceFactor(Node* curNode, bool wasInserted);
    void rebalanceAfterInsert(Node* node);
    void rebalanceAfterRemove(Node* node);
    void rightRotation(Node* mainNode);
    void leftRotation(Node* mainNode);

    //rebalanceAfterRemove helper function
    void remRotationHeights(Node* loweredNode);

    //printTree helper functions
    void printSpaces(Node* curNode, int spaces);
    void canItGoRight(Node* curNode, int spaces);
    void canItGoLeft(Node* curNode, int spaces);
    void moveUp(Node* parent, Node* curNode, int spaces);

    //remove helper functions
    Node* findAddress(TYPE item);
    void simpleCases(Node* toDelete);

    void cleanUp(Node* curNode);

public:
    AVLTree(int(*cmp)(const TYPE& item1, const TYPE& item2))
    {
        compare = cmp;
        root = nullptr;
        numNodes = 0;
    }
	~AVLTree(){
        cleanUp(root);
    }
    TYPE search(TYPE searchedFor);
    void insert(TYPE newData);
    void remove(TYPE item);
    int count();
    void printTree();
};

//----------printTree Functions----------

template <typename TYPE>
void AVLTree<TYPE>::printSpaces(Node* curNode, int spaces)
{
    while(spaces != 0)
    {
        cout << "      ";
        spaces --;
    }
    cout << "|_____";
    cout << curNode->data << "(" << curNode->height << ")" << endl;
}

template <typename TYPE>
void AVLTree<TYPE>::canItGoRight(Node* curNode, int spaces)
{
    if(curNode->right != nullptr)
    {
        curNode = curNode->right;
        spaces++;
        printSpaces(curNode, spaces);
        canItGoRight(curNode, spaces);
    }
    else
        canItGoLeft(curNode, spaces);
}

template <typename TYPE>
void AVLTree<TYPE>::canItGoLeft(Node* curNode, int spaces)
{
    if(curNode->left != nullptr)
    {
        curNode = curNode->left;
        spaces++;
        printSpaces(curNode, spaces);
        canItGoRight(curNode, spaces);
    }
    else
        moveUp(curNode->parent, curNode, spaces);
        
}

template <typename TYPE>
void AVLTree<TYPE>::moveUp(Node* parent, Node* curNode, int spaces)
{
    Node* oldChild = curNode;
    curNode = parent;
    spaces --;
    if(curNode == root) //are you at the root?
        return;
    else
    {
        if(curNode->left == oldChild)
            moveUp(curNode->parent, curNode, spaces);
        else
            canItGoLeft(curNode, spaces);
    }
}

template <typename TYPE>
void AVLTree<TYPE>::printTree()
{
    int spaces = 0;
    Node* curNode = root;
    cout << curNode->data << "(" << curNode->height << ")" << endl;
    //right side subtree
    if(curNode->right != nullptr)
    {
        printSpaces(curNode->right, spaces);
        canItGoRight(curNode->right, spaces);
    }
    //left side subtree
    
    if(curNode->left != nullptr)
    {
        printSpaces(curNode->left, spaces);
        canItGoRight(curNode->left, spaces);
    }
}

template <typename TYPE>
TYPE AVLTree<TYPE>::search(TYPE searchedFor)
{
    Node* searchedNode = findAddress(searchedFor);
    if(searchedNode != nullptr)
        return searchedNode->data;
    else
        return searchedFor;
}

template <typename TYPE>
void AVLTree<TYPE>::insert(TYPE newData)
{
    try{
        if (root == nullptr)
        {
            root = new Node();
            root->data = newData;
            lastInsertedData = newData;
            updateHeights(root, 1);
            numNodes++;
        }
        else
        {
            Node* curNode = root;
            while(curNode != nullptr)
            {
                if (compare(newData, curNode->data) == 0)
                    throw "No Duplicates Allowed";
                else if(compare(newData, curNode->data) == -1)
                {
                    if(curNode->left != nullptr) //if left child exists
                        curNode = curNode->left;
                    else // add in node
                    {
                        Node* newNode = new Node();
                        newNode->data = newData;

                        newNode->parent = curNode;
                        curNode->left = newNode;

                        numNodes++;
                        lastInsertedData = newData;
                        updateHeights(newNode, 1);
                        checkBalanceFactor(newNode, true);
                        return;
                    }
                } 
                else
                    if(curNode->right != nullptr) //if right child exists
                        curNode = curNode->right;
                    else // add in node
                    {
                        Node* newNode = new Node();
                        newNode->data = newData;

                        newNode->parent = curNode;
                        curNode->right = newNode;

                        numNodes++;
                        lastInsertedData = newData;
                        updateHeights(newNode, 1);
                        checkBalanceFactor(newNode, true);
                        return;
                    }
            }
        }
    } catch(const char* msg){
            cerr << msg << endl;
    }
}

//remove() & search() helper function
template <typename TYPE>
typename AVLTree<TYPE>::Node* AVLTree<TYPE>::findAddress(TYPE searchedFor)
{
    //if tree is empty
    try{
        if (root == nullptr)
            throw "Node Does Not Exist";
        else
        {
            Node* curNode = root;
            while(curNode != nullptr)
            {
                if (compare(searchedFor, curNode->data) == 0)
                    return curNode;
                else if(compare(searchedFor, curNode->data) == -1)
                    curNode = curNode->left;
                else
                    curNode = curNode->right;
            }
            throw "Node Does Not Exist";
        }
    } catch(const char* msg){
            cerr << msg << endl;
    }
    return nullptr;
}

//remove() helper function
template <typename TYPE>
void AVLTree<TYPE>::simpleCases(Node* toDelete)
{
    Node* tempParent = toDelete->parent;
    // no children
    if(toDelete->left == nullptr && toDelete->right == nullptr)
    {
        if(toDelete->parent->left == toDelete)//if toDelete is a left child
        {   
            toDelete->parent->left = nullptr;
            if(tempParent->right == nullptr) // changing the height if no sibling to toDelete
                updateHeights(tempParent, 1);
        }
        else if (toDelete->parent->right == toDelete)//if toDelete is a right child
        {
            toDelete->parent->right = nullptr;
            if(tempParent->left == nullptr) // changing the height if no sibling to toDelete
                updateHeights(tempParent, 1);
        }
        else //if toDelete is the root
            this->root = nullptr;
        toDelete->parent = nullptr;
    }
    // one child
    else if (toDelete->left == nullptr || toDelete->right == nullptr)
    {
        if(toDelete->parent->left == toDelete)//if toDelete is a left child
        {    
            if(toDelete->left != nullptr) //toDelete is a left child with a left child
            {
                toDelete->parent->left = toDelete->left;
                toDelete->left->parent = toDelete->parent;
                toDelete->parent = nullptr;
                toDelete->left = nullptr;
            }
            else //toDelete is a left child with a right child
            {
                toDelete->parent->left = toDelete->right;
                toDelete->right->parent = toDelete->parent;
                toDelete->parent = nullptr;
                toDelete->right = nullptr;
            }
            if(tempParent->right == nullptr || tempParent->right->height <= tempParent->left->height) // changing the height if no sibling to toDelete OR sibling height < left height
                updateHeights(tempParent, tempParent->left->height + 1);
        }
        else if (toDelete->parent->right == toDelete)//if toDelete is a right child
        {
            if(toDelete->left != nullptr) //toDelete is a right child with a left child
            {
                toDelete->parent->right = toDelete->left;
                toDelete->left->parent = toDelete->parent;
                toDelete->parent = nullptr;
                toDelete->left = nullptr;
            }
            else //toDelete is a right child with a right child
            {
                toDelete->parent->right = toDelete->right;
                toDelete->right->parent = toDelete->parent;
                toDelete->parent = nullptr;
                toDelete->right = nullptr;
            }
            if(tempParent->left == nullptr || tempParent->left->height <= tempParent->right->height) // changing the height if no sibling to toDelete OR sibling height < right height
                updateHeights(tempParent, tempParent->right->height + 1);
        }
        else //if toDelete is the root
        {
            if(toDelete->right != nullptr)
            {   
                this->root = toDelete->right;
                toDelete->right->parent = nullptr;
            }
            else
            {
                this->root = toDelete->left;
                toDelete->left->parent = nullptr;
            }
        }
    }
    if(tempParent != nullptr)
        checkBalanceFactor(tempParent, false);
}

template <typename TYPE>
void AVLTree<TYPE>::remove(TYPE item)
{
    Node* toDelete = findAddress(item);
    if(toDelete == nullptr)
        return;

    if(toDelete->left == nullptr || toDelete->right == nullptr)
        simpleCases(toDelete);
    else // two children
    {
        //find max on left subtree
        Node* maxNode = toDelete->left;
        while(maxNode->right != nullptr)
        {
            maxNode = maxNode->right;
        }
        //copy the value in toDelete
        toDelete->data = maxNode->data;
        //delete the duplicate
        simpleCases(maxNode);
    }
    numNodes--;
    //cout << "-----after remove() before rebalancing-----" << endl;
    //printTree();  
}

template <typename TYPE>
int AVLTree<TYPE>::count()
{
    return numNodes;
}

template <typename TYPE>
void AVLTree<TYPE>::checkBalanceFactor(Node* curNode, bool wasInserted)
{
    //cout << "inside checkBalanceFactor with " << curNode->data << endl;
    int BF;
    while(curNode != nullptr)
    {
        if(curNode->left == nullptr && curNode->right != nullptr) //only right child exists
            BF = curNode->right->height - 0;
        else if (curNode->right == nullptr && curNode->left != nullptr) //only left child exists
            BF = 0 - curNode->left->height;
        else if(curNode->right == nullptr && curNode->left == nullptr) //no children
            BF = 0;
        else // two children
            BF = curNode->right->height - curNode->left->height;

        //cout << "BF of " << curNode->data << ": " << BF << endl;

        if(BF < -1 || BF > 1)
        {
            if(wasInserted == true)
                rebalanceAfterInsert(curNode);
            if(wasInserted == false)
                rebalanceAfterRemove(curNode);
        }
        curNode = curNode->parent;
    }
    return;
}

template <typename TYPE>
void AVLTree<TYPE>::updateHeights(Node* beginNode, int beginHeight)
{
    //cout << "inside updateHeights with " << beginNode->data << "and beginHeight as " << beginHeight << endl;
    int otherHeight = 0;
    int maxHeight = 0;
    while(beginNode != nullptr)
    {
        beginNode->height = beginHeight;
        
        if(beginNode->parent != nullptr)
        {
            if(beginNode == beginNode->parent->left && beginNode->parent->right != nullptr) //beginNode is a left child AND has a right sibling
                otherHeight = beginNode->parent->right->height;
            else if(beginNode == beginNode->parent->right && beginNode->parent->left != nullptr) //beginNode is a right child AND has a left sibling
                otherHeight = beginNode->parent->left->height;
            else // no sibling
                otherHeight = 0;
            
            //cout << "otherHeight: " << otherHeight << endl;

            if(beginNode->height >= otherHeight)
                maxHeight = beginNode->height;
            else if(beginNode->height < otherHeight)
                maxHeight = otherHeight;
            beginNode = beginNode->parent;
            beginHeight = maxHeight + 1;
        }
        else
            return;
    }
}

/*
while(beginNode != nullptr)
    {
        beginNode->height = beginHeight;
        if(beginNode->parent != nullptr)
        {
            if(beginNode->parent->height - beginNode->height >= 1 && fromInsert == true) //other subtree is equal when inserting, so stop
                    return;
            //stopping when the other subtree is larger
            if(beginNode == beginNode->parent->left && beginNode->parent->right != nullptr) //its a left child with a right sibling
            {
                if(beginNode->parent->height - beginNode->parent->right->height == 1 && fromInsert == false) //right subtree is larger, so stop
                    return;
            }
            if(beginNode == beginNode->parent->right && beginNode->parent->left != nullptr) //its a right child with a left sibling
            {
                if(beginNode->parent->height - beginNode->parent->left->height == 1 && fromInsert == false)//left subtree is larger, so stop
                    return;
            }
        }
        beginNode = beginNode->parent;
        beginHeight++;
    }
*/

template <typename TYPE>
void AVLTree<TYPE>::rebalanceAfterInsert(Node* node)
{
    //cout << "inside rebalanceAfterInsert with " << node->data << endl;
    //cout << "lastInsertedData: " << lastInsertedData << endl;

    //inside case
    //right-left
    if(lastInsertedData > node->data && lastInsertedData < node->right->data) 
    {
        rightRotation(node->right->left);
        updateHeights(node->right->right, 1);
        leftRotation(node->right);
        if(node->left != nullptr) // if there was a left child
            updateHeights(node, node->left->height + 1);
        else
            updateHeights(node, 1);
    }
    //left-right
    else if(lastInsertedData < node->data && lastInsertedData > node->left->data) 
    {
        leftRotation(node->left->right);
        updateHeights(node->left->left, 1);
        rightRotation(node->left);
        if(node->right != nullptr) // if there was a right child
            updateHeights(node, node->right->height + 1);
        else
            updateHeights(node, 1);
    }
    
    //outside case
    //left-left
    else if(lastInsertedData < node->data && lastInsertedData < node->left->data)
    {
        rightRotation(node->left);
        if(node->right != nullptr) // if there was a right child
            updateHeights(node, node->right->height + 1);
        else
            updateHeights(node, 1);
    }
    //right-right
    else if(lastInsertedData > node->data && lastInsertedData > node->right->data)
    {
       leftRotation(node->right);
       if(node->left != nullptr) // if there was a left child
            updateHeights(node, node->left->height + 1);
       else
            updateHeights(node, 1);
    }
    return;
}

template <typename TYPE>
void AVLTree<TYPE>::remRotationHeights(Node* loweredNode)
{
    //cout << "in remRotationHeights with " << loweredNode->data << endl;
    //this function updates heights after rotations for rebalance after remove function

        if(loweredNode->right == nullptr || loweredNode->left == nullptr)
        {
            //out << "checkpoint: left or right child does not exist" << endl;
            if(loweredNode->right == nullptr && loweredNode->left != nullptr) //only has left child
                updateHeights(loweredNode, loweredNode->left->height + 1);
            else if (loweredNode->left == nullptr && loweredNode->right != nullptr) //only has a right child
                updateHeights(loweredNode, loweredNode->right->height + 1);
            else //has no children
            {
                //cout << "checkpoint: has no children, about to updateHeights" << endl;
                updateHeights(loweredNode, 1);
            }
        }
        else //Y has right and left  (finding higher subtree)
        {
            //cout << "right's height: " << loweredNode->right->height << "and left's height:" << loweredNode->left->height << endl;
            if(loweredNode->right->height > loweredNode->left->height) //right is higher subtree
                updateHeights(loweredNode, loweredNode->right->height + 1);
            else //left is higher or equal subtree
                updateHeights(loweredNode, loweredNode->left->height + 1);
        }
    return;
}

template <typename TYPE>
void AVLTree<TYPE>::rebalanceAfterRemove(Node* node)
{
    //cout << "inside rebalanceAfterRemove with " << node->data << endl;
    /*
    nodeZ = first node out of balance
    nodeY = the larger height child of nodeZ
    nodeX = the larger height child of nodeY
    (if heights are equal, it will the left child)
    */
    Node* nodeZ = node;
    Node* nodeY;
    Node* nodeX;
    //finding nodeY
    if(nodeZ->right != nullptr && nodeZ->left != nullptr)//nodeZ's left and right children exist
    {
        if(nodeZ->right->height > nodeZ->left->height)
            nodeY = nodeZ->right;
        else
            nodeY = nodeZ->left;
    }
    else if(nodeZ->right != nullptr && nodeZ->left == nullptr) //only nodeZ's right exists
        nodeY = nodeZ->right;
    else //only nodeZ's left child exists
        nodeY = nodeZ->left;
    
    //finding nodeX
    if(nodeY->right != nullptr && nodeY->left != nullptr)//nodeY's left and right children exist  
    {
        if(nodeY->right->height > nodeY->left->height)
            nodeX = nodeY->right;
        else
            nodeX = nodeY->left;
    }
    else if(nodeY->right != nullptr && nodeY->left == nullptr) //only nodeY's right exists
        nodeX = nodeY->right;
    else //only nodeY's left child exists
        nodeX = nodeY->left;
    /*
    cout << "nodeZ: " << nodeZ->data << endl;
    cout << "nodeY: " << nodeY->data << endl;
    cout << "nodeX: " << nodeX->data << endl;
    */

    //left-right
    if(nodeZ->left == nodeY && nodeY->right == nodeX)
    {
        //cout << "inside left-right" << endl;
        leftRotation(nodeX);
        //printTree();
        remRotationHeights(nodeY);
        //printTree(); 
        rightRotation(nodeX);
        //printTree();
        remRotationHeights(nodeZ);
        //printTree(); 
    }
    //right-left
    else if(nodeZ->right == nodeY && nodeY->left == nodeX)
    {
        //cout << "inside right-left" << endl;
        rightRotation(nodeX);
        //printTree();
        remRotationHeights(nodeY); 
        //printTree();
        leftRotation(nodeX);
        //printTree();
        remRotationHeights(nodeZ);
        //printTree();
    }
    //left-left
    else if(nodeZ->left == nodeY && nodeY->left == nodeX)
    {
        //cout << "inside left-left" << endl;
        rightRotation(nodeY);
        //printTree();
        remRotationHeights(nodeZ);
        //printTree();
    }
    //right-right
    else if(nodeZ->right == nodeY && nodeY->right == nodeX)
    {
        //cout << "inside right-right" << endl;
        leftRotation(nodeY);
        //printTree();
        remRotationHeights(nodeZ);
        //printTree();
    }
    checkBalanceFactor(nodeZ, false);
    return;
}

template <typename TYPE>
void AVLTree<TYPE>::rightRotation(Node* mainNode)
{
    //cout << "inside rightRotation with " << mainNode->data << endl;
    //right rotates mainNode(who is a left) and its parent
    Node* tempParent = mainNode->parent;
    Node* tempGParent = mainNode->parent->parent;
    Node* tempChild = mainNode->right;

    //parent becomes node's right child
    mainNode->right = tempParent;
    tempParent->parent = mainNode;
    //node's old right child (if there) will be parent's left child
    if(tempChild != nullptr)
    {
        tempParent->left = tempChild;
        tempChild->parent = tempParent;
    }
    else 
        tempParent->left = nullptr;
    //if grandparent exists
    if(tempGParent != nullptr)
    {
        //node becomes left/right child of grandparent
        if(compare(mainNode->data, tempGParent->data) == -1)
        {
            tempGParent->left = mainNode;
            mainNode->parent = tempGParent;
        }
        else
        {
            tempGParent->right = mainNode;
            mainNode->parent = tempGParent;
        }
    }
    //if there's no grandparent, aka changing root
    else
    {
        mainNode->parent = nullptr;
        this->root = mainNode;
    }
    return;
}

template <typename TYPE>
void AVLTree<TYPE>::leftRotation(Node* mainNode)
{
    //cout << "inside leftRotation with " << mainNode->data << endl;
    //left rotates mainNode(who is a right) with its parent
    Node* tempParent = mainNode->parent;
    Node* tempGParent = mainNode->parent->parent;
    Node* tempChild = mainNode->left;

    //parent becomes node's left child
    mainNode->left = tempParent;
    tempParent->parent = mainNode;
    //node's old left child (if there) will be parent's right child
    if(tempChild != nullptr)
    {
        tempParent->right = tempChild;
        tempChild->parent = tempParent;
    }
    else 
    tempParent->right = nullptr;
    //if grandparent exists
    if(tempGParent != nullptr)
    {
        //node becomes left/right child of grandparent
        if(compare(mainNode->data, tempGParent->data) == -1)
        {
            tempGParent->left = mainNode;
            mainNode->parent = tempGParent;
        }
        else
        {
            tempGParent->right = mainNode;
            mainNode->parent = tempGParent;
        }
    }
    //if there's no grandparent, aka changing root
    else
    {
        mainNode->parent = nullptr;
        this->root = mainNode;
    }
    return;
}

template <typename TYPE>
void AVLTree<TYPE>::cleanUp(Node* curNode)
{  
    if (curNode == nullptr) 
        return;
 
    /* first delete both subtrees */
    cleanUp(curNode->left);
    cleanUp(curNode->right);
     
    /* then delete the node */
    delete curNode;   
}