/*******************************************************************************
* Name        : rbtree.h
* Author      : Tyler Christensen and Samantha Bryan
* Version     : 1.0
* Date        : 11/18/2024
* Description : Implementation of red-black tree.
* Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
******************************************************************************/
#ifndef RBTREE_H_
#define RBTREE_H_

#include "node.h"
#include "tree.h"
#include "treeprinter.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <limits.h>
#include <queue>

using namespace std;


// Forward declaration
template <typename K, typename V>
class RedBlackTree;




/**
* tree_exception class
* Demonstrates how you can write your own custom exceptions in C++.
*/
class tree_exception: public std::exception {
public:
    tree_exception(const std::string &message) : message_{message} { }
    
    ~tree_exception() throw() { }
    
    virtual const char* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;
};


template<typename K, typename V>
class RedBlackTree : public Tree<K> {
public:
    /**
    * Constructor to create an empty red-black tree.
    */
    explicit RedBlackTree() : root_{nullptr}, size_{0} { }


    /**
    * Constructor to create a red-black tree with the elements from the
    * vector.
    */
    explicit RedBlackTree(std::vector< std::pair<K, V> > &elements):
                                            root_(nullptr), size_(0) {
    insert_elements(elements);
    }

    /**
    * Destructor.
    */
    ~RedBlackTree() {
        delete_tree(root_);
    }




    /**
    * Inserts elements from the vector into the red-black tree.
    * Duplicate elements are not inserted.
    */
    void insert_elements(std::vector< std::pair<K, V> > &elements){
        for(size_t i = 0, len = elements.size(); i < len; ++i){
            try{
                //cout << "hello" << endl;
                insert(elements[i].first, elements[i].second);
                //to_ascii_drawing();
            } 
            catch(const tree_exception &te){
               std::cerr << "Warning: " << te.what() << std::endl;
            }
        }
    }




    /**
    * Inserts a key-value pair into the red black tree.
    *
    * MUST throw a tree_exception if attempting to insert a key that is
    * already present in the tree.
    */
    void insert(const K &key, const V &value){
        //cout << "113" << endl;
        Node<K, V> *x = root_, *y = nullptr; // `x` is the current node, `y` is the parent
        Node<K, V> *z = new Node<K, V>(key, value); // Create the new node, initialized as red

        std::ostringstream oss;
        std::string message = "Attempt to insert duplicate key '";
        oss << message << z->key() << "'.";

        std::string final_string = oss.str();

        bool exit = false;

        // Traverse the tree to find the correct position for `z`
        while(x != nullptr){
            y = x; // Track the parent of the current node
            if(z->key() < x->key()){
                x = x->left; // Go left
            }
            else if(z->key() > x->key()){
                x = x->right; // Go right
            }
            else{
                // Duplicate key found
                delete z; // Avoid memory leak
                throw tree_exception(final_string);
                exit = true;
            }
        }


        if(exit == false){
            // Set `z`'s parent to `y`
            z->parent = y;

            // Insert `z` as the appropriate child of `y`
            if(y == nullptr){
                root_ = z; // Tree was empty, `z` becomes the root
            }
            else if(z->key() < y->key()){
                y->left = z;
            }
            else{
                y->right = z;
            }

           // Initialize `z`'s children as null and its color as red
           z->left = nullptr;
           z->right = nullptr;
           z->color = RED;


           // Fix potential red-black tree violations
           insert_fixup(z);
           size_ = size_ + 1;
       }
   }


   /**
    * Returns an ASCII representation of the red-black tree.
    */
   std::string to_ascii_drawing() {
       BinaryTreePrinter<K, V> printer(root_);
       return printer.to_string();
   }


   /**
    * Returns the height of the red-black tree.
    */
   int height() const {
       return height(root_) - 1;
   }


   /**
    * Returns the number of nodes in the red-black tree.
    */
   size_t size() const {
       return size_;
   }


   /**
    * Returns the leaf count of the red-black tree.
    */
   size_t leaf_count() const {
       return leaf_count(root_);
   }


