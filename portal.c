#include "portal.h"

#include <stdlib.h>


Portals create_portals(int amount) {
    Portals p;
    p.length = amount;
    p.portals = (Portal*) malloc(sizeof(Portal) * amount);
    return p;
}

bool is_portal_at(const Portals* portals, int x, int y) {
    for (int i = 0; i < portals->length; i++) {
        Portal portal = portals->portals[i];
        if (portal.one_x == x && portal.one_y == y || portal.two_x == x && portal.two_y == y) {
            return true;
        }
    }
    return false;
}

bool is_near_portal(const Portals* portals, int x, int y) {
    const int offsets[][2] = {{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int i = 0; i < portals->length; i++) {
        Portal portal = portals->portals[i];

        for (int j = 0; j < 5; j++) {
            const int nx = x + offsets[j][0];
            const int ny = y + offsets[j][1];

            if ((portal.one_x == nx && portal.one_y == ny) ||
                (portal.two_x == nx && portal.two_y == ny)) {
                return true;
            }
        }
    }
    return false;
}
