#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <string>

#include "Map.h"
#include "wave.h"
#include "Player.h"
#include "Tower.h"
#include "Enemy.h"

#include "Basicenemy.h"
#include "fastenemy.h"
#include "Tankenemy.h"
#include "peka.h"

#include "Canontower.h"
#include "Machinegun.h"
#include "Wizardtower.h"
#include "archertower.h"

static const sf::Color C_GRASS_A(74, 120, 42, 255);
static const sf::Color C_GRASS_B(60, 98, 34, 255);
static const sf::Color C_PATH_BDR(130, 88, 36, 255);
static const sf::Color C_PATH(190, 145, 80, 255);
static const sf::Color C_HUD(18, 20, 28, 255);
static const sf::Color C_GOLD(255, 215, 0, 255);
static const sf::Color C_RED(220, 55, 55, 255);

static sf::Text makeText(sf::Font& f, const std::string& s, unsigned sz) {
    sf::Text t(s, f, sz);
    return t;
}

static void drawGrass(sf::RenderWindow& win) {
    const int T = 40;
    sf::RectangleShape tile(sf::Vector2f((float)T, (float)T));
    for (int y = 40; y < 600; y += T)
        for (int x = 0; x < 800; x += T) {
            tile.setPosition((float)x, (float)y);
            tile.setFillColor(((x / T + y / T) % 2 == 0) ? C_GRASS_A : C_GRASS_B);
            win.draw(tile);
        }
}

static void drawPath(sf::RenderWindow& win, Path& p, int pathW) {
    int half = pathW / 2, borderExtra = 4, bHalf = half + borderExtra;
    for (int j = 0; j + 1 < p.size(); j++) {
        coor a = p.get(j), b = p.get(j + 1);
        int ax = a.getxaxis(), ay = a.getyaxis(), bx = b.getxaxis(), by = b.getyaxis();
        sf::RectangleShape seg;
        if (ay == by) {
            int x0 = (ax < bx ? ax : bx) - borderExtra;
            seg.setSize(sf::Vector2f((float)(abs(bx - ax) + borderExtra * 2), (float)(bHalf * 2)));
            seg.setPosition((float)x0, (float)(ay - bHalf));
        }
        else {
            int y0 = (ay < by ? ay : by) - borderExtra;
            seg.setSize(sf::Vector2f((float)(bHalf * 2), (float)(abs(by - ay) + borderExtra * 2)));
            seg.setPosition((float)(ax - bHalf), (float)y0);
        }
        seg.setFillColor(C_PATH_BDR); win.draw(seg);
    }
    for (int j = 0; j + 1 < p.size(); j++) {
        coor a = p.get(j), b = p.get(j + 1);
        int ax = a.getxaxis(), ay = a.getyaxis(), bx = b.getxaxis(), by = b.getyaxis();
        sf::RectangleShape seg;
        if (ay == by) {
            seg.setSize(sf::Vector2f((float)abs(bx - ax), (float)(half * 2)));
            seg.setPosition((float)(ax < bx ? ax : bx), (float)(ay - half));
        }
        else {
            seg.setSize(sf::Vector2f((float)(half * 2), (float)abs(by - ay)));
            seg.setPosition((float)(ax - half), (float)(ay < by ? ay : by));
        }
        seg.setFillColor(C_PATH); win.draw(seg);
    }
    for (int j = 0; j < p.size(); j++) {
        coor c = p.get(j);
        float cx = (float)c.getxaxis(), cy = (float)c.getyaxis();
        sf::CircleShape outer((float)bHalf);
        outer.setOrigin((float)bHalf, (float)bHalf);
        outer.setPosition(cx, cy); outer.setFillColor(C_PATH_BDR); win.draw(outer);
        sf::CircleShape inner((float)half);
        inner.setOrigin((float)half, (float)half);
        inner.setPosition(cx, cy); inner.setFillColor(C_PATH); win.draw(inner);
    }
}

static void drawMap(sf::RenderWindow& win, Map& map) {
    drawGrass(win);
    for (int i = 0; i < map.getCount(); i++)
        drawPath(win, map.getPath(i), 36);
}

