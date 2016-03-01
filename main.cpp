//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <stdio.h>
#include "RTree.h"

using namespace std;

struct Rect {
    Rect() { }

    Rect(int a_minX, int a_minY, int a_maxX, int a_maxY) {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
    }


    int min[2];
    int max[2];
};

struct Rect rects[] =
{
    Rect(0, 0, 2, 2), // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
    Rect(5, 5, 7, 7),
    Rect(6, 6, 8, 8),
    Rect(1, 1, 3, 3),
};

int nrects = sizeof(rects) / sizeof(rects[0]);

Rect search_rect(6, 6, 7, 7); // search will find above rects that this one overlaps

bool MySearchCallback(int id, void *arg, double Mi[2], double Ma[2]) {
    printf("Hit data rect %d\n", id);
    printf("%lf,%lf,%lf,%lf\n", Mi[0], Mi[1], Ma[0], Ma[1]);
    return true; // keep going
}

int main() {

    RTree<int, int, 2> rtree;

    int i, nhits;
    printf("nrects = %d\n", nrects);

    for (i = 0; i < nrects; i++) {
        rtree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
    }

    nhits = rtree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

    printf("Search resulted in %d hits\n", nhits);

    // Iterator test
    int itIndex = 0;
    RTree<int, int, 2>::Iterator it;
    for (rtree.GetFirst(it);
         !rtree.IsNull(it);
         rtree.GetNext(it)) {
        int value = rtree.GetAt(it);
        int boundsMin[2] = {0, 0};
        int boundsMax[2] = {0, 0};
        it.GetBounds(boundsMin, boundsMax);
        printf("it[%d] %d = (%d,%d,%d,%d)\n", itIndex++, value, boundsMin[0], boundsMin[1], boundsMax[0], boundsMax[1]);
    }

    return 0;
}
