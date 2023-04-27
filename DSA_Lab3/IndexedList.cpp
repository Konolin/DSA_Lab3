#include <exception>

#include "IndexedList.h"
#include "ListIterator.h"

IndexedList::IndexedList() {
    capacity = 10;
    head = -1;
    firstEmpty = 0;

    elements = new TElem[capacity];
    next = new int[capacity];

    for (int index = 0; index < capacity; index++)
        next[index] = index + 1;
    next[capacity - 1] = -1;
}

int IndexedList::size() const {
    if (isEmpty()) return 0;

    int size = 1;
    int currentIndex = head;
    int nextIndex = next[currentIndex];

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
    //TODO - Implementation
    return NULL_TELEM;
}

TElem IndexedList::setElement(int pos, TElem e) {
    //TODO - Implementation
    return NULL_TELEM;
}

void IndexedList::addToEnd(TElem e) {
    if (isEmpty()) { // insert first element
        int newPosition = firstEmpty;
        elements[newPosition] = e;
        firstEmpty = next[newPosition];
        next[newPosition] = head;
        head = newPosition;
    } else {
        if (firstEmpty == -1) resize(2);

        int lastOccupiedPosition = head;
        while (next[lastOccupiedPosition] != -1)
            lastOccupiedPosition = next[lastOccupiedPosition];

        int newPosition = firstEmpty;
        elements[newPosition] = e;
        firstEmpty = next[newPosition];
        next[newPosition] = -1;
        next[lastOccupiedPosition] = newPosition;
    }
}

void IndexedList::addToPosition(int pos, TElem e) {
    if (pos < 0) throw std::exception();

    if (firstEmpty == -1) resize(2);

    int newPosition = firstEmpty;
    firstEmpty = next[firstEmpty];
    elements[newPosition] = e;
    next[newPosition] = -1;

    if (pos == 0) {
        if (head == -1) {
            head = newPosition;
        } else {
            next[newPosition] = head;
            head = newPosition;
        }
    } else {

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
        newLinks[index] = newLinks[index];
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
}