static void drawMiniPath(sf::RenderWindow& win, Path& pp,
    int px, int py, int pw, int ph, int pathW) {
    float sx = (float)pw / 800.f, sy = (float)ph / 600.f;
    for (int j = 0; j + 1 < pp.size(); j++) {
        coor a = pp.get(j), b = pp.get(j + 1);
        int ax = px + (int)(a.getxaxis() * sx), ay = py + (int)(a.getyaxis() * sy);
        int bx = px + (int)(b.getxaxis() * sx), by = py + (int)(b.getyaxis() * sy);
        sf::RectangleShape seg;
        if (a.getyaxis() == b.getyaxis()) {
            seg.setSize(sf::Vector2f((float)(abs(bx - ax) + 2), (float)(pathW + 2)));
            seg.setPosition((float)(ax < bx ? ax : bx) - 1, (float)(ay - pathW / 2 - 1));
        }
        else {
            seg.setSize(sf::Vector2f((float)(pathW + 2), (float)(abs(by - ay) + 2)));
            seg.setPosition((float)(ax - pathW / 2 - 1), (float)(ay < by ? ay : by) - 1);
        }
        seg.setFillColor(C_PATH_BDR); win.draw(seg);
    }
    for (int j = 0; j + 1 < pp.size(); j++) {
        coor a = pp.get(j), b = pp.get(j + 1);
        int ax = px + (int)(a.getxaxis() * sx), ay = py + (int)(a.getyaxis() * sy);
        int bx = px + (int)(b.getxaxis() * sx), by = py + (int)(b.getyaxis() * sy);
        sf::RectangleShape seg;
        if (a.getyaxis() == b.getyaxis()) {
            seg.setSize(sf::Vector2f((float)abs(bx - ax), (float)pathW));
            seg.setPosition((float)(ax < bx ? ax : bx), (float)(ay - pathW / 2));
        }
        else {
            seg.setSize(sf::Vector2f((float)pathW, (float)abs(by - ay)));
            seg.setPosition((float)(ax - pathW / 2), (float)(ay < by ? ay : by));
        }
        seg.setFillColor(C_PATH); win.draw(seg);
    }
    for (int j = 0; j < pp.size(); j++) {
        coor c = pp.get(j);
        float jx = (float)(px + (int)(c.getxaxis() * sx));
        float jy = (float)(py + (int)(c.getyaxis() * sy));
        float r = pathW / 2.f;
        sf::CircleShape outer(r + 1.f); outer.setOrigin(r + 1.f, r + 1.f);
        outer.setPosition(jx, jy); outer.setFillColor(C_PATH_BDR); win.draw(outer);
        sf::CircleShape inner(r); inner.setOrigin(r, r);
        inner.setPosition(jx, jy); inner.setFillColor(C_PATH); win.draw(inner);
    }
}

static void drawEnemies(sf::RenderWindow& win, enemy** e, int total) {
    for (int i = 0; i < total; i++)
        if (e[i] && e[i]->isalive())
            e[i]->render(win);
}

static void drawTowers(sf::RenderWindow& win, tower** t, int count,
    sf::Vector2i mouse) {
    for (int i = 0; i < count; i++) {
        if (!t[i] || !t[i]->isalive()) continue;
        float tx = t[i]->getx(), ty = t[i]->gety();
        float dx = tx - mouse.x, dy = ty - mouse.y;
        if (std::sqrt(dx * dx + dy * dy) <= 30.f) {
            float r = (float)t[i]->getrange();
            sf::CircleShape ring(r);
            ring.setOrigin(r, r); ring.setPosition(tx, ty);
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineThickness(1.f);
            ring.setOutlineColor(sf::Color(255, 255, 255, 70));
            win.draw(ring);
        }
        t[i]->render(win);
        for (int k = 0; k < t[i]->getlevel() - 1 && k < 3; k++) {
            sf::RectangleShape pip(sf::Vector2f(6.f, 5.f));
            pip.setPosition(tx - 10.f + k * 8.f, ty + 8.f);
            pip.setFillColor(C_GOLD);
            win.draw(pip);
        }
    }
}

