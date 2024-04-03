//--------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA4
// List.cpp 
// Implementation file for List ADT
//--------------------------------------------------------------------------

#include "List.h"

#define NIL -1


// Private Constructor -----------------------------------------------------

List::Node::Node(int x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}


// Class Constructors & Destructors ----------------------------------------
   
// Creates new List in the empty state.
List::List() {
    Node *F = new Node(NIL);
    Node *B = new Node(NIL);
    F->next = B;
    B->prev = F;
    
    frontDummy = F;
    backDummy = B;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

// Copy constructor.
List::List(const List& L) {
    // Make this an empty List.
    Node *F = new Node(NIL);
    Node *B = new Node(NIL);
    F->next = B;
    B->prev = F;
    
    frontDummy = F;
    backDummy = B;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;

    // Load elements of L into this.
    moveFront();
    Node *N = L.frontDummy->next;
    while (N->next != nullptr) {
        this->insertBefore(N->data);
        N = N->next;
    }
    
    pos_cursor = L.pos_cursor;
    num_elements = L.num_elements;
}

// Destructor
List::~List() {
    if (!isEmpty()) {
        this->clear();
    }

    delete frontDummy;
    delete backDummy;
}


// Access functions --------------------------------------------------------

// isEmpty()
// Returns true if this List is empty, false otherwise.
bool List::isEmpty() {
    return num_elements == 0;
}

// size()
// Returns the size of this List.
int List::size() {
    return num_elements;
}

// position()
// Returns the position of the cursor in this List. The value returned
// will be in the range 0 to size().
int List::position() {
    return pos_cursor;
}


// Manipulation procedures -------------------------------------------------

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront() {
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = beforeCursor->next;

    return;
}

// moveBack()
// Moves cursor to position size() in this List.
void List::moveBack() {
    pos_cursor = num_elements;
    afterCursor = backDummy;
    beforeCursor = afterCursor->prev;

    return;
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<size()
int List::peekNext() {
    if (pos_cursor >= num_elements) {
        std::cerr << "List Error: calling peekNext() with no Node after cursor." << std::endl;
        exit(EXIT_FAILURE);
    }

    return afterCursor->data;
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
int List::peekPrev() {
    if (pos_cursor <= 0) {
        std::cerr << "List Error: calling peekPrev() with no Node before cursor." << std::endl;
        exit(EXIT_FAILURE);
    }

    return beforeCursor->data;
} 

// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<size() 
int List::moveNext() {
    if (pos_cursor >= num_elements) {
        std::cerr << "List Error: calling moveNext() with no Node after cursor." << std::endl;
        exit(EXIT_FAILURE);
    }

    pos_cursor += 1;
    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;
    
    return this->peekPrev();
}

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
int List::movePrev() {
    if (pos_cursor <= 0) {
        std::cerr << "List Error: calling movePrev() with no Node before cursor." << std::endl;
        exit(EXIT_FAILURE);
    }

    pos_cursor -= 1;
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;

    return this->peekNext();    
}

// insertAfter()
// Inserts x after cursor.
void List::insertAfter(int x) {
    Node *N = new Node(x);

    beforeCursor->next = N;
    afterCursor->prev = N;

    N->prev = beforeCursor;
    N->next = afterCursor;
    
    afterCursor = N;
    num_elements += 1;

    return;
}

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(int x) {
    Node *N = new Node(x);

    beforeCursor->next = N;
    afterCursor->prev = N;

    N->prev = beforeCursor;
    N->next = afterCursor;
    
    beforeCursor = N;
    num_elements += 1;
    pos_cursor += 1;

    return;
}

// eraseAfter()
// Deletes element after cursor.
// pre: position()<size()
void List::eraseAfter() {
    if (pos_cursor >= num_elements) {
        std::cerr << "List Error: calling eraseAfter() with no Node after cursor." << std::endl;
        exit(EXIT_FAILURE);
    }

    beforeCursor->next = afterCursor->next;
    afterCursor->next->prev = beforeCursor;

    delete afterCursor;
    
    afterCursor = beforeCursor->next;
    num_elements -= 1;

    return;
}

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore() {
    if (pos_cursor <= 0) {
        std::cerr << "List Error: calling eraseBefore() with no Node before cursor." << std::endl;
        exit(EXIT_FAILURE);
    }

    afterCursor->prev = beforeCursor->prev;
    beforeCursor->prev->next = afterCursor;

    delete beforeCursor;
    
    beforeCursor = afterCursor->prev;
    num_elements -= 1;
    pos_cursor -= 1;

    return;
}

// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of the element x.
// If x is found, places the cursor immediately after the found element (so 
// eraseBefore() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position size(),
// and returns -1. 
int List::findNext(int x) {
    Node *N = afterCursor;

    while (N->next != nullptr) {
        if (N->data == x) {
            moveNext();
            return pos_cursor;
        }
        
        moveNext();
        N = N->next;
    }

    pos_cursor = num_elements;
    return -1;
}

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of the element x.
// If x is found, places the cursor immediately before the found element (so 
// eraseAfter() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position 0, and 
// returns -1. 
int List::findPrev(int x) {
    Node *N = beforeCursor;

    while (N->prev != nullptr) {
        if (N->data == x) {
            movePrev();
            return pos_cursor;
        }
        
        movePrev();
        N = N->prev;
    }

    pos_cursor = 0;
    return -1;
}

// cleanup()
// Removes any repeated elements in this List, leaving only unique data
// values. The order of the remaining elements is obtained by retaining
// the frontmost occurrance of each element, and removing all other 
// occurances. The cursor is not moved with respect to the retained 
// elements, i.e. it lies between the same two retained elements that it 
// did before cleanup() was called.
void List::cleanup() {
    Node *N = frontDummy;
    int seen[num_elements]; // Array to keep track of seen element's data.
    int index = 0; // Index to insert into seen[]. Num elements in seen[].
    int pos = 0; // Position of N in the linked list.
    bool repeat = false; // Boolean to indicate if a repeat element was found in a certain pass.

    for (N = N->next; N != nullptr; N = N->next) {
        for (int i = 0; i < index; i += 1) {
            if (N->data == seen[i]) {
                N->prev->next = N->next;
                N->next->prev = N->prev;

                if (pos < pos_cursor) { // Element deleted before position()
                    pos_cursor -= 1;
                    pos -= 1;
                }

                // Maintain afterCursor and beforeCursor if necessary.
                if (N == afterCursor) {
                    afterCursor = N->next;
                } else if (N == beforeCursor) {
                    beforeCursor = N->prev;
                }

                Node temp = Node(N->data);
                temp.next = N->next;
                delete N; // Free memory allocated for N.
                N = &temp;
                
                num_elements -= 1;
                repeat = true;
            } 
        }

        if (!repeat) {
            seen[index] = N->data;
            index += 1;
        }

        repeat = false;
        pos += 1;
    }

    return;
}

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
    moveFront();
    while (num_elements > 0) {
        eraseAfter();
    }

    return;
}

// concat()
// Returns a new List consisting of the elements of this List, followed
// the elements of L. The returned List's cursor will be at its front
// (position 0).
List List::concat(const List& L) {
    List M = List(*this);
    
    M.moveBack();
    for (Node *N = L.frontDummy->next; N->next != nullptr; N = N->next) {
        M.insertBefore(N->data);
    }
    
    M.moveFront();
    return M;
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() {
    Node *N = nullptr;
    std::string s = "(";

    for (N = frontDummy->next; N->next != nullptr; N=N->next) {
        if (N->next->next) {
            s += std::to_string(N->data) + ", ";
        } else {
            s += std::to_string(N->data);
        }
    }

    s += ")";

    return s;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) {
    Node *N = this->frontDummy;
    Node *M = R.frontDummy;

    if (this->num_elements != R.num_elements){
        return false;
    }

    while (N != nullptr) {
        if (N->data != M->data) {
            return false;
        }
        
        N = N->next;
        M = M->next;
    }

    return true;
}


// Overriden Operators -----------------------------------------------------
   
// operator<<()
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, List& L ) {
    return stream << L.List::to_string();
}

// operator==()
// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged are unchanged.
bool operator==( List& A, const List& B ) {
    return A.List::equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L ) {
    if (this != &L) { // Not self assignment
        // Make a copy of L.
        List temp = L;

        // Swap the copy's fields with the fields of this.
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }

    // Return this with the new data installed.
    return *this;

    // The copy, if there is one, is deleted upon return.
}
