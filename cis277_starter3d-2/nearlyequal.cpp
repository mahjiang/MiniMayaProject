#include "nearlyequal.h"
#include <math.h>

bool NearlyEqual::nearlyEqual(float a, float b, float epsilon) {
    const float absA = fabs(a);
    const float absB = fabs(b);
    const float diff = fabs(a - b);
    // shortcut return true;
    if (a == b) {return true;}
    else if (a * b == 0) { // a or b or both are zero // relative error is not meaningful here
        return diff < (epsilon * epsilon);
    } else { // use relative error
        return diff / (absA + absB) < epsilon; }
}

