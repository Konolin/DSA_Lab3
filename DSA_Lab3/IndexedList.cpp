#include <exception>

#include "IndexedList.h"
#include "ListIterator.h"


// worst = average = best = θ(n)
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


// worst = average = best = θ(n)
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

// worst = average = best = θ(1)
bool IndexedList::isEmpty() const {
    return head == -1;
}


// worst = average = θ(n)
// best = θ(1)
TElem IndexedList::getElement(int pos) const {
    // check if "pos" is valid
    if (pos < 0 || pos >= capacity) throw std::exception();

    // start at the head and search for "pos" until it's found or the end of the SLLA is reached
    int currentIndex = head;
    int currentPosition = 0;
    while (currentPosition != pos && currentIndex != -1) {
        currentIndex = next[currentIndex];
        currentPosition++;
    }

    // throw exception if "pos" was not found
    if (currentPosition != pos) throw std::exception();

    return elements[currentIndex];
}


// worst = average = θ(n)
// best = θ(1)
TElem IndexedList::setElement(int pos, TElem e) {
    // check if pos is valid
    if (pos < 0 || pos >= size()) throw std::exception();

    int currentIndex = head;
    int currentPosition = 0;
    TElem oldValue;

    // find the position
    while (currentPosition != pos && currentIndex != -1) {
        currentIndex = next[currentIndex];
        currentPosition++;
    }

    // add element
    oldValue = elements[currentIndex];
    elements[currentIndex] = e;

    return oldValue;
}


// worst = average = θ(n)
// best = θ(1)
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
        if (firstEmpty == -1) resize_up();

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

// worst = average = θ(n)
// best = θ(1)
void IndexedList::addToPosition(int pos, TElem e) {
    // check if the position is valid
    if (pos < 0) throw std::exception();
    if (pos >= size()) throw std::exception();

    // resize if necessary
    if (firstEmpty == -1) resize_up();

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

        // stop when the position before "pos" is found
        while (currentPosition < pos - 1) {
            currentPosition++;
            currentNode = next[currentNode];
        }

        next[newPosition] = next[currentNode];
        next[currentNode] = newPosition;
    }
}

// worst = average = θ(n)
// best = θ(1)
TElem IndexedList::remove(int pos) {
    // check if pos is valid
    if (pos < 0 || pos >= size()) throw std::exception();

    int currentIndex = head;
    int currentPosition = 0;
    int previousIndex = -1;
    TElem removedValue;

    // find the position in list
    while (currentPosition != pos && currentIndex != -1) {
        previousIndex = currentIndex;
        currentIndex = next[currentIndex];
        currentPosition++;
    }

    // throw exception if pso is not found
    if (currentPosition != pos) throw std::exception();

    removedValue = elements[currentIndex];

    // check if head needs to be modified
    if (currentIndex == head) {
        if (size() == 1) head = -1;
        else head = next[currentIndex];
    } else next[previousIndex] = next[currentIndex];

    // update empty position
    next[currentIndex] = firstEmpty;
    firstEmpty = currentIndex;

    // check if resize is necessary
    if (size() < capacity / 4) resize(0.5);

    return removedValue;
}


// worst = average = θ(n)
// best = θ(1)
int IndexedList::search(TElem e) const {
    int currentElement = head;
    int currentPosition = 0;

    // traverse the list
    while (currentElement != -1) {
        // if the position was found, return it
        if (elements[currentElement] == e)
            return currentPosition;
        currentElement = next[currentElement];
        currentPosition++;
    }

    return -1;
}


// worst = average = best = θ(n)
ListIterator IndexedList::iterator() const {
    return ListIterator(*this);
}


// worst = average = best = θ(n)
IndexedList::~IndexedList() {
    delete[] elements;
    delete[] next;
}


// worst = average = best = θ(n)
void IndexedList::resize_up() {
    int newCapacity = capacity * SIZE_UP_FACTOR;
    auto newElements = new TElem[newCapacity];
    auto newNext = new int[newCapacity];

    // move new elements in the new arrays
    for (int index = 0; index < capacity; index++) {
        newElements[index] = elements[index];
        newNext[index] = next[index];
    }

    // initialize the empty next indexes
    for (int index = capacity; index < newCapacity; index++)
        newNext[index] = index + 1;
    newNext[newCapacity - 1] = -1;
    firstEmpty = capacity;

    // replace old arrays with the new ones
    delete[] elements;
    delete[] next;

    elements = newElements;
    next = newNext;
    capacity = newCapacity;
}

void IndexedList::resize_down() {
    int newCapacity = capacity / SIZE_DOWN_FACTOR;
    auto newElements = new TElem[newCapacity];
    auto newNext = new int[newCapacity];

    // move existing elements to the new arrays
    for (int index = 0; index < size(); index++) {
        newElements[index] = elements[index];
        newNext[index] = next[index];
    }

    // replace old arrays with the new ones
    delete[] elements;
    delete[] next;

    elements = newElements;
    next = newNext;
    capacity = newCapacity;
}
