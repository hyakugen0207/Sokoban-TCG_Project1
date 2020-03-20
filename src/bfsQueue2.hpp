#ifndef BFS_NODE
#define BFS_NODE

class bfs_node{
    public:
        unsigned long long int playerMoveBox;
        // xxx=&&&&******(50$)  
        //  = : parity bit  * : player  ,  & : move , $ : box
        bfs_node *next;
        
        //constructor
        bfs_node(): playerMoveBox(9), next(0){};
        bfs_node(unsigned long long int pmb): next(0){
            playerMoveBox = pmb;
        };
};

#endif //BFS_NODE




#ifndef BFS_QUEUE
#define BFS_QUEUE

    using namespace std;
    
    #ifndef BFS_QUEUE_PARAMETER
    #define BFS_QUEUE_PARAMETER

    //缺判斷poool會不會過大
    const int MAX_POOL_SIZE = 10000;
    const bool DEBUGGING = false;
    
    #endif //BFS_QUEUE_PARAMETER

/*
 *    head                       tail
 *      |                         |
 *      v                         v
 *      node -> node -> node -> node 
 */ 

class bfs_queue{
    public :
        bfs_node *head;
        bfs_node *tail;
        bfs_node *pool;
        bfs_node *tmp;
        int poolSize; // for analysis
        int size; // for analysis
        unsigned long long int data;
        //constructor
        bfs_queue():head(0), tail(0), pool(0), tmp(0), poolSize(0), size(0){
        };

        ~bfs_queue(){

            while(head!=nullptr){
                tmp = head;
                head = head->next;
                delete tmp;
            }
            tmp = pool;
            while(pool!=nullptr){
                tmp = pool;
                pool = pool->next;
                delete tmp;
            }
        }

        void push(unsigned long long int*);
        bool isEmpty();
        unsigned long long int pop();
        void test();
};


bool bfs_queue::isEmpty(){
    return (head==nullptr);
}



void bfs_queue::push(unsigned long long int* data){

    if(head==nullptr){
       
        if(pool!=nullptr){ //get node from pool
            
            //if(DEBUGGING) cout << "empty and get node from pool" << endl;
            pool->playerMoveBox = (*data);
            head = pool;
            tail = head;
            
            
            if(pool->next != nullptr){
                pool = pool->next;
            }else{
                pool = nullptr;
            }
            head->next = nullptr;
            tail->next = nullptr;
            poolSize--;
        }else{
            //if(DEBUGGING) cout << "empty and new node" << endl;
            head = new bfs_node((*data));
            tail = head;
            head->next = tail->next = nullptr;
            size++;
        }
        return;
    }
    if(pool!=nullptr){
        //if(DEBUGGING) cout << "get node from pool" << endl;
        tail->next = pool;
        pool->playerMoveBox = (*data);
        tmp = pool;
        if(pool->next != nullptr){
            pool = pool->next;
        }else{
            pool = nullptr;
        }
        tmp->next = nullptr;
        tail = tmp; 
        poolSize--;
        size++;
    }else{
        //if(DEBUGGING) cout << "new node" << endl;
        tmp = new bfs_node((*data));
        //if(tmp->road==""){
        //cout << "push empty string2" << r << endl;}
        tail->next = tmp;
        tmp -> next = nullptr;
        tail = tmp;  
        size++;     
    }
    return;
}


unsigned long long int bfs_queue::pop(){
    //if(head==nullptr){
    //    cout << "Is Empty!" << endl;
    //}
    data = head->playerMoveBox;
    tmp = head;
    head = head->next;
    if(pool==nullptr){
        //tmp->inPool = true;
        pool = tmp;
        pool->next = nullptr;
        size--;
        poolSize++;
        //if(DEBUGGING) cout << "pool empty, put node into pool!" << pool <<endl;
    }else{
        //tmp->inPool = true;
        tmp->next = pool;
        pool = tmp;
        size--;
        poolSize++;
        //if(DEBUGGING) cout << "put node into pool" << pool <<endl;
    }
    /*else{
        //tmp->inPool = false;
        //if(DEBUGGING)
        cout << "reach MAX_POOL_SIZE delete the node" << tmp->playerMoveBox <<endl;
        //cout << "full" << endl;
        delete tmp;
    }*/
    return data;
}
/*
template <typename T, typename K>
void bfs_queue<T,K>::test(){
    if(DEBUGGING) cout << "in test" << endl;
    tmp = head;
    for(int i = 0 ; i < size ; ++i){
        cout << tmp->road << " " << tmp->current_map << endl;
        tmp = tmp->next;
    }
    return;
}
*/


#endif // BFS_QUEUE

