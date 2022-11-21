#ifndef PRACTICAL3_EXTERNAL_H
#define PRACTICAL3_EXTERNAL_H

#endif //PRACTICAL3_EXTERNAL_H

//#define DEBUG

#ifdef DEBUG

long fun(long a, long b) {
    return a + b;
}

#else

extern long fun(long a, long b);

#endif