//
//  BFS.h
//  graph2
//
//  Created by Владимир Еганов on 30.03.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//

#ifndef BFS_h
#define BFS_h

#include <vector>
#include <queue>

template <class Graph>
class BFS
{
private:
    const Graph &G;
    vector <int> used;
    vector <int> where;
    int it=0;
public:
    BFS(const Graph &_G,int first=-1) :G(_G),used(G.V(),-1),where(G.V(),-1){
        if (first == -1) {
            for (int i = 0; i < G.V(); i++)
            {
                //            where[i]=i;
                if (used[i]==-1) bfs(Edge(i,i));
            }
        }else{
            bfs(Edge(first,first));
        }
        
    }
    void bfs(Edge start){
        queue <Edge> q;
        //        used[start.first]=it++;
        q.push(start);
        while (!q.empty()){
            start= q.front();
            
            q.pop();
            if (used[start.second]==-1) {
                
                used[start.second]=it++;
                where[start.second]=start.first;
                typename Graph::adjiterrator A (G, start.second);
                for (int j = A.begin(); !A.end(); j = A.next())
                {
                    if (used[j]==-1){
                        q.push(Edge(start.second,j));
                        //where[j]=start.first;
                        //used[j]=it++;
                    }
                }
            }
        }
    }
    vector<int> getwhereV(){
        return where;
    }
    vector<int> getusedV(){
        return used;
    }
};
#endif /* BFS_h */