static void drawHUD(sf::RenderWindow& win, sf::Font& font,
    int gold, int lives, int wave, int sel,
    const char* mapName, tower** towers, int towerCount,
    sf::Vector2i mouse) {
    sf::RectangleShape hud(sf::Vector2f(800.f, 40.f));
    hud.setFillColor(C_HUD); win.draw(hud);

    sf::Text g = makeText(font, "Gold: " + std::to_string(gold), 18);
    g.setFillColor(C_GOLD); g.setPosition(8.f, 9.f); win.draw(g);

    sf::Text l = makeText(font, "Lives: " + std::to_string(lives), 18);
    l.setFillColor(C_RED); l.setPosition(155.f, 9.f); win.draw(l);

    sf::Text w = makeText(font, "Wave: " + std::to_string(wave) + "/5", 18);
    w.setFillColor(sf::Color(200, 200, 255)); w.setPosition(288.f, 9.f); win.draw(w);

    const char* tnames[] = { "Canon","MachineGun","Wizard","Archer" };
    sf::Text tt = makeText(font, "[" + std::to_string(sel) + "] " + tnames[sel - 1], 16);
    tt.setFillColor(sf::Color(150, 230, 150)); tt.setPosition(430.f, 11.f); win.draw(tt);

    sf::Text hint = makeText(font, "1-4:Tower  U:Upgrade  Click:Place", 11);
    hint.setFillColor(sf::Color(90, 100, 120)); hint.setPosition(570.f, 14.f); win.draw(hint);

    for (int i = 0; i < towerCount; i++) {
        if (!towers[i] || !towers[i]->isalive()) continue;
        float dx = towers[i]->getx() - mouse.x, dy = towers[i]->gety() - mouse.y;
        if (std::sqrt(dx * dx + dy * dy) <= 30.f) {
            sf::RectangleShape tip(sf::Vector2f(155.f, 22.f));
            float tipX = std::min((float)mouse.x + 10.f, 635.f);
            float tipY = std::max((float)mouse.y - 32.f, 42.f);
            tip.setPosition(tipX, tipY);
            tip.setFillColor(sf::Color(20, 20, 30, 220));
            tip.setOutlineThickness(1.f);
            tip.setOutlineColor(sf::Color(80, 80, 100));
            win.draw(tip);
            sf::Text tipT = makeText(font, "Lv" + std::to_string(towers[i]->getlevel()) + "  Press U=Upgrade", 11);
            tipT.setFillColor(sf::Color(200, 200, 100));
            tipT.setPosition(tipX + 4.f, tipY + 4.f);
            win.draw(tipT);
            break;
        }
    }

    sf::Text mn = makeText(font, mapName, 12);
    mn.setFillColor(sf::Color(160, 160, 100, 180));
    mn.setPosition(6.f, 584.f); win.draw(mn);
}

