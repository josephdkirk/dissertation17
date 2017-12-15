#ifndef BIGINT_H
#define BIGINT_H

#include <vector>

typedef struct{
    std::vector<bool> digits;
    bool positive;
} bigint;


#endif /* BIGINT_H */

