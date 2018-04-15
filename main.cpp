//
//  main.cpp
//  graph2
//
//  Created by Владимир Еганов on 02.03.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
#include <queue>
#include <algorithm>
#include "pfs.h"
#include "Edge.h"
#include "BFS.h"
#include "vshortest.hpp"
#include "tc.h"
#include "ts.h"
#include "sc.h"
using namespace std;



//матрица связности
class DenseGRAPH
{
private:
    vector<vector<bool>> adj;
    bool dir;
    int vCount,eCount;
public:
    DenseGRAPH(){
        vCount=0;
        eCount=0;
    }
    DenseGRAPH(DenseGRAPH& G){
        dir=G.D();
        vCount=G.V();
        eCount=G.E();
        adj = vector<vector<bool>>(vCount,vector<bool>(vCount,0));
        for (int i = 0; i < G.V()-1; i++) {
            for (int k = 0; k < G.V(); k++) {
                if (G.way(Edge(i,k))) {
                    adj[i][k]=1;
                }
            }
        }
    }
    DenseGRAPH(int ver,bool twoside){
        vCount = ver;
        eCount = 0;
        adj = vector<vector<bool>>(ver,vector<bool>(ver,0));
        dir = twoside;
        
    }
    int V()const{return vCount;}
    int E()const{return eCount;}
    bool D()const{return dir;}
    void insert(Edge e){
        if (!adj[e.first][e.second]) {
            eCount++;
        }
        adj[e.first][e.second]=1;
        
        if (dir) {
            adj[e.second][e.first]=1;
        }
    }
    void remove(Edge e){
        if (adj[e.first][e.second]) {
            eCount--;
        }
        adj[e.first][e.second]=0;
        
        if (dir) {
            adj[e.second][e.first]=0;
        }
    }
    bool way(Edge e)const{
        return adj[e.first][e.second];
    }
    class adjiterrator;
    friend class adjiterrator;
    
};
class DenseGRAPH::adjiterrator{
private:
    const DenseGRAPH &G;
    int i=-1,v;
public:
    adjiterrator(const DenseGRAPH &Gr,int _v):G(Gr),v(_v){}
    int next(){
        for (i++; i<G.V(); i++) {
            if (G.adj[v][i]) {
                return i;
            }
        }
        return -1;
    }
    int begin(){
        i=-1;
        return next();
    }
    bool end(){
        return i >= G.V();
    }
};

class SparseGRAPH
{
private:
    struct Node{
    public:
        int v;
        Node* next;
        Node(){
            v=-1;
            next=nullptr;
        }
        Node(int x, Node* nex):v(x),next(nex){}
        bool exist(int x){
            Node* tmp=this;
            while(tmp!=nullptr){
                if(tmp->v==x)return 1;
                tmp = tmp->next;
            }
            return 0;
        }
        void del(int x){
            Node* tmp=this;
            while(tmp->next!=nullptr){
                if(tmp->next->v==x){
                    Node *temp = tmp->next;
                    tmp->next = tmp->next->next;
                    delete temp;
                }
                tmp = tmp->next;
            }
        }
    };
    vector<Node*> adj;
    bool dir;
    int vCount,eCount;
public:
    SparseGRAPH(int ver, bool twoside):dir(twoside),vCount(ver),eCount(0),adj(ver,0){}
    int V()const{return vCount;}
    int E()const{return eCount;}
    bool D()const{return dir;}
    void insert(Edge e){
        if (adj[e.first]->exist(e.second)) {
            return;
        }
        eCount++;
        adj[e.first]=new Node(e.second,adj[e.first]);
        if (dir) {
            adj[e.second]=new Node(e.first,adj[e.second]);
        }
    }
    void remove(Edge e){
        if (adj[e.first]->exist(e.second)) {
            eCount--;
        }
        else{ return;}
        if (adj[e.first]->v==e.second) {
            Node *temp = adj[e.first];
            adj[e.first] = adj[e.first]->next;
            delete temp;
        }
        else{
            adj[e.first]->del(e.second);
        }
        if (dir) {
            if (adj[e.second]->v==e.first) {
                Node *temp = adj[e.second];
                adj[e.second] = adj[e.second]->next;
                delete temp;
            }
            else{
                adj[e.second]->del(e.first);
            }
        }
    }
    bool way(Edge e)const{
        return adj[e.first]->exist(e.second);
    }
    class adjiterrator;
    friend class adjiterrator;
};
class SparseGRAPH::adjiterrator{
private:
    const SparseGRAPH &G;
    Node* i;
    int v;
public:
    adjiterrator(const SparseGRAPH &Gr,int _v):G(Gr),v(_v),i(NULL){}
    int next(){
        if (i->next) {
            i=i->next;
            return i->v;
        }
        return -1;
    }
    int begin(){
        i=G.adj[v];
        return i ? i->v : -1;
    }
    bool end(){
        return !i;
    }
};