static bool runStartScreen(sf::RenderWindow& window, sf::Font& font) {
    srand(42);
    const int STARS = 80;
    float starX[80], starY[80], starSpd[80];
    for (int i = 0; i < STARS; i++) {
        starX[i] = (float)(rand() % 800);
        starY[i] = (float)(rand() % 600);
        starSpd[i] = 0.2f + (rand() % 10) * 0.05f;
    }
    sf::Clock anim;
    while (window.isOpen()) {
        float t = anim.getElapsedTime().asSeconds();
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); return false; }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                window.close(); return false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                int mx = event.mouseButton.x, my = event.mouseButton.y;
                if (mx >= 300 && mx <= 500 && my >= 340 && my <= 390) return true;
                if (mx >= 300 && mx <= 500 && my >= 410 && my <= 460) { window.close(); return false; }
            }
        }
        window.clear(sf::Color(8, 10, 20));
        sf::CircleShape star(2.f);
        star.setFillColor(sf::Color(255, 255, 255, 160));
        for (int i = 0; i < STARS; i++) {
            starY[i] += starSpd[i];
            if (starY[i] > 600) starY[i] = 0;
            star.setPosition(starX[i], starY[i]);
            window.draw(star);
        }
        int pulse = (int)(128 + 127 * std::sin(t * 2.f));
        sf::Text shadow = makeText(font, "TOWER DEFENSE", 56);
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
        shadow.setPosition(203.f, 103.f); window.draw(shadow);
        sf::Text title = makeText(font, "TOWER DEFENSE", 56);
        title.setFillColor(sf::Color(255, (sf::Uint8)pulse, 50));
        title.setPosition(200.f, 100.f); window.draw(title);
        sf::Text sub = makeText(font, "Defend your base. Survive all 5 waves.", 18);
        sub.setFillColor(sf::Color(160, 160, 200));
        sub.setPosition(200.f, 175.f); window.draw(sub);
        sf::RectangleShape div(sf::Vector2f(400.f, 2.f));
        div.setPosition(200.f, 210.f);
        div.setFillColor(sf::Color(60, 60, 100)); window.draw(div);
        const char* ctrls[] = { "1 - 4      Select tower type",
                                 "Click      Place tower on map",
                                 "U          Upgrade hovered tower",
                                 "ESC        Quit game" };
        for (int i = 0; i < 4; i++) {
            sf::Text c = makeText(font, ctrls[i], 14);
            c.setFillColor(sf::Color(130, 180, 130));
            c.setPosition(255.f, 230.f + i * 24.f);
            window.draw(c);
        }
        bool hovP = (mouse.x >= 300 && mouse.x <= 500 && mouse.y >= 340 && mouse.y <= 390);
        sf::RectangleShape pBtn(sf::Vector2f(200.f, 50.f));
        pBtn.setPosition(300.f, 340.f);
        pBtn.setFillColor(hovP ? sf::Color(60, 180, 60) : sf::Color(40, 130, 40));
        pBtn.setOutlineThickness(2.f);
        pBtn.setOutlineColor(sf::Color(100, 220, 100)); window.draw(pBtn);
        sf::Text pTxt = makeText(font, "PLAY", 22);
        pTxt.setFillColor(sf::Color::White);
        sf::FloatRect pb = pTxt.getLocalBounds();
        pTxt.setOrigin(pb.width / 2.f, pb.height / 2.f);
        pTxt.setPosition(400.f, 360.f); window.draw(pTxt);
        bool hovQ = (mouse.x >= 300 && mouse.x <= 500 && mouse.y >= 410 && mouse.y <= 460);
        sf::RectangleShape qBtn(sf::Vector2f(200.f, 50.f));
        qBtn.setPosition(300.f, 410.f);
        qBtn.setFillColor(hovQ ? sf::Color(180, 50, 50) : sf::Color(130, 35, 35));
        qBtn.setOutlineThickness(2.f);
        qBtn.setOutlineColor(sf::Color(220, 80, 80)); window.draw(qBtn);
        sf::Text qTxt = makeText(font, "QUIT", 22);
        qTxt.setFillColor(sf::Color::White);
        sf::FloatRect qb = qTxt.getLocalBounds();
        qTxt.setOrigin(qb.width / 2.f, qb.height / 2.f);
        qTxt.setPosition(400.f, 430.f); window.draw(qTxt);
        sf::Text ver = makeText(font, "OOP Semester Project  v1.0", 11);
        ver.setFillColor(sf::Color(50, 50, 70));
        ver.setPosition(10.f, 580.f); window.draw(ver);
        window.display();
    }
    return false;
}

