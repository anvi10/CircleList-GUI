#include "clist.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;

CircleList::CircleList() {
    header = NULL;
}

//find out what this means
CircleList::~CircleList() {

	if (header == NULL)
		return;

    CNode* node = header;
    
    while(node->next != header) {
        delete node;
        node = node->next;
    }
    delete node;
}

bool
CircleList::empty() const {
    //TODO 1
    //checks if list is empty
    //check if header is empty
    return header==NULL;
}

const Elem&
CircleList::front() const {
    //TODO 1
    //returns the first element
    
    //check if list is empty
    if(empty()) {
        //throw error
    }
    
    return header->elem;
}

const Elem&
CircleList::back() const {
    //TO
    //returns the last element
    
    //check if list is empty
    if(empty()) {
        //throw error
    }
    
    CNode* previous = header->prev;
    return previous->elem;
}

void
CircleList::add(CNode * v, const Elem& e) {
    //adds a new element e before v
    
    //check if we have empty list
    if(v==NULL) {
        header = new CNode();
        header->elem = e;
        header->next = header;
        header->prev = header;
    }
    else {
        CNode* node = new CNode();
        node->elem = e;
        
        node->prev = v->prev;
        node->prev->next = node;
        
        node->next = v;
        v->prev = node;
    }

   _size++;
}

void
CircleList::addFront(const Elem& e) {
    //TODO 1
    //adds before header
    add(header, e);
    
    //sets element before header to the header
    header = header->prev;
}

void
CircleList::addBack(const Elem& e) {
    //TODO 1
    //adds before header
    add(header, e);
}

void
CircleList::remove(CNode * v) {
    //TODO 1
    //removes a specific node form the list
    if(empty()) {
        //no nodes to remove
        //header->elem == NULL
        return;
    }
    else {
        if(v->prev == v) {
            //1 node
            delete header;
            header = NULL;
        }
        else {
            //2+ nodes
            CNode* prevNode = v->prev; //b
            CNode* nextNode = v->next; //a
            
            prevNode->next = nextNode; //a
            nextNode->prev = prevNode; //b
            
            if(v == header) {
                header = nextNode;
            }
            
            delete v;
        }
    }
}

void
CircleList::removeFront() {
    //removes the front element
    remove(header);
}

void
CircleList::removeBack() {
    //removes the back element
    remove(header->prev);
}

void
CircleList::clear() {
    //removes all the elements
    if (empty())
    	return;
CNode * node = header;	
    while (node->next != header) {
        	node = node->next;
	    remove(node->prev);
    }
    
    remove(header);
}

Elem&
CircleList::operator [] (int i) const {
    //assume header = [0]
    CNode* node = header;
    
    if(i<0) {
        for(int j = 0; j>i; j--) {
            node = node->prev;
        }
    }
    else {
        for(int j = 0; j<i; j++) {
            node = node->next;
        }
    }
    
    return node->elem;
}

void
CircleList::setElemAt (int i, const Elem newElem) const{
    CNode* node = header;
    
    if(i<0) {
        for(int j = 0; j>i; j--) {
            node = node->prev;
        }
    }
    else {
        for(int j = 0; j<i; j++) {
            node = node->next;
        }
    }
    
    node->elem = newElem;
}

void
CircleList::print() const {
    //prints all elements
    if(empty()) {
        return;
    }
    else {
        CNode* node = header;
        
        while(node->next != header) {
            
            cout << "X: " << node->elem.getX() << " Y: " << node->elem.getY() << endl; //replace with proper print call
            node = node->next;
        }
        
        cout << "X: " << node->elem.getX() << " Y: " << node->elem.getY() << endl; //replace with proper print call
    }
}

void
CircleList::addAt(int i, const Elem& e) {
    //add elem e at index i
    CNode* node = header;

    if(i<0) {
        for(int j = 0; j>i; j--) {
            node = node->prev;
        }
    }
    else {
        for(int j = 0; j<i; j++) {
            node = node->next;
        }
    }
    
    add(node, e);
    if(i == 0) {
        header = header->prev;
    }
}

void
CircleList::removeAt(int i) {
    //remove ith element
    CNode* node = header;
    
    if(i<0) {
        for(int j = 0; j>i; j--) {
            node = node->prev;
        }
    }
    else {
        for(int j = 0; j<i; j++) {
            node = node->next;
        }
    }
    
    remove(node);
}
