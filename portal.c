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