   /**
    * Returns the internal node count of the red-black tree.
    */
   size_t internal_node_count() const {
       return internal_node_count(root_);
   }


   /**
    * Returns the diameter of the red-black tree rooted at node.
    * Diameter is defined as the length of the longest path between
    * two (non-null) leaves in the tree. The path does not necessarily have to
    * pass through the root.
    */
   size_t diameter() const {
       return diameter(root_);
   }


   /**
    * Returns the max width of the red-black tree, i.e. the largest number of
    * nodes on any level.
    */
   size_t max_width() const {
       size_t max_width = 0;
       for (int i = 0, h = height() + 1; i < h; i++) {
           size_t w = width(root_, i);
           if (w > max_width) {
               max_width = w;
           }
       }
       return max_width;
   }


   /**
    * Returns the successful search cost, i.e. the average number of nodes
    * visited to find a key that is present.
    */
   double successful_search_cost() const {
       return size_ == 0 ? 0 : 1 + (double)sum_levels() / size_;
   }


   /**
    * Returns the unsuccessful search cost, i.e. the average number of nodes
    * visited to find a key that is not present.
    */
   double unsuccessful_search_cost() const {
       return (double)sum_null_levels() / null_count();
   }


   /**
    * Searches for item. If found, returns the node
    * for it in the tree; otherwise, returns nullptr.
    */
    Node<K, V> *find(const K &key) {
       Node<K, V> *x = root_;
       while (x != nullptr) {
           const K& current_key = x->key();
           if (key == current_key) {
               break; // Found!
           } else if (key < current_key) {
               x = x->left;
           } else {
               x = x->right;
           }
       }
       return x;
   }


   std::vector<K> inorder_traversal_helper(Node<K, V> *current) const {
       std::vector<K> result;
       if(current == nullptr) {
           return result;
       }
       for(auto const &element : inorder_traversal_helper(current->left)) {
           result.push_back(element);
       }
       result.push_back(current->key());
       for(auto const &element:  inorder_traversal_helper(current->right)) {
           result.push_back(element);
       }
       return result;
   }


   std::vector<K> inorder_traversal() const {
       return inorder_traversal_helper(root_);
   }




private:
   Node<K, V> *root_;
   size_t size_;


   /**
    * Deletes all nodes from the red-black tree.
    */
   void delete_tree(Node<K, V> *n) {
       if (n == nullptr) {
           return; // Base case: empty node
       }
       delete_tree(n->left);  // Recursively delete left subtree
       delete_tree(n->right); // Recursively delete right subtree
       delete n; // Delete the current node
       size_ = 0;
   }


   /**
    * Fixup method described on p. 316 of CLRS.
    */
   
   void insert_fixup(Node<K, V> *z) {
       // TODO
        while (z->parent != nullptr && z->parent->parent != nullptr && z->parent->color == RED && z != nullptr){
           if (z->parent->parent->left != nullptr && z->parent == z->parent->parent->left) { // If z's parent is a left child
               Node <K, V>* y = z->parent->parent->right; // then z's uncle y is the right child
               if (y != nullptr && y->color == RED) {  //Case 1: z's uncle y is red
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }

               else{
                   if (z != root_ && z->parent != root_) {
                       if (z == z->parent->right) { //Case 2a: z's uncle y is black and z is a right child
                           z = z->parent; //solving for case 2a
                           left_rotate(z);//solving for case 2a
                       }
                   }               
                   if (z != root_){
                       if (z->parent != nullptr && z->parent->parent != nullptr){
                        z->parent->color = BLACK; //solving for case 3a: z's uncle y is black and z is a left child
                        z->parent->parent->color = RED; //solving for case 3a
                        right_rotate(z->parent->parent); //solving for case 3a
                       }
                    }
               }
           } 
           
           else{ //Symmetrical case, same as above with "right" and "left" reversed
               Node <K, V>* y = z->parent->parent->left; // then z's uncle y is the right child
               if (y != nullptr && y->color == RED) {  //Case 1: z's uncle y is red
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } 
               

                else{
                    if (z != root_ && z->parent != root_) {
                        if (z == z->parent->left) { //Case 2a: z's uncle y is black and z is a right child
                            z = z->parent; //solving for case 2a
                            right_rotate(z);//solving for case 2a
                        }
                    }               
                    if (z != root_){
                        if (z->parent != nullptr && z->parent->parent != nullptr){
                            z->parent->color = BLACK; //solving for case 3a: z's uncle y is black and z is a left child
                            z->parent->parent->color = RED; //solving for case 3a
                            left_rotate(z->parent->parent); //solving for case 3a
                        }
                    }
                }
            }
        }

        //before completing the algorithm, make sure the root's color is black
        // Last line below
        root_->color = BLACK;

    }



