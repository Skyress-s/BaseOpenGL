#include "pch.h"
#include "Range.h"

#include <cmath>

float operator%(float lhs, const Range& rhs) { // TODO master implemation like using fmod()
    if (lhs > rhs.to) return (lhs -rhs.GetRange()) %(rhs);
    if (lhs < rhs.from) return (lhs +rhs.GetRange()) %(rhs);
    return lhs;
}
