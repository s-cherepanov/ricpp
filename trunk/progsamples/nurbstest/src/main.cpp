#include "trimgrid.h"

using namespace RiCPP;

int main (int argc, char * const argv[]) {
    std::cout << "S-T-A-R-T!\n";
    std::cout << "------------------------------\n";

    // insert code here...
    RtFloat knots[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3};
    RtInt nknot = sizeof(knots)/sizeof(RtFloat);

    CTrimGrid grid(nknot, knots, 0,   3,   10,
                   nknot, knots, 0.5, 2.5,  3);

    std::cout << "CTrimGrid grid" << std::endl;
    grid.write(std::cout, "    ");

    RtInt nloops = 2;
    RtInt ncurves[] = {1, 1};
    RtInt order[] = { 4, 4};
    RtFloat knot[] = {
        0, 0, 0, 0,
        1, 1, 1, 2, 2, 2, 3, 3, 3,
        4, 4, 4, 5, 5, 5, 6, 6, 6,
        7, 7, 7, 8, 8, 8, 9, 9, 9,
        10, 10, 10, 11, 11, 11, 12, 12, 12,
        13, 13, 13, 14, 14, 14, 15, 15, 15,
        16, 16, 16, 16,

        0, 0, 0, 0,
        1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4,
        5, 5, 5, 5
    };
    RtFloat amin[]  = { 0, 0};
    RtFloat amax[] = { 16, 5 };
    RtInt n[] = { 49, 16 };
    RtFloat u[] = {
        (RtFloat)0.216594, (RtFloat)0.228702, (RtFloat)0.257058, (RtFloat)0.459505, (RtFloat)0.627541,
        (RtFloat)0.70847 , (RtFloat)0.70847 , (RtFloat)0.70847 , (RtFloat)0.657874, (RtFloat)0.61537,
        (RtFloat)0.517633, (RtFloat)0.419601, (RtFloat)0.321863, (RtFloat)0.026317, (RtFloat)0,
        (RtFloat)0       , (RtFloat)0       , (RtFloat)0.137641, (RtFloat)0.331995, (RtFloat)0.538457,
        (RtFloat)0.645765, (RtFloat)0.714586, (RtFloat)0.72064 , (RtFloat)0.744919, (RtFloat)0.902823,
        (RtFloat)0.94335 , (RtFloat)0.969667, (RtFloat)1       , (RtFloat)1       , (RtFloat)1,
        (RtFloat)1       , (RtFloat)0.979737, (RtFloat)0.957497, (RtFloat)0.941311, (RtFloat)0.904862,
        (RtFloat)0.88256 , (RtFloat)0.88256 , (RtFloat)0.882561, (RtFloat)0.88256 , (RtFloat)0.88256,
        (RtFloat)0.88256 , (RtFloat)0.574906, (RtFloat)0.489899, (RtFloat)0.228702, (RtFloat)0.060728,
        (RtFloat)0.052635, (RtFloat)0.107234, (RtFloat)0.161995, (RtFloat)0.216594, (RtFloat)0.704454,
        (RtFloat)0.704454, (RtFloat)0.542472, (RtFloat)0.376475, (RtFloat)0.242911, (RtFloat)0.184222,
        (RtFloat)0.184222, (RtFloat)0.184222, (RtFloat)0.323902, (RtFloat)0.410947, (RtFloat)0.631556,
        (RtFloat)0.676098, (RtFloat)0.704454, (RtFloat)0.704455, (RtFloat)0.704454, (RtFloat)0.704454
    };
    RtFloat v[] = {
        (RtFloat)0.314607, (RtFloat)0.242316, (RtFloat)0.132002, (RtFloat)0.132002, (RtFloat)0.132002,
        (RtFloat)0.186248, (RtFloat)0.285691, (RtFloat)0.379725, (RtFloat)0.394184, (RtFloat)0.397826,
        (RtFloat)0.408669, (RtFloat)0.408668, (RtFloat)0.430385, (RtFloat)0.462888, (RtFloat)0.64737,
        (RtFloat)0.726947, (RtFloat)0.889686, (RtFloat)1       , (RtFloat)1       , (RtFloat)1,
        (RtFloat)0.913195, (RtFloat)0.846256, (RtFloat)0.918603, (RtFloat)0.99095 , (RtFloat)0.99095,
        (RtFloat)0.99095 , (RtFloat)0.980078, (RtFloat)0.972849, (RtFloat)0.934315, (RtFloat)0.934314,
        (RtFloat)0.857127, (RtFloat)0.860714, (RtFloat)0.864356, (RtFloat)0.864356, (RtFloat)0.864356,
        (RtFloat)0.848077, (RtFloat)0.808289, (RtFloat)0.627043, (RtFloat)0.627042, (RtFloat)0.264003,
        (RtFloat)0.021688, (RtFloat)0       , (RtFloat)0       , (RtFloat)0       , (RtFloat)0.088571,
        (RtFloat)0.314607, (RtFloat)0.314608, (RtFloat)0.314607, (RtFloat)0.314607, (RtFloat)0.645549,
        (RtFloat)0.772143, (RtFloat)0.867943, (RtFloat)0.867943, (RtFloat)0.867943, (RtFloat)0.806468,
        (RtFloat)0.703438, (RtFloat)0.584074, (RtFloat)0.560565, (RtFloat)0.549694, (RtFloat)0.524419,
        (RtFloat)0.509906, (RtFloat)0.490039, (RtFloat)0.541825, (RtFloat)0.541824, (RtFloat)0.645549
    };
    RtFloat w[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    CTrimPath trimPath(nloops, ncurves,
                       order, knot,
                       amin, amax,
                       n, u, v, w);

    std::vector<CTrimCoords> trimLoopCoords;
    trimPath.tesselate(trimLoopCoords, 16);

    std::cout << "CTrimPath trimPath" << std::endl;
    trimPath.write(std::cout, "    ");

    writeVector(trimLoopCoords, "trimLoopCoords", std::cout, "");

    std::cout << "------------------------------\n";

    grid.trim(trimPath);

    std::cout << "E-N-D!\n";
    return 0;
}
