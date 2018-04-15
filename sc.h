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

template <class in_Graph, class out_Graph>
void reverse(const in_Graph &inGraph, out_Graph &outGraph) {
    for (int i = 0; i < inGraph.V(); i++)
    {
        typename in_Graph::adjiterrator A(inGraph, i);
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            outGraph.insert(Edge(j,i));
        }
    }
}

template <class Graph>
class sc{
    const Graph& G;
    int scnt, cnt;
    vector<int> postr;
    vector<int> posti;
    vector<int> ids;
    void dfs(const Graph &Gr, int v){
        ids[v]=scnt;
        typename Graph::adjiterrator A(Gr, v);
        for (int i = A.begin() ; !A.end() ; i = A.next()) {
            if (ids[i]==-1) {
                dfs(Gr,i);
            }
        }
        posti[cnt++]=v;
    }
public:
    sc(const Graph &Gr):G(Gr),scnt(0),cnt(0),postr(G.V()),posti(G.V()),ids(G.V(),-1){
        Graph R(G.V(),0);
        reverse<Graph, Graph>(G,R);
        for (int i = 0; i < R.V(); i++) {
            if (ids[i]==-1) {
                dfs(R,i);
            }
        }
        postr = posti;
        cnt = 0;
        scnt = 0;
        ids.assign(G.V(),-1);
        for (int i = G.V()-1; i >= 0; i--) {
            if (ids[postr[i]]==-1) {
                dfs(G,postr[i]);
                scnt++;
            }
        }
    }
    int count(){
        return cnt;
    }
    vector<int> getId(){
        return ids;
    }
};

#endif /* sc_h */
