#pragma once
#include <iostream>
#include <cmath>

#include "coor.h"

class Path {
private:
    coor* waypoints;
    int total; // total number of waypoints

public:
    Path() {
        waypoints = nullptr;
        total = 0;
    }

    void set(coor* arr, int size) {
        // deep copy
        delete[] waypoints;

        total = size;
        waypoints = new coor[size];

        for (int i = 0; i < size; i++) {
            waypoints[i] = arr[i];
        }
    }

    coor get(int i) {
        return waypoints[i];
    }

    int size() {
        return total;
    }

    ~Path() {
        delete[] waypoints;
    }
};