static int runMapSelect(sf::RenderWindow& window, sf::Font& font) {
    struct Card { int x; const char* name; const char* hint; sf::Color accent; };
    Card cards[3] = {
        { 30,"Triple S","3 S-shaped lanes",   sf::Color(200,160, 60)},
        {290,"Triple Z","3 Z-shaped zigzags", sf::Color(80,140,220)},
        {550,"Triple U","3 deep U-turn lanes",sf::Color(60,190,100)},
    };
    while (window.isOpen()) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        int hovered = -1;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); return 0; }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                window.close(); return 0;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                for (int i = 0; i < 3; i++) {
                    if (mouse.x >= cards[i].x && mouse.x <= cards[i].x + 220 &&
                        mouse.y >= 60 && mouse.y <= 530) return i;
                }
            }
        }
        for (int i = 0; i < 3; i++)
            if (mouse.x >= cards[i].x && mouse.x <= cards[i].x + 220 &&
                mouse.y >= 60 && mouse.y <= 530) hovered = i;
        window.clear(sf::Color(18, 22, 30));
        sf::Text title = makeText(font, "SELECT MAP", 28);
        title.setFillColor(sf::Color(220, 200, 100));
        title.setPosition(290.f, 16.f); window.draw(title);
        for (int i = 0; i < 3; i++) {
            bool hov = (hovered == i);
            int cx = cards[i].x, cy = 60, cw = 220, ch = 470;
            sf::RectangleShape bg(sf::Vector2f((float)cw, (float)ch));
            bg.setPosition((float)cx, (float)cy);
            bg.setFillColor(hov ? sf::Color(38, 46, 60) : sf::Color(26, 32, 44));
            bg.setOutlineThickness(hov ? 2.f : 1.f);
            bg.setOutlineColor(hov ? cards[i].accent : sf::Color(50, 60, 78));
            window.draw(bg);
            sf::Text nt = makeText(font, cards[i].name, 20);
            nt.setFillColor(cards[i].accent);
            nt.setPosition((float)cx + 10, (float)cy + 10); window.draw(nt);
            sf::Text ht = makeText(font, cards[i].hint, 13);
            ht.setFillColor(sf::Color(160, 160, 160));
            ht.setPosition((float)cx + 10, (float)cy + 36); window.draw(ht);
            int px = cx + 10, py = cy + 60, pw = cw - 20, ph = ch - 120;
            sf::RectangleShape prev(sf::Vector2f((float)pw, (float)ph));
            prev.setPosition((float)px, (float)py);
            prev.setFillColor(sf::Color(55, 88, 38));
            prev.setOutlineThickness(1.f);
            prev.setOutlineColor(sf::Color(70, 105, 48)); window.draw(prev);
            Map tmp; tmp.setMap(i);
            for (int pi = 0; pi < tmp.getCount(); pi++)
                drawMiniPath(window, tmp.getPath(pi), px, py, pw, ph, 8);
            sf::RectangleShape btn(sf::Vector2f((float)(cw - 20), 26.f));
            btn.setPosition((float)cx + 10, (float)(cy + ch - 52));
            btn.setFillColor(hov ? cards[i].accent : sf::Color(50, 60, 78));
            window.draw(btn);
            sf::Text bt = makeText(font, hov ? "CLICK TO PLAY" : "Select", 14);
            bt.setFillColor(hov ? sf::Color(20, 20, 20) : sf::Color(160, 160, 160));
            sf::FloatRect br = bt.getLocalBounds();
            bt.setOrigin(br.width / 2.f, br.height / 2.f);
            bt.setPosition((float)cx + cw / 2.f, (float)(cy + ch - 52) + 10.f);
            window.draw(bt);
        }
        sf::Text footer = makeText(font, "Hover to preview - click to start", 15);
        footer.setFillColor(sf::Color(100, 110, 130));
        footer.setPosition(210.f, 548.f); window.draw(footer);
        window.display();
    }
    return 0;
}