template<class Graph>
class BI
{
    const Graph &G;
    bool OK;
    vector<int> vc;
    bool dfsR(int v, int c)
    {
        vc[v] = (c + 1) % 2;
        typename Graph::adjiterrator A(G, v);
        for (int t = A.begin(); !A.end(); t = A.next())
        {
            if (vc[t] == -1)
            {
                if (!dfsR(t, vc[v]))
                {
                    return false;
                }
            }
            else if (vc[t] != c)
            {
                return false;
            }
        }
        return true;
    }
public:
    BI(const Graph &G) : G(G), OK(true), vc(G.V(), -1)
    {
        for (int v = 0; v < G.V(); v++)
        {
            if (vc[v] == -1)
            {
                if (!dfsR(v, 0))
                {
                    OK = false;
                    return;
                }
            }
        }
    }
    bool bipartite() const
    {
        return OK;
    }
    int color(int v) const
    {
        return vc[v];
    }
};


template <class Graph>
class G2neato
{
private:
    const Graph &G;
    
public:
    G2neato(const Graph &_G) :G(_G)
    {
    }
    
    void graph()
    {
        char dir;
        ofstream myfile;
        myfile.open("graph");
        if(G.D()){
            myfile << "Graph ";
            dir='-';
        }
        else{
            myfile << "Digraph ";
            dir='>';
        }
        myfile << "G{\ngraph [overlap=scale sep=3.5];\nnode [shape=circle];\n";
        for (int i = 0; i < G.V(); i++)
        {
            typename Graph::adjiterrator A(G, i);
            for (int j = A.begin(); !A.end(); j = A.next())
            {
                if ((i <= j) || !G.D())
                {
                    
                    myfile << '\t' << i << " -" << dir << " " << j << ";" << endl;
                }
            }
        }
        myfile << "}";
        myfile.close();
        system("/usr/local/bin/neato graph -Tpng -o graph.png && open graph.png");
    }
    void bidraw(){
        BI<Graph> bi(G);
        if (!bi.bipartite()) {
            cout<< "not bipartite"<<endl;
            return;
        }
        ofstream myfile;
        myfile.open("graph");
        myfile << "Graph G{\ngraph [overlap=scale sep=3.5];\nnode [shape=circle];\n";
        for (int i = 0; i < G.V(); i++){
            myfile << '\t' << i;
            if (bi.color(i))
            {
                myfile <<" [color=red];" << endl;
            }
            else{
                myfile <<" [color=green];" << endl;
            }
            
        }
        for (int i = 0; i < G.V(); i++)
        {
            typename Graph::adjiterrator A(G, i);
            for (int j = A.begin(); !A.end(); j = A.next())
            {
                if (i <= j)
                {
                    myfile << '\t' << i << " -- " << j << ";" << endl;
                }
            }
        }
        myfile << "}";
        myfile.close();
        system("/usr/local/bin/circo graph -Tpng -o grapg.png && open grapg.png");
    }
};
template <class Graph>
class DFS
{
private:
    const Graph &G;
    vector <int> used;
    vector <int> where;
    int it=0;
public:
    DFS(const Graph &_G) :G(_G),used(G.V(),-1),where(G.V(),-1){
        for (int i = 0; i < G.V(); i++)
        {
//            where[i]=i;
            if (used[i]==-1) dfs(i);
        }
    }
    void dfs(int start){
        typename Graph::adjiterrator A(G, start);
        used[start]=it++;
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            if (used[j]==-1){
                dfs(j);
                where[j]=start;
            }
            
        }
        
    }
    vector<int> getusedV(){
        return used;
    }
    vector<int> getwhereV(){
        return where;
    }
};



