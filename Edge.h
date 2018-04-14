//
//  Edge.h
//  graph2
//
//  Created by Владимир Еганов on 18.03.2018.
//  Copyright © 2018 Ульяна Сыроегина. All rights reserved.
//

#ifndef Edge_h
#define Edge_h
struct Edge{
    //private:
    int first, second;
    //public:
    Edge(int start=-1,int finish=-1){
        first = start;
        second = finish;
    }
    bool operator== (const Edge& e )const{
        return (this->first==e.first)&&(this->second==e.second);
    }
};
struct hash_edge{
    size_t operator() (const Edge& e )const{
        return std::hash<int> ()(e.first+e.second);
    }
};

#endif /* Edge_h */
