#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Path.h"

class Map {
private:
    Path* paths;
    int count;
    int mapID;

    void buildMap0() {
        count = 3;
        paths = new Path[count];
        coor p0[] = { coor(80,0), coor(80,200), coor(240,200), coor(240,400), coor(80,400), coor(80,600) };
        paths[0].set(p0, 6);
        coor p1[] = { coor(390,0), coor(390,200), coor(550,200), coor(550,400), coor(390,400), coor(390,600) };
        paths[1].set(p1, 6);
        coor p2[] = { coor(700,0), coor(700,200), coor(560,200), coor(560,400), coor(700,400), coor(700,600) };
        paths[2].set(p2, 6);
    }
    void buildMap1() {
        count = 3;
        paths = new Path[count];
        coor p0[] = { coor(60,0), coor(60,180), coor(260,180), coor(260,430), coor(60,430), coor(60,600) };
        paths[0].set(p0, 6);
        coor p1[] = { coor(340,0), coor(340,180), coor(540,180), coor(540,430), coor(340,430), coor(340,600) };
        paths[1].set(p1, 6);
        coor p2[] = { coor(650,0), coor(650,230), coor(770,230), coor(770,380), coor(650,380), coor(650,600) };
        paths[2].set(p2, 6);
    }
    void buildMap2() {
        count = 3;
        paths = new Path[count];
        coor p0[] = { coor(80,0), coor(80,480), coor(220,480), coor(220,600) };
        paths[0].set(p0, 4);
        coor p1[] = { coor(360,0), coor(360,480), coor(500,480), coor(500,600) };
        paths[1].set(p1, 4);
        coor p2[] = { coor(640,0), coor(640,480), coor(760,480), coor(760,600) };
        paths[2].set(p2, 4);
    }

    // ── Internal: draw checkered grass into a region ──────
    static void drawCheckered(sf::RenderWindow& window,
        float ox, float oy,
        float w, float h,
        int tileSize = 40) {
        sf::Color c1(34, 100, 34);
        sf::Color c2(28, 85, 28);
        int cols = (int)(w / tileSize) + 1;
        int rows = (int)(h / tileSize) + 1;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                sf::RectangleShape tile(sf::Vector2f((float)tileSize, (float)tileSize));
                tile.setPosition(ox + c * tileSize, oy + r * tileSize);
                tile.setFillColor((r + c) % 2 == 0 ? c1 : c2);
                window.draw(tile);
            }
        }
    }

    // ── Internal: draw roads for a given path array ────────
    static void drawRoads(sf::RenderWindow& window,
        Path* paths, int count,
        float ox, float oy,
        float scaleX, float scaleY,
        float roadWidth) {
        const sf::Color roadColor(180, 140, 75);
        const sf::Color borderColor(140, 105, 50);

        for (int i = 0; i < count; i++) {
            int pts = paths[i].size();
            for (int j = 0; j < pts - 1; j++) {
                coor a = paths[i].get(j);
                coor b = paths[i].get(j + 1);

                float ax = ox + a.getxaxis() * scaleX;
                float ay = oy + a.getyaxis() * scaleY;
                float bx = ox + b.getxaxis() * scaleX;
                float by = oy + b.getyaxis() * scaleY;

                float dx = bx - ax;
                float dy = by - ay;
                float length = sqrt(dx * dx + dy * dy);
                float angle = atan2(dy, dx) * 180.f / 3.14159265f;

                // Border
                sf::RectangleShape border(sf::Vector2f(length, roadWidth + 5.f));
                border.setOrigin(0.f, (roadWidth + 5.f) / 2.f);
                border.setPosition(ax, ay);
                border.setRotation(angle);
                border.setFillColor(borderColor);
                window.draw(border);

                // Surface
                sf::RectangleShape road(sf::Vector2f(length, roadWidth));
                road.setOrigin(0.f, roadWidth / 2.f);
                road.setPosition(ax, ay);
                road.setRotation(angle);
                road.setFillColor(roadColor);
                window.draw(road);
            }
            // Round joints
            for (int j = 0; j < pts; j++) {
                coor c = paths[i].get(j);
                float cx = ox + c.getxaxis() * scaleX;
                float cy = oy + c.getyaxis() * scaleY;
                sf::CircleShape joint(roadWidth / 2.f + 2.f);
                joint.setOrigin(roadWidth / 2.f + 2.f, roadWidth / 2.f + 2.f);
                joint.setPosition(cx, cy);
                joint.setFillColor(roadColor);
                window.draw(joint);
            }
        }
    }