template <class Graph>
class countCompanents {
    const Graph &G;
    vector<int> compnums;
    int compcount = 0;
    void depth(int v){
        typename Graph::adjiterrator it(G,v);
        compnums[v] = compcount;
        for (int t = it.begin(); !it.end(); t = it.next()) {
            
            if (compnums[t]==-1) {
                depth(t);
            }
        }
    }
public:
    countCompanents (const Graph &Gr):G(Gr),compnums(G.V(),-1){
        for (int i = 0; i < G.V(); i++) {
            if (compnums[i]==-1) {
                depth(i);
                compcount++;
            }
        }
    }
    bool connect(int v1, int v2){
        return compnums[v1]==compnums[v2];
    }
    int operator[](int op){
        return compnums[op];
    }
    int count(){
        return compcount;
    }
};

template <class Graph>
class countBridges {
    Graph &G;
    int Bridges = 0;
public:
    countBridges(Graph &Gr):G(Gr){
        int t = countCompanents<Graph>(G).count();
        for (int i = 0; i < G.V(); i++) {
            for (int j = 0; j < G.V(); j++) {
                if (G.way(Edge(i,j))) {
                    G.remove(Edge(i,j));
                    int t2 = countCompanents<Graph>(G).count();
                    if (t2 > t) {
                        Bridges++;
                    }
                    G.insert(Edge(i,j));
                }
            }
        }
    }
    int bridges(){
        return Bridges;
    }
};

