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
    return index != -1;
}

TElem ListIterator::getCurrent() const {
    if (!valid()) throw std::exception();
    return list.elements[index];
}