static bool runEndScreen(sf::RenderWindow& window, sf::Font& font,
    int wave, int gold, bool won) {
    sf::Clock anim;
    while (window.isOpen()) {
        float t = anim.getElapsedTime().asSeconds();
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); return false; }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                window.close(); return false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                int mx = event.mouseButton.x, my = event.mouseButton.y;
                if (mx >= 280 && mx <= 520 && my >= 390 && my <= 440) return true;
                if (mx >= 280 && mx <= 520 && my >= 455 && my <= 505) { window.close(); return false; }
            }
        }
        window.clear(won ? sf::Color(5, 15, 5) : sf::Color(15, 5, 5));
        sf::Color gc = won ? sf::Color(10, 25, 10) : sf::Color(25, 10, 10);
        for (int r = 0; r < 16; r++) for (int c = 0; c < 21; c++) {
            sf::RectangleShape tile(sf::Vector2f(39.f, 39.f));
            tile.setPosition((float)c * 40, (float)r * 40);
            tile.setFillColor((r + c) % 2 == 0 ? gc : sf::Color(gc.r > 5 ? gc.r - 5 : 0, gc.g > 5 ? gc.g - 5 : 0, gc.b > 5 ? gc.b - 5 : 0));
            window.draw(tile);
        }
        sf::RectangleShape ov(sf::Vector2f(800.f, 600.f));
        ov.setFillColor(sf::Color(0, 0, 0, 150)); window.draw(ov);
        sf::RectangleShape panel(sf::Vector2f(480.f, 320.f));
        panel.setPosition(160.f, 120.f);
        panel.setFillColor(won ? sf::Color(12, 38, 12, 235) : sf::Color(38, 10, 10, 235));
        panel.setOutlineThickness(3.f);
        panel.setOutlineColor(won ? sf::Color(60, 200, 60) : sf::Color(200, 50, 50));
        window.draw(panel);
        int pulse = (int)(160 + 95 * std::sin(t * 3.f));
        sf::Text titleT = makeText(font, won ? "VICTORY!" : "GAME OVER", 52);
        titleT.setFillColor(won ? sf::Color(70, (sf::Uint8)pulse, 70)
            : sf::Color((sf::Uint8)pulse, 45, 45));
        sf::FloatRect tr = titleT.getLocalBounds();
        titleT.setOrigin(tr.width / 2.f, tr.height / 2.f);
        titleT.setPosition(400.f, 168.f); window.draw(titleT);
        sf::RectangleShape div(sf::Vector2f(380.f, 1.f));
        div.setPosition(210.f, 222.f);
        div.setFillColor(sf::Color(70, 70, 70)); window.draw(div);
        sf::Text s1 = makeText(font, "Waves Survived :  " + std::to_string(wave) + " / 5", 18);
        s1.setFillColor(sf::Color(180, 180, 255));
        s1.setPosition(230.f, 235.f); window.draw(s1);
        sf::Text s2 = makeText(font, "Gold Remaining :  " + std::to_string(gold), 18);
        s2.setFillColor(C_GOLD);
        s2.setPosition(230.f, 263.f); window.draw(s2);
        sf::Text s3 = makeText(font,
            won ? "All waves cleared! Excellent work." :
            "Your base was overwhelmed by enemies.", 15);
        s3.setFillColor(sf::Color(140, 140, 140));
        s3.setPosition(230.f, 295.f); window.draw(s3);
        bool hovR = (mouse.x >= 280 && mouse.x <= 520 && mouse.y >= 390 && mouse.y <= 440);
        sf::RectangleShape rBtn(sf::Vector2f(240.f, 50.f));
        rBtn.setPosition(280.f, 390.f);
        rBtn.setFillColor(hovR ? sf::Color(55, 170, 55) : sf::Color(35, 115, 35));
        rBtn.setOutlineThickness(2.f);
        rBtn.setOutlineColor(sf::Color(80, 210, 80)); window.draw(rBtn);
        sf::Text rTxt = makeText(font, "PLAY AGAIN", 20);
        rTxt.setFillColor(sf::Color::White);
        sf::FloatRect rb = rTxt.getLocalBounds();
        rTxt.setOrigin(rb.width / 2.f, rb.height / 2.f);
        rTxt.setPosition(400.f, 413.f); window.draw(rTxt);
        bool hovQ = (mouse.x >= 280 && mouse.x <= 520 && mouse.y >= 455 && mouse.y <= 505);
        sf::RectangleShape qBtn(sf::Vector2f(240.f, 50.f));
        qBtn.setPosition(280.f, 455.f);
        qBtn.setFillColor(hovQ ? sf::Color(170, 45, 45) : sf::Color(115, 30, 30));
        qBtn.setOutlineThickness(2.f);
        qBtn.setOutlineColor(sf::Color(210, 65, 65)); window.draw(qBtn);
        sf::Text qTxt = makeText(font, "QUIT", 20);
        qTxt.setFillColor(sf::Color::White);
        sf::FloatRect qb = qTxt.getLocalBounds();
        qTxt.setOrigin(qb.width / 2.f, qb.height / 2.f);
        qTxt.setPosition(400.f, 478.f); window.draw(qTxt);
        window.display();
    }
    return false;
}

