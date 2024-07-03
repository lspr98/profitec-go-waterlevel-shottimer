#ifndef _SERIALUTILS_H
#define _SERIALUTILS_H

#include <Arduino.h>

/*

    Template wrapper for Serial.print to allow printing to serial in std::cout like fashion.

    Note: Also overloads the "<=" operator to automatically append a \n to the last output, so the following two commands result in the same output:
    sout << "Hello World" << "\n"
    sout <= "Hello World"

*/

class Serout {

public:

    template <typename T>
    friend Serout& operator<<(Serout& so, T s) {
        Serial.print(s);
        return so;
    };
    template <typename T>
    friend Serout& operator<=(Serout& so, T s) {
        Serial.println(s);
        return so;
    };

    Serout& warn();

    Serout& err();

    Serout& info();

};

extern Serout sout;

#endif