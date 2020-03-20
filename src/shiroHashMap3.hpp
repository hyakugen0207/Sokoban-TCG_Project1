#ifndef HASH_MAP
#define HASH_MAP
#include <string>
#include <iostream>
#include<cmath>
using namespace std;

class shiro_hashmap_Node{
    public:
        unsigned long long int data;
        shiro_hashmap_Node* next;
    
    ~shiro_hashmap_Node(){
        if(next!=nullptr){
            delete next;
        }
    }
};


class shiro_hashmap{
    public:
        int size;
        int entryBoardSize;
        int totalSize;
        int h_num;
        int s_right;
        int node_num;
        unsigned int offset;
        unsigned int prime;
        unsigned int hash;
        int bucket;
        shiro_hashmap_Node** list;
        shiro_hashmap_Node* key;
        shiro_hashmap_Node* tmp;
        shiro_hashmap_Node* node;
        void init(int);
        bool insert(unsigned long long int*);
        unsigned long long int find(unsigned long long int*);

    shiro_hashmap(int s, int p){
        offset = 2166136261;
        prime = 16777619;
        bucket = pow(2,p);
        size = 0;
        totalSize = 0;
        node_num = 0;
        init(s);
    }

    ~shiro_hashmap(){
        for(int i = 0; i < bucket; ++i){
            delete list[i];
        }
        delete list;
    }
};

#endif //HASH_MAP