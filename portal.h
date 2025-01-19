#ifndef PORTAL_H
#define PORTAL_H

typedef struct {
    int one_x, one_y;
    int two_x, two_y;
    int frame;
} Portal;

typedef struct {
    Portal* portals;
    int length;
} Portals;

Portals create_portals(int amount);
bool is_portal_at(const Portals* portals, int x, int y);
bool is_near_portal(const Portals* portals, int x, int y);

#endif //PORTAL_H
