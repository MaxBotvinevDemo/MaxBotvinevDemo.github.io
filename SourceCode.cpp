//
//  BridgeAlgo.cpp
//  BridgeAlgo
//
//  Created by MaxBotvinev on 24.08.17.
//  Copyright © 2017 MaxBotvinev. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <queue>

using namespace std;

const int MAX_NEIGHBORS = 6;

struct Point
{
    //Data
    int id;
    int groupID;
    int neighbors[MAX_NEIGHBORS];
    
    //Variables
    bool visited;
    int steps;
    
    Point() : id(-1), groupID(-1)
    {
        Reset();
    }
    
    void Reset()
    {
        visited = false;
        steps = 1;
    }
};


void GetBridge(int * neighbors, int nLength, int * groups, int g1Length, int g2Length, int * a, int * b)
{
    //Check groups
    if(g1Length < 1 || g2Length < 1)
    {
        printf("For Bridge Algo need 2 group of points with at least 1 point per each.");
        *a = *b = -1;
    }
    
    //PARSE ro Object Model
    int pLength = nLength / MAX_NEIGHBORS;
    Point * points = new Point[pLength];
    int p, n;
    for (p = 0, n = 0; p < pLength; ++p)
    {
        Point point;
        point.id = p;
        for(int pn = 0; pn < MAX_NEIGHBORS; ++pn)
            point.neighbors[pn] = neighbors[n++];
        points[p] = point;
    }
    
    int gLength = g1Length + g2Length;
    //Foreach point selected in groups
    for(p = 0; p < gLength; ++p)
        points[groups[p]].groupID = p < g1Length ? 0 : 1;
    
    //ALGO
    //Distances for every point of group1 to points of group2
    int * distances = new int[g1Length * g2Length];
    int d = 0;
    //ForEach Point of group1
    for (int gp1 = 0; gp1 < g1Length; ++gp1)
    {
        //ForEach Point of group2
        for (int gp2 = g1Length; gp2 < gLength; ++gp2)
        {
            //Reset
            for (p = 0; p < pLength; ++p)
            {
                Point * point = & points[p];
                point->Reset();
            }
            
            queue<Point> pQueue;
            Point * p1 = & points[groups[gp1]];
            p1->visited = true;
            pQueue.push(*p1);
            
            Point * p2 = & points[groups[gp2]];
            
            while(pQueue.size() > 0)
            {
                Point * from = & pQueue.front();
                //Foreach neighbor of visiting point
                for(n = 0; n < MAX_NEIGHBORS; ++n)
                {
                    p = from->neighbors[n];
                    if(p > -1)
                    {
                        Point * neighbor = & points[p];
                        if(!neighbor->visited)
                        {
                            neighbor->visited = true;
                            neighbor->steps += from->steps;
                            pQueue.push(*neighbor);
                            
                            if(neighbor->id == p2->id)
                            {
                                distances[d++] = p2->steps;
                            }
                        }
                    }
                }
                pQueue.pop();
            }
        }
    }
    
    //Select closest
    int minDist = INT_MAX;
    d = 0;
    for (int gp1 = 0; gp1 < g1Length; ++gp1)
    {
        for (int gp2 = g1Length; gp2 < gLength; ++gp2)
        {
            int dist = distances[d++];
            if(dist < minDist)
            {
                minDist = dist;
                
                //Output
                *a = groups[gp1];
                *b = groups[gp2];
            }
        }
    }
    
    
    //Clear
    delete [] points;
    delete [] distances;
}

