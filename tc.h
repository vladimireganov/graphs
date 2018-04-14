//
//  tc.h
//  graph2
//
//  Created by Владимир Еганов on 01.04.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//

#ifndef tc_h
#define tc_h
#include "Edge.h"
template< class Graph>
class tc{
    Graph T;
public:
    tc(const Graph &Gr):T(Gr.V(),0){
        for (int i = 0; i < Gr.V(); i++) {
            for (int j = 0; j < Gr.V() ; j++) {
                T.remove(Edge(i,j));
                for (int k = 0; k < Gr.V(); k++) {
                    if (Gr.way(Edge(i,j)) && Gr.way(Edge(j,k))) {
                        T.insert(Edge(i,k));
                    }
                }
            }
        }
    }
};
template< class Graph>
class tc_dfs{
    Graph T;
    const Graph& G;
    void dfs(int v){
        typename Graph::adjiterrator A(G, v);
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            T.insert(Edge(v,j));
            dfs(j);
            typename Graph::adjiterrator B(T, j);
            for (int k = B.begin(); !B.end(); k = B.next())
            {
                T.insert(Edge(v,k));
            }
            
            
        }
    }
    void dfs(int v, int w){
        T.insert(Edge(v,w));
        typename Graph::adjiterrator A(G, w);
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            if (!T.way(Edge(v,j))) {
                dfs(v, j);
            }
        }
    }
public:
    tc_dfs(const Graph &Gr):T(Gr.V(),0),G(Gr){
        
        for (int i = 0; i < Gr.V(); i++) {
            dfs(i,i);
            
        }
        
    }
    Graph& get_graph(){
        return T;
    }
};

#endif /* tc_h */
