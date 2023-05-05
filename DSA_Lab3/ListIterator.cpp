#include <exception>

#include "IndexedList.h"
#include "ListIterator.h"

//primim o lista noua ca parametru si trebuie sa adaugam toate elementele din lista actuala in lista noua

/**Time Complexity: Best Case, Worst Case, Average Case: θ(n)*/
IndexedList::IndexedList() {
    capacity = 15;
    head = -1;
    firstEmpty = 0;

    elements = new TElem[capacity];
    next = new int[capacity];

    // set default positions for the next elements
    for (int index = 0; index < capacity; index++)
        next[index] = index + 1;
    next[capacity - 1] = -1;
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(n)*/
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

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)*/
bool IndexedList::isEmpty() const {
    return head == -1;
}

/**Time Complexity: Best Case: θ(1), when the desired element is at the first position
                    Worst Case: θ(n), when the desired element is at the last position
                    Average Case: θ(n)*/
TElem IndexedList::getElement(int pos) const {
    // check if "pos" is valid
    if (pos < 0) throw std::exception();
    if (pos >= size()) throw std::exception();

    // start at the head and search for "pos" until it's found or the end of the SLLA is reached
    int currentIndex = head;
    int currentPosition = 0;
    while (currentPosition != pos && currentIndex != -1){
        currentIndex = next[currentIndex];
        currentPosition++;
    }

    // throw exception if "pos" was not found
    if (currentPosition != pos) throw std::exception();

    return elements[currentIndex];
}

/**Time Complexity: Best Case: θ(1), when the desired element is at the first position
                    Worst Case: θ(n), when the desired element is at the last position
                    Average Case: θ(n)*/
TElem IndexedList::setElement(int pos, TElem e) {
    if (pos < 0 || pos >= size())
        throw std::exception();
    int currentIndex = head;
    int currentPosition = 0;
    TElem oldValue;
    while (currentPosition != pos && currentIndex != -1){
        currentIndex = next[currentIndex];
        currentPosition++;
    }
    if (currentPosition != pos)
        throw std::exception();
    else {
        oldValue = elements[currentIndex];
        elements[currentIndex] = e;
    }
    return oldValue;
}

/**Time Complexity: Best Case: θ(1), when it doesn't need to resize and the last occupied element is also the head
                    Worst Case: θ(n), when it needs to resize or when the last occupied element is not the head
                    Average Case: θ(n)*/
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

/**Time Complexity: Best Case: θ(1), when it doesn't need to resize and the element to be added is at position 0
                    Worst Case: θ(n), when it needs to resize or when it has to add the element to the second to last position
                    Average Case: θ(n)*/
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

    // add element on the first position (change head)
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

/**Time Complexity: Best Case: θ(1), when the desired element is at the first position
                    Worst Case: θ(n), when the desired element is at the last position
                    Average Case: θ(n)*/
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

    // throw exception if pos is not found
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
    if (size() <= capacity / 4)
        resize_down();

    return removedValue;
}

/**Time Complexity: Best Case: θ(1), when the desired element is at the first position
                    Worst Case: θ(n), when the desired element is at the last position
                    Average Case: θ(n)*/
int IndexedList::search(TElem e) const {
    int currentElement = head;
    int currentPosition = 0;

    while (currentElement != -1){
        if (elements[currentElement] == e){
            return currentPosition;
        }
        currentElement = next[currentElement];
        currentPosition++;
    }
    return -1;
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(n)*/
ListIterator IndexedList::iterator() const {
    return ListIterator(*this);        
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)*/
IndexedList::~IndexedList() {
    delete[] next;
    delete[] elements;
}

/**Time Complexity: Best Case, Worst Case, Average Case: θ(n)*/
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

/**Time Complexity: Best Case, Worst Case, Average Case: θ(n)*/
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

/**Time Complexity: Best Case, Worst Case, Average Case: θ(1)

descr: returns the head of the current list
pre: list e IndexedList
post: head e int
Pseudocode:
function get_head()
    return head
end-function
 */
int IndexedList::get_head() const{
    return head;
}


/**Time Complexity: Best Case, Worst Case, Average Case: θ(n)

descr: adds the elements of a list given as parameter at the end of the current list
pre: list1 e IndexedList
     list2 e IndexedList
post: list1 = list1 + list2

Pseudocode:
function add_elements(IndexedList list)
    TElem elem
    int currentIndex = @ get_head(list)
    while next[currentIndex] != -1
        currentIndex = next[currentIndex]
    end-while
    ListIterator it(list)
    while @ valid(it)
        elem = @ getCurrent(it)
        if head == -1
            int newPosition = firstEmpty
            elements[newPosition] = elem
            firstEmpty = next[newPosition]
            next[newPosition] = -1
            head = newPosition
        else
            if firstEmpty == -1
                @ resize_up();
            end-if
            int newPosition = firstEmpty
            elements[newPosition] = elem
            firstEmpty = next[newPosition]
            next[newPosition] = -1
            next[currentIndex] = newPosition
        end-if
        currentIndex = next[currentIndex]
        @ next(it)
    end-while
end-function
 */
void IndexedList::add_elements(const IndexedList& list) {
    TElem elem;
    int currentIndex = list.get_head();
    while (next[currentIndex] != -1){
        currentIndex = next[currentIndex];
    }
    ListIterator it(list);
    while(it.valid()){
        elem = it.getCurrent();
        if (head == -1) {
            // insert first element in the SLLA position
            int newPosition = firstEmpty;
            elements[newPosition] = elem;
            firstEmpty = next[newPosition];
            next[newPosition] = -1;
            head = newPosition;
        } else {
            // check if there is need for a resize
            if (firstEmpty == -1) resize_up();

            // insert the element
            int newPosition = firstEmpty;
            elements[newPosition] = elem;
            firstEmpty = next[newPosition];
            next[newPosition] = -1;
            // links the last element of the SLLA with the new element
            next[currentIndex] = newPosition;
        }
        currentIndex = next[currentIndex];
        it.next();
    }
}
