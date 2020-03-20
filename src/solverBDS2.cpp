#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "shiroBoard2.hpp"
#include "shiroHashMap3.hpp"
#include "bfsQueue2.hpp"
using namespace std;

unsigned long long int dir = 15;
unsigned long long int one = 1;
unsigned long long int check;
vector<unsigned long long int> random_num; // vector for random
const bool Debug = false;
const string way = "udlrUvD^L>R<";

int wayConverter(int i){
    return (i%2==0) ? ++i : --i;
}

unsigned long long int compareSame(shiro_hashmap* maps, unsigned long long int* target){
    return maps->find(target);
}

unsigned long long int checkReachSolution(vector<shiro_hashmap*>* maps, int* index, unsigned long long int* data){
    // index == 0 compare with maps[1] (from goal)
    // index !=0 compare with maps[0] (from start point)
    if((*index)==0){
        check = ((*maps)[1])->find(data);
        if(check!=0ULL){
            return check;
        }
    }else{
        check = ((*maps)[0])->find(data);;
        if(check!=0ULL){
            return check;
        }
    }
    return 0ULL;
}

void deleteQueues(vector<bfs_queue*>* queues){
    int s = queues->size();
    for(int i = 0; i < s; ++i){
        delete (*queues)[i];
        queues->pop_back();
    }
}

void deleteHashMaps(vector<shiro_hashmap*>* maps){
    int s = maps->size();
    for(int i = 0; i < s; ++i){
        delete (*maps)[i];
        maps->pop_back();
    }
}

string getResult(unsigned long long int* d, shiro_board* b, shiro_hashmap* h, bool isPost){
    string result = "";
    //cout << "getresult" << endl;
    b->decode(d);
    unsigned long long int data = (*d);
    // todo: find hashmap2's previous move
    data = h->find(&data);

    while(((data<<4)>>60)!=15ULL){

        if(isPost){
            result.push_back(way[((((data<<4)>>60)%2==0)? ((data<<4)>>60)+1 : ((data<<4)>>60)-1)]);
        }else{
            result.push_back(way[((data<<4)>>60)]);
        }
        //recreate previous map
        b->move(way[((((data<<4)>>60)%2==0)? ((data<<4)>>60)+1 : ((data<<4)>>60)-1)]);

        b->combine(&data,&(b->box),&(b->px),&(b->py),&dir,&one);

        //find previous move
        data = h->find(&data);
    }
        
    return result;
}

int main(){

    shiro_board* board = new shiro_board();
    for(int i=0 ; i < way.size() ; ++i) random_num.push_back(i); 
    
    while(board->createBoard()){
        //init possible solutions and their queues / hashmaps
        auto possibleSolutionMap = board->possibleSoultion();
        
        
        vector<bfs_queue*> myQueues;
        vector<shiro_hashmap*> hashmaps;
        for(int i = 0; i < possibleSolutionMap->size()+1 ; ++i){
            myQueues.push_back(new bfs_queue());
        }
        hashmaps.push_back(new shiro_hashmap(board->m*board->n,21));
        hashmaps.push_back(new shiro_hashmap(board->m*board->n,24));
        //cout << "make queues and hashmaps" << endl;

        
        //init queue element
        unsigned long long int data;
        unsigned long long int hashData;
        possibleSolutionMap->insert(possibleSolutionMap->begin(),board);
        
        for(int i = 0; i < possibleSolutionMap->size() ; ++i){
            auto solutionBoard = (*possibleSolutionMap)[i];
            solutionBoard->combine(&hashData,&(solutionBoard->box),&(solutionBoard->px),&(solutionBoard->py),&dir,&one);
            if(i==0){
                hashmaps[0]->insert(&hashData);
            }else{
                hashmaps[1]->insert(&hashData);
            }
            myQueues[i]->push(&hashData);
        }


        bool isSolved = false;
        int depth = 1;
        int step = 1;
        unsigned long long int p = 0;
        unsigned long long int solution;
       
        while(!isSolved){
            for(int i = 0; i < myQueues.size(); ++i){
                //random_shuffle(random_num.begin(), random_num.end()); // random dir
                while(!myQueues[i]->isEmpty() && !isSolved){
                    data = myQueues[i]->pop();
                    (*possibleSolutionMap)[i]->decode(&data);

                    //check solution
                    if((solution = checkReachSolution(&hashmaps,&i,&data))!=0ULL){
                        //cout << "solved" << endl;
                        isSolved = true;
                        string ans1 = getResult(&data,(*possibleSolutionMap)[0],hashmaps[0],false);
                        reverse(ans1.begin(),ans1.end());
                        //cout << "ans1 : " << ans1 << endl; 
                        string ans2 = getResult(&data,(*possibleSolutionMap)[1],hashmaps[1],true);
                        
                        //cout << "ans2 : " << ans2 << endl; 
                        cout << ans1.size()+ans2.size()<<endl;
                        cout << ans1+ans2 << endl;
                        /*
                        //cout << "buket0 total :" << hashmaps[0]->totalSize <<endl;
                        //cout << "buket0 use :" << hashmaps[0]->size <<endl;
                        cout << "buket0 useRate :" << float(hashmaps[0]->size)/float(hashmaps[0]->totalSize) <<endl;
                        //cout << "buket1 total :" << hashmaps[1]->totalSize <<endl;
                        //cout << "buket1 use :" << hashmaps[1]->size <<endl;
                        cout << "buket1 useRate :" << float(hashmaps[1]->size)/float(hashmaps[1]->totalSize) <<endl;
                        //merge answer
                        int queue_size = 0;
                        int hash_size = 0;
                        for(int i = 0; i < myQueues.size(); ++i){
                            queue_size += myQueues[i]->size;
                            queue_size += myQueues[i]->poolSize;
                        }
                        
                        hash_size += hashmaps[0]->node_num;
                        hash_size += hashmaps[1]->node_num;

                        cout << "queue_size : " << queue_size << endl;
                        cout << "hash_size : " << hash_size << endl; 
                        */
                        break;
                    }

                    


                    for(int d = 0; d < way.size(); ++d){ //diff act to try
                        (*possibleSolutionMap)[i]->decode(&data);
                
                        if((*possibleSolutionMap)[i]->move(way[random_num[d]])){ // if the move is legal
                            (*possibleSolutionMap)[i]->combine(&hashData,&((*possibleSolutionMap)[i]->box),&((*possibleSolutionMap)[i]->px),&((*possibleSolutionMap)[i]->py),&(random_num[d]),&p);
                            if(i==0){
                                if(hashmaps[0]->insert(&hashData)){ // if this map doesn't meet before.
                                    myQueues[i]->push(&hashData);
                                }  
                            }else{
                                if(hashmaps[1]->insert(&hashData)){ // if this map doesn't meet before.
                                    myQueues[i]->push(&hashData);
                                }  
                            }        
                        }
                    }
                    if(p!=(data>>60)){ // if reach depth
                        break;
                    }
                }   
                
            }
            //cout << depth << endl;
            p = (p+1)%2;
            depth += step;     
        }
        deleteQueues(&myQueues);
        deleteHashMaps(&hashmaps);
    }
    return 0;
}