// ─────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense");
    window.setFramerateLimit(60);

    sf::Font font;
    bool hasFont = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    if (!hasFont) hasFont = font.loadFromFile("arial.ttf");
    if (!hasFont) hasFont = font.loadFromFile("C:/Windows/Fonts/calibri.ttf");
    if (!hasFont) hasFont = font.loadFromFile("C:/Windows/Fonts/segoeui.ttf");

    while (window.isOpen()) {

        if (!hasFont || !runStartScreen(window, font)) break;
        if (!window.isOpen()) break;

        int chosenMap = runMapSelect(window, font);
        if (!window.isOpen()) break;

        Map    map;  map.setMap(chosenMap);
        player pl(&map);
        wave   w(&map);

        sf::Clock dtClock;               // ← measures frame delta time
        sf::Clock gameClock;             // ← FIX: measures total elapsed time for fire cooldowns

        int  selectedTower = 3;
        bool gameOver = false, gameWon = false;

        while (window.isOpen() && !gameOver && !gameWon) {

            float dt = dtClock.restart().asSeconds();
            float currentTime = gameClock.getElapsedTime().asSeconds(); // ← FIX: real time
            sf::Vector2i mouse = sf::Mouse::getPosition(window);

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { window.close(); break; }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) { window.close(); break; }
                    if (event.key.code == sf::Keyboard::Num1) selectedTower = 1;
                    if (event.key.code == sf::Keyboard::Num2) selectedTower = 2;
                    if (event.key.code == sf::Keyboard::Num3) selectedTower = 3;
                    if (event.key.code == sf::Keyboard::Num4) selectedTower = 4;
                    if (event.key.code == sf::Keyboard::U) {
                        tower** towers = pl.gettower();
                        int tc = pl.gettowercount();
                        for (int i = 0; i < tc; i++) {
                            if (!towers[i] || !towers[i]->isalive()) continue;
                            float dx = towers[i]->getx() - mouse.x;
                            float dy = towers[i]->gety() - mouse.y;
                            if (std::sqrt(dx * dx + dy * dy) <= 30.f) {
                                towers[i]->update(pl); break;
                            }
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    if (mouse.y > 40 && mouse.y < 580) {
                        tower* t = nullptr;
                        switch (selectedTower) {
                        case 1: t = new canontower((float)mouse.x, (float)mouse.y); break;
                        case 2: t = new machineguntower((float)mouse.x, (float)mouse.y); break;
                        case 3: t = new wizardtower((float)mouse.x, (float)mouse.y); break;
                        case 4: t = new archertower((float)mouse.x, (float)mouse.y); break;
                        }
                        if (t && !pl.placetower((float)mouse.x, (float)mouse.y, t)) delete t;
                    }
                }
            }
            if (!window.isOpen()) break;

            w.update();
            enemy** enemies = w.getEnemies();
            int total = w.getTotal();
            tower** towers = pl.gettower();
            int towerCount = pl.gettowercount();

            // ── FIX: enemy movement only — no attacking here ──
            for (int i = 0; i < total; i++) {
                if (!enemies[i] || !enemies[i]->isalive()) continue;

                bool stopped = false;
                for (int j = 0; j < towerCount; j++) {
                    if (!towers[j] || !towers[j]->isalive()) continue;
                    if (enemies[i]->towerinrange(towers[j])) {
                        enemies[i]->stop(towers[j], dt);
                        stopped = true;
                        break;
                    }
                }
                if (!stopped) {
                    enemies[i]->setstop();
                    enemies[i]->move();
                }

                if (enemies[i]->hasReachedEnd()) {
                    pl.loselife();
                    enemies[i]->takedamage(99999);
                }
            }

            // ── FIX: towers attack with correct currentTime ──
            pl.update(enemies, total, currentTime);

            if (pl.gameover())   gameOver = true;
            if (w.getWave() > 4) gameWon = true;

            // Draw
            window.clear(sf::Color(18, 22, 30));
            drawMap(window, map);
            drawEnemies(window, w.getEnemies(), w.getTotal());
            drawTowers(window, pl.gettower(), pl.gettowercount(), mouse);

            if (mouse.y > 40) {
                bool bad = pl.isonpath((float)mouse.x, (float)mouse.y)
                    || pl.overlapping((float)mouse.x, (float)mouse.y);
                sf::RectangleShape ghost(sf::Vector2f(28.f, 28.f));
                ghost.setOrigin(14.f, 14.f);
                ghost.setPosition((float)mouse.x, (float)mouse.y);
                ghost.setFillColor(bad ? sf::Color(220, 50, 50, 100) : sf::Color(80, 200, 80, 100));
                window.draw(ghost);
            }

            if (hasFont)
                drawHUD(window, font, pl.getgold(), pl.getlives(),
                    w.getWave() + 1, selectedTower,
                    map.getMapName(), pl.gettower(),
                    pl.gettowercount(), mouse);

            window.display();
        }
        
        if (!window.isOpen()) break;

        bool replay = runEndScreen(window, font, w.getWave(), pl.getgold(), gameWon);
        if (!replay) break;
    }

    return 0;
}