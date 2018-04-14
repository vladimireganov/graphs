//
//  pfs.h
//  graph2
//
//  Created by Владимир Еганов on 18.03.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//
#ifndef pfs_h
#define pfs_h

#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include "Edge.h"

using namespace std;

template <class item>
class qrand{
private:
    vector<item> q;
    int N;
public:
    qrand(int maxN):N(0),q(maxN+1,0){
        srand(time(NULL));
    }
    int empty(){
        return N==0;
    }
    void put(item it){
        q[N++]=it;
    }
    item get()
    {
        int i = int(N*rand() / (1.0 + RAND_MAX));
        item tmp = q[i];
        q[i]=q[N-1];
        N--;
        return tmp;
    }
};

template <class Graph>
class PFS{
private:
    vector<int> ord;
    vector<int> st;
    Graph &G;
    int cnt;
public:
    PFS(Graph &Gr):G(Gr),ord(Gr.V(),-1),st(Gr.V(),-1),cnt(0){
        for (int i = 0; i < G.V(); i++)
        {
//            st[i]=i;
            if (ord[i]==-1) pfs(i);
        }
    }
    void pfs(Edge e){
        qrand <Edge> q(G.V());
        ord[e.second]=cnt++;
        q.put(e);
        while (!q.empty()){
            e=q.get();
            st[e.second]=e.first;
            typename Graph::adjiterrator A (G, e.second);
            for (int j = A.begin(); !A.end(); j = A.next())
            {
                if (ord[j]==-1){
                    q.put(Edge(e.second,j));
//                    st[j]=e.second;
                    ord[j]=cnt++;
                }
            }
        }
    }
    void print(){
        for (int i = 0; i< G.V(); i++) {
//            cout<<"i =  "<<i<<" "<<ord[i]<<"  "<<st[i]<<endl;
            cout<<i<<setw(4)<<ord[i]<<setw(4)<<st[i]<<endl;
        }
    }
};

#endif /* pfs_h */
