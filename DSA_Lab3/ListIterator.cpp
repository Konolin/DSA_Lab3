#include "ListIterator.h"
#include "IndexedList.h"
#include <exception>

ListIterator::ListIterator(const IndexedList &list) : list(list) {
    index = list.head;
}

void ListIterator::first() {
    index = list.head;
}

void ListIterator::next() {
    if (valid()) index = list.next[index];
    else throw std::exception();
}

bool ListIterator::valid() const {
    return index != -1 && index < list.capacity;
}

TElem ListIterator::getCurrent() const {
    return list.elements[index];
}