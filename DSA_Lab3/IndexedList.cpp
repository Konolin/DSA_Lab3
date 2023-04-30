#include <exception>

#include "IndexedList.h"
#include "ListIterator.h"

IndexedList::IndexedList() {
    capacity = 10;
    head = -1;
    firstEmpty = 0;

    elements = new TElem[capacity];
    next = new int[capacity];

    // set default positions for the next elements
    for (int index = 0; index < capacity; index++)
        next[index] = index + 1;
    next[capacity - 1] = -1;
}

int IndexedList::size() const {
    if (isEmpty()) return 0;

    // start at the head of the SLLA
    int size = 1;
    int currentIndex = head;
    int nextIndex = next[currentIndex];

    // go to the next element and increase the size, until there is no next element
    while (nextIndex != -1) {
        size++;
        currentIndex = nextIndex;
        nextIndex = next[currentIndex];
    }

    return size;
}


bool IndexedList::isEmpty() const {
    return head == -1;
}

TElem IndexedList::getElement(int pos) const {
    // check if "pos" is valid
    if (pos < 0) throw std::exception();
    if (pos >= capacity) throw std::exception();
    
    // start at the head and search for "pos" until it's found or the end of the SLLA is reached
    int currentPosition = head;
    while (currentPosition != pos && next[currentPosition] != -1) 
        currentPosition = next[currentPosition];
   
    // throw exception if "pos" was not found
    if (currentPosition != pos) throw std::exception();
    
    return elements[currentPosition];
}

TElem IndexedList::setElement(int pos, TElem e) {
    //TODO - Implementation
    return NULL_TELEM;
}

void IndexedList::addToEnd(TElem e) {
    if (isEmpty()) {
        // insert first element in the SLLA position
        int newPosition = firstEmpty;
        elements[newPosition] = e;
        firstEmpty = next[newPosition];
        next[newPosition] = -1;
        head = newPosition;
    } else {
        // check if there is need for a resize
        if (firstEmpty == -1) resize(2);

        // look for the last occupied position
        int lastOccupiedPosition = head;
        while (next[lastOccupiedPosition] != -1)
            lastOccupiedPosition = next[lastOccupiedPosition];

        // insert the element
        int newPosition = firstEmpty;
        elements[newPosition] = e;
        firstEmpty = next[newPosition];
        next[newPosition] = -1;
        // links the last element of the SLLA with the new element
        next[lastOccupiedPosition] = newPosition;
    }
}

void IndexedList::addToPosition(int pos, TElem e) {
    // check if the position is valid
    if (pos < 0) throw std::exception();

    // resize if necessary
    if (firstEmpty == -1) resize(2);

    // insert the new element on the first empty position
    int newPosition = firstEmpty;
    firstEmpty = next[firstEmpty];
    elements[newPosition] = e;
    next[newPosition] = -1;

    // add element on the first potion (change head)
    if (pos == 0) {
        // add the first element of the container
        if (head == -1) {
            head = newPosition;
        // add a new first element
        } else {
            next[newPosition] = head;
            head = newPosition;
        }
    // add in the middle of the container
    } else {
        // find the pos-th position
        int currentPosition = 0;
        int currentNode = head;

        // stop at the end of the container or when the position before "pos" is found
        while (currentNode != -1 && currentPosition < pos - 1) {
            currentPosition++;
            currentNode = next[currentNode];
        }

        // if the position is valid, add the new element
        if (currentNode != -1) {
            next[newPosition] = next[currentPosition];
            next[currentPosition] = newPosition;
        } else throw std::exception();
    }
}

TElem IndexedList::remove(int pos) {
    //TODO - Implementation
    return NULL_TELEM;
}

int IndexedList::search(TElem e) const {
    //TODO - Implementation
    return 0;
}

ListIterator IndexedList::iterator() const {
    return ListIterator(*this);
}

IndexedList::~IndexedList() {
    delete[] elements;
    delete[] next;
}

void IndexedList::resize(float factor) {
    int newCapacity = capacity * factor;
    auto newElements = new TElem[newCapacity];
    auto newLinks = new int[newCapacity];

    // move new elements in the new arrays
    for (int index = 0; index < capacity; index++) {
        newElements[index] = elements[index];
        newLinks[index] = links[index];
    }

    // initialize the empty next indexes
    for (int index = capacity; index < newCapacity; index++)
        newLinks[index] = index + 1;
    newLinks[newCapacity - 1] = -1;
    firstEmpty = capacity;

    // replace old arrays with the new ones
    delete[] elements;
    delete[] next;

    elements = newElements;
    next = newLinks;
    capacity = newCapacity;

    delete[] newElements;
    delete[] newLinks;
}
