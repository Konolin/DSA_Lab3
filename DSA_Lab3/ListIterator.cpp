#include "ListIterator.h"
#include "IndexedList.h"
#include <exception>

/**Time Complexity: Best Case, Worst Case, Average Case: θ(n), because it uses the IndexedList constructor to instantiate the list, which results in the same time complexity as that constructor*/
ListIterator::ListIterator(const IndexedList& list) : list(list){
    index = list.head;
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)*/
void ListIterator::first(){
    index = list.head;
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)*/
void ListIterator::next(){
    if (valid()) index = list.next[index];
    else throw std::exception();
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)*/
bool ListIterator::valid() const{
    return index != -1;
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)*/
TElem ListIterator::getCurrent() const{
    if (!valid()) throw std::exception();
    return list.elements[index];
}
