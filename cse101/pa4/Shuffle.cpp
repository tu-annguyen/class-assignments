//-----------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA4
// Shuffle.cpp 
// Top-level client and main program for PA4
//-----------------------------------------------------------------------------

#include <ostream>
#include <iomanip>
#include "List.h"

// shuffle()
// Splits a List A in half and performs a perfect shuffle.
void shuffle(List &A) {
    int split = A.size() / 2;

    List D;
    // First iteration: B = (0, 1, 2, ..., split - 1)
    // First iteration: C = (split, split + 1, ..., n)
    List B, C;
    A.moveFront();
    for (int i = 0; i < split; i += 1) {
        B.insertBefore(A.moveNext());
    }
    while (A.position() < A.size()) {
        C.insertBefore(A.moveNext());
    }

    // First iteration: D = (0, split, 1, split + 1, ..., spliit - 1, n)
    D.clear();
    B.moveFront();
    C.moveFront();
    D.moveFront();
    while (B.position() < B.size()) {
        D.insertBefore(C.moveNext());
        D.insertBefore(B.moveNext());
    }
    if (C.size() > B.size()) {
        D.insertBefore(C.peekNext());
    }
    A = D;
}

// shuffles()
// Returns the number of shuffle() calls it takes to return a deck of size
// n back to its original order.
int shuffles(int n) {
    int count = 0;

    // A = (0, 1, 2, ..., n)
    List A;
    for (int i = 0; i < n; i += 1) {
        A.insertBefore(i);
    }

    List B = A;

    do {
        shuffle(A);
        count += 1;
    } while (!A.equals(B));

    return count;
}

int main(int argc, char * argv[]) {
    int size = 0; // Deck size.

    // Set size from command-line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <deck-size>" << std::endl;
        return 1;
    }

    size = std::stoi(argv[1]);
    if (size < 0) {
        std::cerr << "Deck size must be greater than 0." << std::endl;
        return 1;
    }
    
    std::cout << "deck size" << std::setw(20) << "shuffle count" << std::endl;
    std::cout << "------------------------------" << std::endl;
    for (int i = 1; i <= size; i += 1) {
        std::cout << " " << i << std::setw(16) << shuffles(i) << std::endl;
    }
    
    return 0;
}