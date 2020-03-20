#ifndef SHIRO_BOARD
#define SHIRO_BOARD

#include <string>

using namespace std;
#define N_MAX 15
#define M_MAX 15
#define AREA_MAX 50

enum _direction { UP, DOWN, LEFT, RIGHT};
typedef _direction DIRECTION;

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};


/*
move	push	pull
u 		U 		^
d 		D 		v
l		L		<
r		R		>
*/

class shiro_board{
    public:
        int n, m;
        char state[N_MAX+5][M_MAX+5];
        unsigned long long int px, py; // player
        unsigned long long int goal = 0;
        unsigned long long int box = 0;
        DIRECTION dir;
        bool push = false;
        bool pull = false;
        unsigned long long int x;
        unsigned long long int y;
        unsigned long long int lastPx;
        unsigned long long int lastPy;
        unsigned long long int nextPx; 
        unsigned long long int nextPy;
        unsigned long long int next2Px;
        unsigned long long int next2Py;

        // init
        bool createBoard();
        void construct();
        vector<shiro_board*>* possibleSoultion();
        
        //delete
        void deletePossibleSolution(vector<shiro_board*>*);


        //check
        bool solved();
        bool inBound(unsigned long long int*, unsigned long long int*);
        bool isBox(unsigned long long int);
        bool isWall(unsigned long long int*, unsigned long long int*);
        bool valid();
        string getResult(unsigned long long int* d);

        //serialized
        void combine(unsigned long long int*, unsigned long long int*, unsigned long long int*, unsigned long long int*, unsigned long long int*, unsigned long long int*);
        //void serialized(char*);
        //void reBuild(char*);
        void decode(unsigned long long int*);
        //move
        void doMovePlayer(unsigned long long int*, unsigned long long int*);
        void doMoveBox(unsigned long long int*, unsigned long long int*, unsigned long long int*, unsigned long long int*);
        bool move(char);

        //debug
        void printBoard();
};

#endif //SHIRO_BOARD

