
#include "shiroHashMap3.hpp"



void shiro_hashmap::init(int boardSize){
    list = new shiro_hashmap_Node*[bucket];
    for(int i = 0; i < bucket; ++i){
        list[i] = nullptr;
    }
    totalSize = bucket;
    entryBoardSize = boardSize;
    //h_num = ((entryBoardSize%8)==0) ? entryBoardSize/8 : (entryBoardSize/8)+1;
    //cout << pow(2,boardSize-(boardSize/3)*2) << endl;
}


bool shiro_hashmap::insert(unsigned long long int* d){
    hash = offset;
    /*for(int i=0 ; i<h_num ; ++i){
        hash *= prime;
        hash ^= (((*d) >> (8*i)) & 255);
        //hash *= prime;
    }*/
    hash *= prime;
    hash ^= ((*d) & 255);
    hash *= prime;
    hash ^= (((*d) >> (8)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (16)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (24)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (32)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (40)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (48)) & 255);
    key = list[hash%bucket];
    tmp = key;
    // xxxx&&&&******(50$)
    //  x : useless , * : player  ,  & : move , $ : box 
    if(!key){
        size++;
    }

    while(key){
        // board exists
        if (!(((*d)<<8)^(((key->data))<<8))) {
            return false;
        }
        key = key->next;
    }
    node = new shiro_hashmap_Node();
    node->data = (*d);
    node->next = tmp;
    list[hash%bucket] = node;
    node_num++;
    return true;
}

unsigned long long int shiro_hashmap::find(unsigned long long int* d){
    
    hash = offset;
    hash *= prime;
    hash ^= ((*d) & 255);
    hash *= prime;
    hash ^= (((*d) >> (8)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (16)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (24)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (32)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (40)) & 255);
    hash *= prime;
    hash ^= (((*d) >> (48)) & 255);
    key = list[hash%bucket];

    while(key){
        // board exists
        if (!(((*d)<<8)^(((key->data))<<8))) {
            return key->data;
        }
        key = key->next;
    }
    return 0ULL;
}