template <class Graph>
class countBridges2{
private:
    Graph &G;
    vector<int> ord;
    vector<int> low;
    int Bridges;
    int cnt;
public:
    countBridges2(Graph &Gr):G(Gr),ord(G.V(),-1),low(G.V(),-1),cnt(0){
//        dfs(Edge(0,0));
        for (int i = 0; i < G.V(); i++)
        {
            if (ord[i]==-1) dfs(Edge(i,i));
        }
    }
    void dfs(Edge e){
        ord[e.second]=cnt++;
        low[e.second]=ord[e.second];
        typename Graph::adjiterrator A(G, e.second);
        for (int j = A.begin(); !A.end(); j = A.next())
        {
            if (ord[j]==-1){
                dfs(Edge(e.second,j));
                if (low[e.second]>low[j]) {
                    low[e.second]=low[j];
                }
                if (low[j]==ord[j]) {
                    Bridges++;
                }
            }else if(j!=e.first){
                if (low[e.second]>ord[j]) {
                    low[e.second]=ord[j];
                }
            }
            
        }
    }
    int bridges(){
        return Bridges;
    }
    void print(){
        for (int i = 0; i < G.V(); i++) {
            cout<< ord[i]<<"  "<<low[i]<<endl;
        }
    }
};
template<class Graph>
class SEARCH
{
//protected:
public:
    const Graph &G;
    int cnt;
    vector<int> ord;
    virtual void searchC(Edge) = 0;
    void search()
    {
        for (int v = 0; v < G.V(); v++)
        {
            if (ord[v] == -1)
                searchC(Edge(v, v));
        }
    }
//public:
    SEARCH(const Graph &G) : G(G), ord(G.V(), -1), cnt(0)
    {}
    int operator[](int v) const
    {
        return ord[v];
    }
};
template <class Graph>
class EC : public SEARCH<Graph>
{
    int bcnt;
    vector<int> low;
 //   int cnt;
 //   vector<int> ord;
 //   Graph &G;
    void searchC(Edge e)
    {
        int w = e.second;
        this->ord[w] = this->cnt++;
        low[w] = this->ord[w];
        typename Graph::adjiterrator A(this->G, w);
        for (int t = A.begin(); !A.end(); t = A.next())
        {
            if (this->ord[t] == -1)
            {
                searchC(Edge(w, t));
                if (low[w] > low[t])
                    low[w] = low[t];
                if (low[t] == this->ord[t]) //если минимальный прямой номер равен
                    bcnt++;
            }
            else if (t != e.first) //если вершина помечена и ссылка в t не родительная
                if (low[w] > this->ord[t]) //если ссыла в t - обратная
                    low[w] = this->ord[t];
        }
    }
    void search()
    {
        for (int v = 0; v < this->G.V(); v++)
        {
            if (this->ord[v] == -1)
                searchC(Edge(v, v));
        }
    }
public:
    EC(Graph &G) : SEARCH<Graph>(G), bcnt(0),low(G.V(), -1)
    {
        search();
    }
    int count() const { return bcnt; }
};
template <class Graph>
class neatoAnimation {
    const Graph &G;
    
public:
    neatoAnimation(const Graph &Gr):G(Gr){
        
    }
    void graphAnimationDFS(){
        
         DFS<Graph> dfs(G);
        string base_string = "/usr/local/bin/circo graph -Tpng -o graph";
        string graphstr = "graph";
        string base_string2 = ".png && open graph";
        string base_string3 = ".png";
        string output;
        vector<int> v = dfs.getusedV();
        vector<int> v2 = dfs.getwhereV();
        const int n = 1;
        string out =string("/usr/local/bin/convert -delay 100 -loop 0 ");
        for (int k = 0; k < G.V() ; k+=n) {
            ofstream show;
            show.open("graph");
            show <<"Graph G{\ngraph [overlap=scale sep=3.5];\nnode [shape=circle];\n";
            for (int i = 0; i < G.V(); i++){
                typename Graph::adjiterrator A(G, i);
                for (int j = A.begin(); !A.end(); j = A.next())
                {
                    if (i <= j)
                    {
                        show << '\t' << i << " -- " << j;
                        if (v[i]<k && (v2[i]==j || v2[j]==i)) {
                            show <<" [penwidth=10.0]";
                        }
                        show << " ;" << endl;
                    }
                }
            }
            show << "}";
            show.close();
            output = base_string+ to_string(k)+".png";//base_string2+to_string(k)+base_string3;
            out += graphstr+to_string(k)+string(".png ");
            system(output.c_str());
            cout<< output<<endl;
        }
    
        out+= "DFSmyimage.gif && open DFSmyimage.gif";
        system(out.c_str());
        cout<< endl<< out;
    }
    void graphAnimationBFS(){
        
        BFS<Graph> bfs(G);
        string base_string = "/usr/local/bin/circo graph -Tpng -o graph";
        string graphstr = "graph";
        string base_string2 = ".png && open graph";
        string base_string3 = ".png";
        string output;
        vector<int> v = bfs.getusedV();
        vector<int> v2 = bfs.getwhereV();
        const int n = 1;
        string out =string("/usr/local/bin/convert -delay 100 -loop 0 ");
        for (int k = 0; k < G.V() ; k+=n) {
            ofstream show;
            show.open("graph");
            show <<"Graph G{\ngraph [overlap=scale sep=3.5];\nnode [shape=circle];\n";
            for (int i = 0; i < G.V(); i++){
                typename Graph::adjiterrator A(G, i);
                for (int j = A.begin(); !A.end(); j = A.next())
                {
                    if (i <= j)
                    {
                        show << '\t' << i << " -- " << j;
                        if (v[i]<k && (v2[i]==j || v2[j]==i)) {
                            show <<" [penwidth=10.0]";
                        }
                        show << " ;" << endl;
                    }
                }
            }
            show << "}";
            show.close();
            output = base_string+ to_string(k)+".png";//base_string2+to_string(k)+base_string3;
            out += graphstr+to_string(k)+string(".png ");
            system(output.c_str());
            cout<< output<<endl;
        }
        
        out+= "BFSmyimage.gif && open BFSmyimage.gif";
        system(out.c_str());
        cout<< endl<< out;
    }
};


