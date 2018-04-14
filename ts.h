//
//  ts.h
//  graph2
//
//  Created by Владимир Еганов on 08.04.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//

#ifndef ts_h
#define ts_h
#include<vector>
#include<fstream>
template< class Graph>
class ts{
    const Graph& G;
    vector<int> v;
    vector<int>post;
    vector<int>posti;
    int count;
    int tcount;
    void dfs(int ver){
        v[ver]=count++;
        typename Graph::adjiterrator A(G, ver);
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            if (v[j]!=-1) {
                dfs(j);
            }
        }
        posti[tcount]=ver;
        post[ver]=tcount++;
    }
public:
    ts(const Graph &Gr):G(Gr),v(G.V(),-1),post(G.V(),-1),posti(G.V(),-1),count(0),tcount(0){
        for(int i = 0 ;i < G.V() ;i++)
            if (v[i]==-1) {
                dfs(i);
            }
        }
    vector<int>getpost(){
        return posti;
    }
};


template<class Graph>
class ts_draw{
    const Graph& G;
public:
    ts_draw(const Graph &Gr):G(Gr){}
    void draw()
    {
        ofstream myfile;
        myfile.open("graph");
        myfile << "Digraph G{\n{graph [rank=same];\nnode [shape=circle];\n";
        ts<Graph> ts(G);
        vector<int>posti=ts.getpost();
        for (int i = 0; i < G.V(); i++)
        {
            typename Graph::adjiterrator A(G, posti[i]);
            for (int j = A.begin(); !A.end(); j = A.next())
            {
                myfile << '\t' << i << " -> "<< j << ";" << endl;
            }
        }
        myfile << "}}";
        myfile.close();
        system("/usr/local/bin/dot graph -Tpng -o graph.png && open graph.png");
    }
};


#endif /* ts_h */
