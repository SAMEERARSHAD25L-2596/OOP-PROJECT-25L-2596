#pragma once

class coor {
private:
    int xaxis;
    int yaxis;

public:
    coor() : xaxis(0), yaxis(0) {}

    coor(int xa, int ya) : xaxis(xa), yaxis(ya) {}

    int getxaxis() const {
        return xaxis;
    }

    int getyaxis() const {
        return yaxis;
    }

    void setx(int h) {
        xaxis = h;
    }

    void sety(int h) {
        yaxis = h;
    }

    void setxy(int h, int l) {
        xaxis = h;
        yaxis = l;
    }
};