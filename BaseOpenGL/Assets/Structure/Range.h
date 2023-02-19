#pragma once

struct Range {
public:
    float from;
    float to;

    bool InRange(const float& t) {
        if (t >= from && t <= to)
            return true;
        return false;
    }

    float GetRange() const {
        return to - from;   
    }
private:
    friend float operator%(float lhs, const Range& rhs);
};

