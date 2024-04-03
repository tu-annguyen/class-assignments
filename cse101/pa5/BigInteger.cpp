//---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA5
// BigInteger.cpp
// Implementation file for the BigInteger ADT
//---------------------------------------------------------------------------

#include <cmath>
#include "BigInteger.h"

// Global variables --------------------------------------------------------

long base = 100;
int power = 2;

// Class Constructors & Destructors ----------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}

// isNumber()
// Helper function for BigInteger(). Determines if string s is a number.
// pre: s is not empty.
bool isNumber(std::string s) {
    if (s.length() <= 0) {
        std::cerr << "BigInteger Error: isNumber() called on empty string" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (s[0] == '+' || s[0] == '-') {
        for (char &c : s.substr(1)) {
            if (std::isdigit(c) == 0) {
                return false;
            }
        }

    } else {
        for (char &c : s) {
            if (std::isdigit(c) == 0) {
                return false;
            }
        }
    }

    return true;
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s) {
    if (s.length() <= 0) {
        std::cerr << "BigInteger Error: constructor called on empty string" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!isNumber(s)) {
        std::cerr << "BigInteger Error: constructor called on non-numeric string" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Sets signum.
    if (s[0] == '-') {
        signum = -1;
        s = s.substr(1);
    } else if (s[0] == '+') {
        signum = 1;
        s = s.substr(1);
    } else {
        signum = 1;
    }

    // 
    for (int i = s.length() - 1; i >= 0; i -= power) {
        if (i < power) {
            digits.insertAfter(std::stol(s.substr(0, i + 1)));
        } else {
            digits.insertAfter(std::stol(s.substr(i - power + 1, power)));
        }
    }
}

// BigInteger()
// Constructor that copies the BigInteger N.
BigInteger::BigInteger(const BigInteger& N) {
    // Make this an empty BigInteger.
    signum = 0;
    digits = List();

    // Load elements of N into this.
    signum = N.signum;
    digits = N.digits;
}

// ~BigInteger()
// Destructor (optional)
// ~BigInteger();


// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign() {
    return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(BigInteger N) {
    // Check the signs.
    if (sign() < N.sign()) {
        return -1;
    } else if (sign() > N.sign()) {
        return 1;
    }

    // Check the size of the digits Lists
    if (digits.size() < N.digits.size()) {
        return -1;
    } else if (digits.size() > N.digits.size()) {
        return 1;
    }

    // this and N have the same sign and number of elements in digits List.
    digits.moveFront();
    N.digits.moveFront();

    while (digits.position() < digits.size()) {
        if (digits.peekNext() < N.digits.peekNext()) {
            return -1;
        } else if (digits.peekNext() > N.digits.peekNext()) {
            return 1;
        }

        digits.moveNext();
        N.digits.moveNext();
    }
    
    return 0;
}


// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero() {
    signum = 0;
    digits.clear();
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate() {
    if (signum == -1) {
        signum = 1; 
    } else if (signum == 1) {
        signum = -1;
    }
}


// BigInteger Arithmetic operations ----------------------------------------

// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L) {
    L.moveFront();
    while (L.position() < L.size()) {
        LIST_ELEMENT n = -1 * L.peekNext();
        L.eraseAfter();
        L.insertAfter(n);
        L.moveNext();
    }
}

// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn) {
    S.clear();
    A.moveBack();
    B.moveBack();
    S.moveFront();

    while (A.position() > 0 && B.position() > 0) {
        S.insertAfter(A.peekPrev() + sgn * B.peekPrev());
        A.movePrev();
        B.movePrev();
    }

    // Flush remaining elements of A into S.
    while (A.position() > 0) {
        S.insertAfter(A.peekPrev());
        A.movePrev();
    }

    // Flush remaining elements of B into S.
    while (B.position() > 0) {
        S.insertAfter(B.peekPrev());
        B.movePrev();
    }
}

// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List& L) {
    L.moveBack();
    L.movePrev();

    while (L.position() > 0) {
        if (L.peekNext() >= pow(10, power)) {
            LIST_ELEMENT n = L.peekNext() - 100;
            L.eraseAfter();
            L.insertAfter(n);

            LIST_ELEMENT m = L.peekPrev() + 1;
            L.eraseBefore();
            L.insertBefore(m);
        }

        if (L.peekNext() < 0) {
            LIST_ELEMENT n = L.peekNext() + 100;
            L.eraseAfter();
            L.insertAfter(n);

            LIST_ELEMENT m = L.peekPrev() - 1;
            L.eraseBefore();
            L.insertBefore(m);
        }

        L.movePrev();
    }

    if (L.peekNext() < 0) {
        LIST_ELEMENT n = -1 * L.peekNext();
        L.eraseAfter();
        L.insertAfter(n);

        return -1;
    }

    return 1;
}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p);

// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, LIST_ELEMENT m);

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) {
    BigInteger A = BigInteger(*this);
    BigInteger B = BigInteger(N);
    BigInteger C;

    // D = The absolute value of A.
    BigInteger D = BigInteger(A);
    D.signum = 1;

    // E = The absolute value of B.
    BigInteger E = BigInteger(B);
    E.signum = 1;

    // Check if B is negative.
    if (A.signum == 1 && B.signum == -1) {
        if (A.compare(E) == -1) {
            BigInteger F = E.sub(A);
            F.negate();
            return F;
        } 

        return A.sub(E);
    }

    // Check if A is negative.
    if (A.signum == -1 && B.signum == 1) {
        if (B.compare(D) == -1) {
            BigInteger F = D.sub(B);
            F.negate();
            return F;
        }

        return B.sub(D);
    }
    
    A.digits.moveFront();
    B.digits.moveFront();

    List S = List();

    sumList(S, A.digits, B.digits, 1);
    C.signum = normalizeList(S);
    C.digits = S;

    // Check if A and B are negative.
    if (A.signum == -1 && B.signum == -1) {
        C.signum = -1;
    }

    return C;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) {
    BigInteger A = BigInteger(*this);
    BigInteger B = BigInteger(N);
    BigInteger C;

    // D = The absolute value of A.
    BigInteger D = BigInteger(A);
    D.signum = 1;

    // E = The absolute value of B.
    BigInteger E = BigInteger(B);
    E.signum = 1;

    // Check if A and B is negative.
    if (A.sign() == -1 && B.sign() == 1) {
        C = D.add(E);
        C.signum = -1;
        return C;
    }
   
    // Check if B is negative.
    if (B.signum == -1) {
        return A.add(E);
    }

    A.digits.moveFront();
    B.digits.moveFront();

    List S = List();

    sumList(S, A.digits, B.digits, -1);
    C.signum = normalizeList(S);
    C.digits = S;

    // Check if digits is 0.
    C.digits.moveFront();
    while (C.digits.position() < C.digits.size() && C.digits.peekNext() == 0) {
        C.digits.eraseAfter();
    }

    if (C.digits.size() == 0) {
        C.makeZero();
    }

    return C;
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) {
    BigInteger A;
    return A;
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string() {
    std::string s;

    // this BigInteger is 0.
    if (digits.isEmpty()) {
        s += '0';
        return s;
    }

    // this BigInteger is negative.
    if (sign() == -1) {
        s += '-';
    }

    digits.moveFront();
    while (digits.position() < digits.size()) {
        // Pad number w/ 0's if there are not enough digits.
        std::string numstr = std::to_string(digits.peekNext());
        while (int(numstr.length()) != power && digits.position() != 0) {
            numstr = '0' + numstr;
        } 

        s += numstr; 
        digits.moveNext();
    }

    return s;
}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ) {
    return stream << N.BigInteger::to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( BigInteger& A, const BigInteger& B ) {
    return A.compare(B) == 0;
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( BigInteger& A, const BigInteger& B ) {
    return A.compare(B) == -1;
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( BigInteger& A, const BigInteger& B ) {
    return A.compare(B) == -1 || A.compare(B) == 0;
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( BigInteger& A, const BigInteger& B ) {
    return A.compare(B) == 1;
}

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( BigInteger& A, const BigInteger& B ) {
    return A.compare(B) == 1 || A.compare(B) == 0;
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( BigInteger A, const BigInteger& B ) {
    return A.add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ) {
    A = A.add(B);
    return A;
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( BigInteger A, const BigInteger& B ) {
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ) {
    A = A.sub(B);
    return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( BigInteger A, const BigInteger& B ) {
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ) {
    A = A.mult(B);
    return A;
}