   /**
    * Left-rotate method described on p. 313 of CLRS.
    */
   void left_rotate(Node<K, V> *x) {
       if (x == nullptr || x->right == nullptr){
           return; // Safeguard
       }
       Node<K, V>* y = x->right;        //Set y
       x->right = y->left;   //Turn y's left subtree into x's right subtree
     
       if(y->left != nullptr){
           y->left->parent = x;
       }

       y->parent = x->parent;  //Link x's parent to y

       if(x->parent == nullptr){
           root_ = y;
       }
       else if(x == x->parent->left){
           x->parent->left = y;
       }
       else{
           x->parent->right = y;
       }

       y->left = x; //Put x on y's left
       x->parent = y;    //assign y as x's parent
   }




   /**
    * Right-rotate method described on p. 313 of CLRS.
    */
   void right_rotate(Node<K, V> *x) {
       if (x == nullptr || x->left == nullptr){
           return;
       }
       Node<K, V>* y = x->left;        //Set y
       x->left = y->right;   //Turn y's right subtree into x's left subtree
     
       if(y->right != nullptr){
           y->right->parent = x;
       }


       y->parent = x->parent;  //Link x's parent to y


       if(x->parent == nullptr){
           root_ = y;
       }
       else if(x == x->parent->right){
           x->parent->right = y;
       }
       else{
           x->parent->left = y;
       }


       y->right = x; //Put x on y's right
       x->parent = y;    //assign y as x's parent
   }




   /**
    * Returns the height of the red-black tree starting at node.
    * A null node starts at height -1.
    */
   size_t height(Node<K, V> *node) const {
       if (node == nullptr){
           return 0;
       }
       if(node->left == nullptr && node->right == nullptr){
           return 1;
       }
       else {
           return 1 + max(height(node->left), height(node->right)); //find the max in left and right subtrees, incrementing by 1 for each level
       }
   }


   /**
    * Returns the count of leaves in the red-black tree starting at node.
    * For this method, a leaf is a non-null node that has no children.
    */
   size_t leaf_count(Node<K, V> *node) const {
       // TODO
       if (node == nullptr){ //if the node is null, stop recursing and return 0 because leaves must be non-null
           return 0;
       }


       if (node != nullptr && node->left == nullptr && node->right == nullptr) { //if node has no children, count the node as a leaf
           return 1;
       }
       return leaf_count(node->right) + leaf_count(node->left); //recursive call to count leaves in right and left subtrees
   }


   /**
    * Returns the count of internal nodes in the red-black tree starting at
    * node.
    * An internal node has at least one child.
    */
   size_t internal_node_count(Node<K, V> *node) const {
       // TODO
       if (node == nullptr){
           return 0;
       }
       else if (node->right == nullptr && node->left == nullptr) { //left and right null
           return 0;
       }
     
       else if (node->right != nullptr && node->left != nullptr) { ///left and right non-null
           return 1 + internal_node_count(node->left) + internal_node_count(node->right); //count the current node and add it to the recursive call on its children
       }
     
       else if (node->right != nullptr && node->left == nullptr){ //left null and right non-null
           return 1 + internal_node_count(node->right); //count the current node and add it to the the recursive call on the right child
       }
     
       else { //right null and left non-null
           return 1 + internal_node_count(node->left); //count the current node and add it to the recursive call on the left child
       }
   }


