//
//  vshortest.hpp
//  graph2
//
//  Created by Владимир Еганов on 30.03.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//

#ifndef vshortest_hpp
#define vshortest_hpp

#include <stdio.h>
#include "Edge.h"
#include <vector>
#include <iostream>
#include "BFS.h"
#include <fstream>
#include <unordered_set>

using namespace std;

template <class Graph>
class vshortest {
    const Graph &G;
    int start;
    int finish;
public:
    vshortest(const Graph &Gr,int start, int finish):G(Gr),start(start),finish(finish){}
    void draw(){
        BFS<Graph> bfs(G,start);
        vector<int> v = bfs.getwhereV();
        unordered_set<Edge,hash_edge> edges;
        int ret = finish;
        while (ret != start) {
            edges.insert(Edge(ret,v[ret]));
            ret = v[ret];
        }
        ofstream myfile;
        myfile.open("graph");
        myfile << "Graph G{\ngraph [overlap=scale sep=3.5];\nnode [shape=circle];\n";
        for (int i = 0; i < G.V(); i++)
        {
            typename Graph::adjiterrator A(G, i);
            for (int j = A.begin(); !A.end(); j = A.next())
            {
                if (i <= j)
                {
                    myfile << '\t' << i << " -- " << j;
                    if (edges.find(Edge(i,j))!=edges.end()||(edges.find(Edge(j,i))!=edges.end())) {
                        myfile << " [color=red penwidth=3.5];"<<endl;
                    }
                    else{
                        myfile <<";"<<endl;
                    }
                }
            }
        }
        myfile << "}";
        myfile.close();
        system("/usr/local/bin/circo graph -Tpng -o grapg.png && open grapg.png");
    }
};

#endif /* vshortest_hpp */
