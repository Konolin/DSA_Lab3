#include "ListIterator.h"
#include "IndexedList.h"
#include <exception>


// worst = average = best = θ(n)
ListIterator::ListIterator(const IndexedList &list) : list(list) {
    index = list.head;
}


// worst = average = best = θ(1)
void ListIterator::first() {
    index = list.head;
}


// worst = average = best = θ(1)
void ListIterator::next() {
    if (valid()) index = list.next[index];
    else throw std::exception();
}


// worst = average = best = θ(1)
bool ListIterator::valid() const {
    return index != -1;
}


// worst = average = best = θ(1)
TElem ListIterator::getCurrent() const {
    if (valid()) return list.elements[index];
    throw std::exception();
}
