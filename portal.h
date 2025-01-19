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
bool is_portal_at(const Portals* portal, int x, int y);

#endif //PORTAL_H