public:
    Map() : paths(nullptr), count(0), mapID(0) { buildMap0(); }

    void setMap(int id) {
        mapID = id;
        delete[] paths;
        paths = nullptr;
        if (id == 0) buildMap0();
        else if (id == 1) buildMap1();
        else              buildMap2();
    }

    int          getMapID()   const { return mapID; }
    Path& getPath(int i) { return paths[i]; }
    int          getCount()   const { return count; }

    const char* getMapName() const {
        if (mapID == 0) return "Triple S";
        if (mapID == 1) return "Triple Z";
        return "Triple U";
    }

    // ── MAIN GAME RENDER ──────────────────────────────────
    void render(sf::RenderWindow& window) {
        // Checkered grass (full screen, below HUD)
        drawCheckered(window, 0, 40, 800, 560, 40);
        // Roads at full scale
        drawRoads(window, paths, count, 0, 0, 1.f, 1.f, 38.f);
    }

    // ── MINI-PREVIEW RENDER (for map select screen) ───────
    // Draws a scaled-down preview inside a rectangle at (px,py) size (pw x ph)
    void renderPreview(sf::RenderWindow& window,
        float px, float py, float pw, float ph,
        int previewMapID) {

        // Build temp paths for preview map
        Path* prevPaths = nullptr;
        int   prevCount = 0;

        if (previewMapID == 0) {
            prevCount = 3;
            prevPaths = new Path[3];
            coor p0[] = { coor(80,0),coor(80,200),coor(240,200),coor(240,400),coor(80,400),coor(80,600) };
            prevPaths[0].set(p0, 6);
            coor p1[] = { coor(390,0),coor(390,200),coor(550,200),coor(550,400),coor(390,400),coor(390,600) };
            prevPaths[1].set(p1, 6);
            coor p2[] = { coor(700,0),coor(700,200),coor(560,200),coor(560,400),coor(700,400),coor(700,600) };
            prevPaths[2].set(p2, 6);
        }
        else if (previewMapID == 1) {
            prevCount = 3;
            prevPaths = new Path[3];
            coor p0[] = { coor(60,0),coor(60,180),coor(260,180),coor(260,430),coor(60,430),coor(60,600) };
            prevPaths[0].set(p0, 6);
            coor p1[] = { coor(340,0),coor(340,180),coor(540,180),coor(540,430),coor(340,430),coor(340,600) };
            prevPaths[1].set(p1, 6);
            coor p2[] = { coor(650,0),coor(650,230),coor(770,230),coor(770,380),coor(650,380),coor(650,600) };
            prevPaths[2].set(p2, 6);
        }
        else {
            prevCount = 3;
            prevPaths = new Path[3];
            coor p0[] = { coor(80,0),coor(80,480),coor(220,480),coor(220,600) };
            prevPaths[0].set(p0, 4);
            coor p1[] = { coor(360,0),coor(360,480),coor(500,480),coor(500,600) };
            prevPaths[1].set(p1, 4);
            coor p2[] = { coor(640,0),coor(640,480),coor(760,480),coor(760,600) };
            prevPaths[2].set(p2, 4);
        }

        // Clip background
        drawCheckered(window, px, py, pw, ph, 20);

        // Scale roads to fit preview box (original map = 800x600)
        float sx = pw / 800.f;
        float sy = ph / 600.f;
        drawRoads(window, prevPaths, prevCount, px, py, sx, sy, 10.f);

        delete[] prevPaths;
    }

    ~Map() { delete[] paths; }
};