   /**
    * Helper method to assist in the computation of tree diameter.
    */
   size_t diameter(Node<K, V> *node) const {
       // TODO
       if(node == nullptr){
           return 0;
       }




       if (node->left == nullptr && node->right == nullptr){
           return 0;
       }




       size_t lefth;
       if (node->left == nullptr){
           lefth = 0;
       } else {
           lefth = height(node->left);
       }




       size_t righth;
       if (node->right == nullptr){
           righth = 0;
       } else {
           righth = height(node->right);
       }


       return lefth + righth;
   }


   /**
    * Returns the width of the red-black tree at the designated level.
    * Width is defined as the number of nodes residing at a level.
    */
   size_t width(Node<K, V> *node, size_t level) const {
       // TODO
       if (node == nullptr){ //empty tree
           return 0;
       }
       if (level == 0) { //first level has one node
           return 1;
       }
     
       queue<Node<K,V>*> q;
       q.push(node);




       while (height() - tree_level(q.front()) + 1 != level){ //check if the queue has reached the desired level
           Node<K, V>* current = q.front();
           q.pop();
           if (current->left != nullptr){ //add left and right children to the queue if non-null
               q.push(current->left);
           }
           if (current->right != nullptr){
               q.push(current->right);
           }
       }
       return q.size(); //length of the queue will be the number of nodes at the designated level
   }






   /**
   Returns the level of a node in the tree.
   Helper function for the width
    */
   size_t tree_level(Node<K, V> *node) const{
       if (node == root_){
           return 0;
       }
       else {
           return 1 + tree_level(node->parent);
       }
   }


   size_t null_count() const {
       return null_count(root_);
   }


   /**
    * Returns the count of null pointers in the red-black tree starting at
    * node.
    */
   size_t null_count(Node<K, V> *node) const {
       // TODO
       if (node == nullptr){ //empty tree has null pointers left and right
           return 2;
       }
     
       queue<Node<K,V>*> q;
       q.push(node);


       size_t nullcount = 0;


       while (!q.empty()){ //iterate until all nodes in the tree have been visited
           Node<K, V>* current = q.front();
           q.pop();
           if (current->left != nullptr){ //add left and right children to the queue if non-null. increment nullcount if they are null
               q.push(current->left);
           } else {
               nullcount++;
           }
           if (current->right != nullptr){
               q.push(current->right);
           } else {
               nullcount++;
           }
       }
       return nullcount;
   }


   size_t sum_levels() const {
       return sum_levels(root_, 0);
   }


   /**
    * Returns the sum of the levels of each non-null node in the red-black
    * tree starting at node.
    * For example, the tree
    *   5 <- level 0
    *  / \
    * 2   8 <- level 1
    *      \
    *       10 <- level 2
    * has sum 1*0 + 2*1 + 1*2 = 4.
    */
   size_t sum_levels(Node<K, V> *node, size_t level) const {
       // TODO
       if (node == nullptr) {
           return 0;
       }
       return level + sum_levels(node->left, level + 1) + sum_levels(node->right, level + 1); //count the current level and recursively add the levels of the node's children
   }


   size_t sum_null_levels() const {
       return sum_null_levels(root_, 0);
   }




   /**
    * Returns the sum of the levels of each null node in the red-black tree
    * starting at node.
    * For example, the tree
    *    5 <- level 0
    *   / \
    *  2   8 <- level 1
    * / \ / \
    * * * * 10 <- level 2
    *       / \
    *       * * <- level 3
    * has sum 3*2 + 2*3 = 12.
    */
   size_t sum_null_levels(Node<K, V> *node, size_t level) const {
       // TODO
       size_t sum = 0;








       if (node == nullptr) {
           sum += level;
           return sum;
       }


       sum += sum_null_levels(node->left, level + 1);
       sum += sum_null_levels(node->right, level + 1);
       return sum;
   }
};




#endif /* RBTREE_H_ */
