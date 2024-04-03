//-----------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA5
// ListTest.cpp 
// Test client for List ADT
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include"List.h"

#define MAX_VALUE 10

using namespace std;

int main () {
    srand(time(0)); // Set a random seed based on time(0).
    
    List A, B, C, D, E;

    for (int i = 1; i < 10; i += 1) {
        A.insertAfter(rand() % MAX_VALUE + 1);
        B.insertBefore(rand() % MAX_VALUE + 1);
    }

    cout << "=============" << endl;
    cout << "Testing insertAfter():" << endl;
    cout << "A = " << A << endl;
    cout << "A.position() = " << A.position() << endl;
    cout << endl;
    cout << "Testing insertBefore():" << endl;
    cout << "B = " << B << endl;
    cout << "B.position() = " << B.position() << endl;
    cout << "=============" << endl;
    cout << endl;

    cout << "=============" << endl;
    cout << "Testing C = A:" << endl;
    C = A;
    cout << "C = " << C << endl;
    cout << "=============" << endl;
    cout << endl;

    cout << "=============" << endl;
    cout << "Testing D = A.concat(B):" << endl;
    D = A.concat(B);
    cout << "D = " << D << endl;
    cout << endl;
    E = A.concat(A);
    cout << "Testing E = A.concat(A):" << endl;
    cout << "E = " << E << endl;
    cout << "=============" << endl;
    cout << endl;

    cout << "=============" << endl;
    cout << "Testing D.cleanup():" << endl;
    D.cleanup();
    cout << "D = " << D << endl;
    E.cleanup();
    cout << "E = " << E << endl;
    cout << "=============" << endl;
    cout << endl;

    cout << "=============" << endl;
    int value = rand() % MAX_VALUE + 1;
    int value1 = rand() % MAX_VALUE + 1000;
    cout << "Testing findNext():" << endl;
    cout << "A = " << A << endl;
    A.moveFront();
    cout << "A.position() = " << A.position() << endl;
    cout << "Testing A.findNext(" << value << "): " << A.findNext(value) << endl;
    cout << "Testing A.findNext(" << value << "): " << A.findNext(value) << endl;
    cout << "Testing A.findNext(" << value1 << "): " << A.findNext(value1) << endl;
    cout << endl;
    cout << "Testing findPrev():" << endl;
    cout << "B = " << B << endl;
    B.moveBack();
    cout << "B.position() = " << B.position() << endl;
    cout << "Testing B.findNext(" << value << "): "  << B.findPrev(value) << endl;
    cout << "Testing B.findNext(" << value << "): "  << B.findPrev(value) << endl;
    cout << "Testing B.findNext(" << value1 << "): " << B.findPrev(value1) << endl;
    cout << "=============" << endl;
    cout << endl;

    cout << "=============" << endl;
    cout << "Testing operator==:" << endl;
    cout << "A == B: " << (A == B ? "true" : "false") << endl;
    cout << "A == C: " << (A == C ? "true" : "false") << endl;
    cout << "A == D: " << (A == D ? "true" : "false") << endl;
    cout << "A == E: " << (A == D ? "true" : "false") << endl;
    cout << "B == C: " << (B == C ? "true" : "false") << endl;
    cout << "=============" << endl;
    cout << endl;

    cout << "=============" << endl;
    cout << "Testing B.clear():" << endl;
    B.clear();
    cout << "B = " << B << endl;
    cout << "B is " << (B.isEmpty() ? "" : "not ") << "empty." << endl;
    cout << "=============" << endl;
    
    return(0);
}