int main(int argc, const char * argv[]) {
    DenseGRAPH graph(13 ,1);
    //    randG(graph, 200);
    graph.insert(Edge(1,0));
    graph.insert(Edge(0,6));
    graph.insert(Edge(1,2));
    graph.insert(Edge(2,6));
    graph.insert(Edge(6,7));
    graph.insert(Edge(7,8));
    graph.insert(Edge(8,10));
    graph.insert(Edge(7,10));
    
    graph.insert(Edge(0,5));
    graph.insert(Edge(3,4));
    graph.insert(Edge(3,5));
    graph.insert(Edge(4,5));
    graph.insert(Edge(4,9));
    graph.insert(Edge(4,11));
    graph.insert(Edge(9,11));
    graph.insert(Edge(11,12));
//    BFS<DenseGRAPH> bfsgraph(graph,9);
//    vector<int> v = bfsgraph.getwhereV();
//    int ret = 8;
//    cout<<ret;
//    while (ret != 9) {
//        ret = v[ret];
//        cout <<"-"<<ret;
//    }
//    cout<<endl;
    
//    countBridges2<DenseGRAPH>bridgeGraph(graph);
//    cout<<bridgeGraph.bridges()<<endl;
//    bridgeGraph.print();
//    G2neato<DenseGRAPH> gra(graph);
//    gra.bidraw();
//    EC<DenseGRAPH> gra2(graph);
//    cout<<endl<<endl<<gra2.count()<<endl;
//    vector<int> v = DFS<DenseGRAPH>(graph).getusedV();
    //for_each(v.begin(), v.end(), [](int x) {cout << x << " "; });
//    pr_vec(DFS<DenseGRAPH>(graph).getusedV());
    //    cout<<path.exist()<<endl;
   // cout << countCompanents<DenseGRAPH>(graph).count()<<endl;
   // gra.graph();
//    PFS<DenseGRAPH> v(graph);
    neatoAnimation<DenseGRAPH> test(graph);
//    v.print();
//    test.graphAnimationDFS();
//    test.graphAnimationBFS();
    //vshortest<DenseGRAPH> vis(graph,8,9);
    //vis.draw();
////////////////////////
    DenseGRAPH graph2(13 ,0);
    //    randG(graph, 200);
//    graph2.insert(Edge(1,0));
//    graph2.insert(Edge(0,6));
//    graph2.insert(Edge(1,2));
//    graph2.insert(Edge(2,6));
//    graph2.insert(Edge(6,7));
//    graph2.insert(Edge(7,8));
//    graph2.insert(Edge(8,10));
//    graph2.insert(Edge(7,10));
//
//    graph2.insert(Edge(0,5));
//    graph2.insert(Edge(3,4));
//    graph2.insert(Edge(3,5));
//    graph2.insert(Edge(4,5));
//    graph2.insert(Edge(4,9));
//    graph2.insert(Edge(4,11));
//    graph2.insert(Edge(9,11));
//    graph2.insert(Edge(11,12));
    /////////////////////
    graph2.insert(Edge(0, 1));
    graph2.insert(Edge(0, 2));
    graph2.insert(Edge(0, 6));
    graph2.insert(Edge(0, 5));
    
    graph2.insert(Edge(2, 3));
    
    graph2.insert(Edge(3, 5));
    graph2.insert(Edge(3, 4));
    
    graph2.insert(Edge(4, 9));
    
    graph2.insert(Edge(6, 4));
    graph2.insert(Edge(6, 9));
    
    graph2.insert(Edge(7, 6));
    graph2.insert(Edge(8, 7));
    
    graph2.insert(Edge(9, 10));
    graph2.insert(Edge(9, 11));
    graph2.insert(Edge(9, 12));
    
    graph2.insert(Edge(11, 12));
    
    DenseGRAPH notDAG(13 ,0);
    
    notDAG.insert(Edge(0, 1));
    notDAG.insert(Edge(0, 5));
    notDAG.insert(Edge(0, 6));
    notDAG.insert(Edge(2, 0));
    notDAG.insert(Edge(2, 3));
    notDAG.insert(Edge(3, 2));
    notDAG.insert(Edge(3, 5));
    notDAG.insert(Edge(4, 2));
    notDAG.insert(Edge(4, 3));
    notDAG.insert(Edge(4, 11));
    
    notDAG.insert(Edge(5, 4));
    notDAG.insert(Edge(6, 4));
    notDAG.insert(Edge(6, 9));
    notDAG.insert(Edge(7, 6));
    notDAG.insert(Edge(7, 8));
    notDAG.insert(Edge(8, 7));
    notDAG.insert(Edge(8, 9));
    notDAG.insert(Edge(9, 10));
    notDAG.insert(Edge(9, 11));
    notDAG.insert(Edge(10, 12));
    notDAG.insert(Edge(11, 12));
    notDAG.insert(Edge(12, 9));
//    G2neato<DenseGRAPH> gra(graph2);
//    gra.graph();
//    tc_dfs<DenseGRAPH> dfs_graph(graph2);
//    G2neato<DenseGRAPH> dfs_gra(dfs_graph.get_graph());
   // dfs_gra.graph();
//    ts_draw<DenseGRAPH> ts(graph2);
//    ts.draw();
    sc<DenseGRAPH> scgraph(notDAG);
    vector<int> ids = scgraph.getId();
    for (int i = 0; i < ids.size(); i++) {
        cout << ids[i] <<endl;
    }
    //G2neato<DenseGRAPH> scgraph;
    return 0;
}
