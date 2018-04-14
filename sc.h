//
//  sc.h
//  graph2
//
//  Created by Владимир Еганов on 13.04.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//

#ifndef sc_h
#define sc_h
#include"Edge.h"
#include<vector>

template <class Graph>
class sc{
    const Graph& G;
    int scnt, cnt;
    vector<int> postr;
    vector<int> posti;
    vector<int> ids;
    void dfs(const Graph &Gr, int v){
        ids[v]=scnt;
        typename Graph::adjiterrator A(inGraph, v);
        for (int i = 0; i < Gr.V(); i++) {
            if (ids[i]!=-1) {
                dfs(Gr,i);
            }
            posti[cnt++]=v;
        }
    }
public:
    sc(const Graph &Gr):G(Gr),scnt(0),cnt(0),postr(G.V()),post(G.V()),ids(G.V(),-1){
        Graph R(G.V(),0);
        reverse(G,R);
        for (int i = 0; i < R.V(); i++) {
            if (ids[i]!=-1) {
                dfs(R,i);
            }
        }
    }
};
template <class Graph>
void reverse(const Graph &inGraph, Graph &outGraph) {
    for (int i = 0; i < inGraph.V(); i++)
    {
        typename Graph::adjiterrator A(inGraph, i);
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            outGraph.insert(Edge(j,i));
        }
    }
}
#endif /* sc_h */