//TEST
int main(int argc, const char * argv[]) {
    
    //Rows are points of GeoSphere and columns are Neighbors of each point
    int neighbors[] =
    {
        42, 44, 52, 59, 66, -1,
        55, 57, 60, 78, 107, -1,
        95, 97, 125, 127, 130, -1,
        112, 114, 122, 129, 136, -1,
        88, 90, 118, 120, 123, -1,
        48, 50, 53, 79, 85, -1,
        102, 104, 132, 134, 137, -1,
        62, 64, 67, 100, 106, -1,
        109, 111, 139, 141, 143, -1,
        81, 83, 115, 117, 146, -1,
        69, 71, 73, 93, 99, -1,
        45, 47, 76, 86, 92, -1,
        43, 42, 47, 46, 72, 75,
        46, 45, 50, 49, 84, 87,
        44, 43, 49, 48, 51, 54,
        54, 53, 57, 56, 77, 80,
        52, 51, 56, 55, 58, 61,
        61, 60, 64, 63, 105, 108,
        59, 58, 63, 62, 65, 68,
        68, 67, 71, 70, 98, 101,
        66, 65, 70, 69, 72, 74,
        74, 73, 76, 75, 91, 94,
        80, 79, 83, 82, 148, 149,
        78, 77, 82, 81, 159, 161,
        87, 86, 90, 89, 151, 152,
        85, 84, 89, 88, 147, 149,
        94, 93, 97, 96, 154, 155,
        92, 91, 96, 95, 150, 152,
        101, 100, 104, 103, 157, 158,
        99, 98, 103, 102, 153, 155,
        108, 107, 111, 110, 160, 161,
        106, 105, 110, 109, 156, 158,
        113, 112, 117, 116, 142, 145,
        116, 115, 120, 119, 147, 148,
        114, 113, 119, 118, 121, 124,
        124, 123, 127, 126, 150, 151,
        122, 121, 126, 125, 128, 131,
        131, 130, 134, 133, 153, 154,
        129, 128, 133, 132, 135, 138,
        138, 137, 141, 140, 156, 157,
        136, 135, 140, 139, 142, 144,
        144, 143, 146, 145, 159, 160,
        0, 44, 12, 43, 66, 72,
        12, 42, 14, 44, 46, 49,
        0, 42, 14, 43, 52, 51,
        11, 47, 13, 46, 86, 87,
        13, 45, 12, 47, 43, 49,
        11, 45, 12, 46, 76, 75,
        5, 50, 14, 49, 53, 54,
        14, 48, 13, 50, 46, 43,
        5, 48, 13, 49, 85, 84,
        14, 44, 16, 52, 54, 56,
        0, 44, 16, 51, 59, 58,
        5, 48, 15, 54, 79, 80,
        15, 53, 14, 48, 51, 56,
        1, 57, 16, 56, 60, 61,
        16, 55, 15, 57, 54, 51,
        1, 55, 15, 56, 78, 77,
        16, 52, 18, 59, 61, 63,
        0, 52, 18, 58, 66, 65,
        1, 55, 17, 61, 107, 108,
        17, 60, 16, 55, 58, 63,
        7, 64, 18, 63, 67, 68,
        18, 62, 17, 64, 61, 58,
        7, 62, 17, 63, 106, 105,
        18, 59, 20, 66, 68, 70,
        0, 59, 20, 65, 42, 72,
        7, 62, 19, 68, 100, 101,
        19, 67, 18, 62, 65, 70,
        10, 71, 20, 70, 73, 74,
        20, 69, 19, 71, 68, 65,
        10, 69, 19, 70, 99, 98,
        20, 66, 12, 42, 74, 75,
        10, 69, 21, 74, 93, 94,
        21, 73, 20, 69, 72, 75,
        12, 47, 21, 76, 74, 72,
        11, 47, 21, 75, 92, 91,
        15, 57, 23, 78, 80, 82,
        1, 57, 23, 77, 107, 161,
        5, 53, 22, 80, 85, 149,
        22, 79, 15, 53, 77, 82,
        9, 83, 23, 82, 146, 159,
        23, 81, 22, 83, 80, 77,
        9, 81, 22, 82, 115, 148,
        13, 50, 25, 85, 87, 89,
        5, 50, 25, 84, 79, 149,
        11, 45, 24, 87, 92, 152,
        24, 86, 13, 45, 84, 89,
        4, 90, 25, 89, 120, 147,
        25, 88, 24, 90, 87, 84,
        4, 88, 24, 89, 123, 151,
        21, 76, 27, 92, 94, 96,
        11, 76, 27, 91, 86, 152,
        10, 73, 26, 94, 99, 155,
        26, 93, 21, 73, 91, 96,
        2, 97, 27, 96, 127, 150,
        27, 95, 26, 97, 94, 91,
        2, 95, 26, 96, 130, 154,
        19, 71, 29, 99, 101, 103,
        10, 71, 29, 98, 93, 155,
        7, 67, 28, 101, 106, 158,
        28, 100, 19, 67, 98, 103,
        6, 104, 29, 103, 134, 153,
        29, 102, 28, 104, 101, 98,
        6, 102, 28, 103, 137, 157,
        17, 64, 31, 106, 108, 110,
        7, 64, 31, 105, 100, 158,
        1, 60, 30, 108, 78, 161,
        30, 107, 17, 60, 105, 110,
        8, 111, 31, 110, 141, 156,
        31, 109, 30, 111, 108, 105,
        8, 109, 30, 110, 143, 160,
        3, 114, 32, 113, 136, 142,
        32, 112, 34, 114, 116, 119,
        3, 112, 34, 113, 122, 121,
        9, 117, 33, 116, 83, 148,
        33, 115, 32, 117, 113, 119,
        9, 115, 32, 116, 146, 145,
        4, 120, 34, 119, 123, 124,
        34, 118, 33, 120, 116, 113,
        4, 118, 33, 119, 88, 147,
        34, 114, 36, 122, 124, 126,
        3, 114, 36, 121, 129, 128,
        4, 118, 35, 124, 90, 151,
        35, 123, 34, 118, 121, 126,
        2, 127, 36, 126, 130, 131,
        36, 125, 35, 127, 124, 121,
        2, 125, 35, 126, 95, 150,
        36, 122, 38, 129, 131, 133,
        3, 122, 38, 128, 136, 135,
        2, 125, 37, 131, 97, 154,
        37, 130, 36, 125, 128, 133,
        6, 134, 38, 133, 137, 138,
        38, 132, 37, 134, 131, 128,
        6, 132, 37, 133, 102, 153,
        38, 129, 40, 136, 138, 140,
        3, 129, 40, 135, 112, 142,
        6, 132, 39, 138, 104, 157,
        39, 137, 38, 132, 135, 140,
        8, 141, 40, 140, 143, 144,
        40, 139, 39, 141, 138, 135,
        8, 139, 39, 140, 109, 156,
        40, 136, 32, 112, 144, 145,
        8, 139, 41, 144, 111, 160,
        41, 143, 40, 139, 142, 145,
        32, 117, 41, 146, 144, 142,
        9, 117, 41, 145, 81, 159,
        33, 120, 25, 88, 148, 149,
        22, 83, 33, 115, 147, 149,
        25, 85, 22, 79, 148, 147,
        35, 127, 27, 95, 151, 152,
        24, 90, 35, 123, 150, 152,
        27, 92, 24, 86, 151, 150,
        37, 134, 29, 102, 154, 155,
        26, 97, 37, 130, 153, 155,
        29, 99, 26, 93, 154, 153,
        39, 141, 31, 109, 157, 158,
        28, 104, 39, 137, 156, 158,
        31, 106, 28, 100, 157, 156,
        41, 146, 23, 81, 160, 161,
        30, 111, 41, 143, 159, 161,
        23, 78, 30, 107, 160, 159,
    };
    
    int groups[] =
    {
        //Group1
        99, 29, 153, 102, 134, 6, 103, 98, 10, 71,  //10 points are selected in G1
        //Group2
        61, 17, 108, 110, 31, 106, 7, 62, 100, 109, //10 points are selected in G2
    };
    
    int nLength = 972, g1Length = 10, g2Length = 10, a = -1, b = -1;
    
    clock_t begin = clock();
    GetBridge(neighbors, nLength, groups, g1Length, g2Length, &a, &b);
    clock_t end = clock();
    
    double sec = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Closest points are: " << a << " <--> " << b << ", Time: " << sec << endl;
    
    return 0;
}
