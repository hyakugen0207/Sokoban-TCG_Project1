#include <cstdio>
#include <cassert>
#include <iostream>
#include <vector>
#include "shiroBoard2.hpp"




bool shiro_board::createBoard(){
    if (fscanf(stdin, "%d%d", &(n), &(m)) != 2) {
        
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        fscanf(stdin, "%s", state[i]);
    }
    construct();
    return 1;
}

void shiro_board::construct(){
    assert(n<=N_MAX and m<=M_MAX and n*m<=AREA_MAX);
        goal = 0ULL;
        box = 0ULL;
        int count = 0;
		for ( int i=0; i<n; ++i ) {
			for ( int j=0; j<m; ++j ) {
				if ( state[i][j]=='@') {
					px = i;
                    py = j;
                }else if(state[i][j]=='+'){
                    px = i;
                    py = j;
                    goal |= (1ULL << count);
                }else if(state[i][j]=='$'){
                    box |= (1ULL << count);
                }else if(state[i][j]=='*'){
                    box |= (1ULL << count);
                    goal |= (1ULL << count);
                }else if(state[i][j]=='.'){
                    goal |= (1ULL << count);
                }
                ++count;
			}
		}
}


void shiro_board::combine(unsigned long long int* result, unsigned long long int* box, unsigned long long int* px, unsigned long long int* py, unsigned long long int* dir, unsigned long long int* p){
    (*result) = 0ULL;
    *result |= *box;
    *result |= (((*px)*m+(*py)) << 50); 
    *result |= ((*dir) << 56);
    *result |= ((((*p)+1)%2) << 60);
}

void shiro_board::decode(unsigned long long int* data){
    box = (((*data) << 14) >> 14);
    px = (((*data) << 8) >> 58) / m;
    py = (((*data) << 8) >> 58) % m;
}

bool shiro_board::solved(){
    return !(box ^ goal);
}

bool shiro_board::isBox(unsigned long long int x){
    return (box & x);
}

bool shiro_board::inBound(unsigned long long int* x, unsigned long long int* y){
    return (((*x) >= 0) && ((*x) < n) && ((*y) >=0) && ((*y) < m));
}

bool shiro_board::isWall(unsigned long long int* x, unsigned long long int* y){
    return (state[(*x)][(*y)]=='#');
}

void shiro_board::printBoard() {
    cout << "box" << endl;
    for (int i = 0; i < n*m ; ++i) {
        cout << bool(box & (1ULL << i));
        if((i+1)%m==0){
            cout << endl;
        }
    }
    cout << "player : ";
    cout << px << "," << py << endl;

    cout << "ori" << endl;
    for (int i = 0; i < n; ++i) {
        fprintf(stdout, "%s\n", state[i]);
    }
    return;
}



bool shiro_board::valid(){


    //if out of bound or go into a wall, return fasle
    if(!inBound(&nextPx, &nextPy) || isWall(&nextPx, &nextPy)) {
        return false;
    }


    //if just move but dest have a box, return false
    if(!push && !pull){ 
        if(isBox((1ULL<<(nextPx*m+nextPy)))) {
            return false;
        }
    }


    //push
    if(push){
        next2Px = nextPx+dx[dir];
        next2Py = nextPy+dy[dir];
        //push but there is no box
        if(!isBox((1ULL<<(nextPx*m+nextPy)))){
            return false;
        }

        //push but box will out of bound or go into a wall or there is another box, return false
        if(!inBound(&next2Px, &next2Py) || isWall(&next2Px, &next2Py) || isBox((1ULL<<(next2Px*m+next2Py)))){
            return false;
        }
    }

    //pull
    if(pull){
        lastPx = px-dx[dir];
        lastPy = py-dy[dir];

        if(!inBound(&lastPx, &lastPy) || isWall(&lastPx, &lastPy)) {
            return false;
        }
        //pull but there is no box
        if(!isBox((1ULL<<(lastPx*m+lastPy)))){
            return false;
        }

        //pull but there is a box in front of the player
        if(isBox((1ULL<<(nextPx*m+nextPy)))){
            return false;
        }
    }

    return true;
}

void shiro_board::doMovePlayer(unsigned long long int* x, unsigned long long int* y){
    //player ^= (1ULL << (px*n+py));
    //player |= (1ULL << (x*n+y));
    px=(*x);
    py=(*y);
}

void shiro_board::doMoveBox(unsigned long long int* oldX, unsigned long long int* oldY, unsigned long long int* newX, unsigned long long int* newY){  
    box ^= (1ULL << ((*oldX)*m+(*oldY)));
    box |= (1ULL << ((*newX)*m+(*newY)));
}


bool shiro_board::move(char move){
    //cout << "move:" << move << endl; 
    if (move == 'u') {
        push = pull = false;
        dir = UP;
    }
    else if (move == 'd') {
        push = pull = false;
        dir = DOWN;
    }
    else if (move == 'l') {
        push = pull = false;
        dir = LEFT;
    }
    else if (move == 'r') {
        push = pull = false;
        dir = RIGHT;
    }
    else if (move == 'U') {
        pull = false;
        dir = UP;
        push = true;
    }
    else if (move == 'D') {
        dir = DOWN;
        push = true;
        pull = false;
    }
    else if (move == 'L') {
        dir = LEFT;
        push = true;
        pull = false;
    }
    else if (move == 'R') {
        dir = RIGHT;
        push = true;
        pull = false;
    }
    else if (move == '^') {
        dir = UP;
        pull = true;
        push = false;
    }
    else if (move == 'v') {
        dir = DOWN;
        pull = true;
        push = false;
    }
    else if (move == '<') {
        dir = LEFT;
        pull = true;
        push = false;
    }
    else if (move == '>') {
        dir = RIGHT;
        pull = true;
        push = false;
    }
    else {
        fprintf(stdout, "illegal step character\n");
        return false;
    }

    //update parameters
    nextPx = px+dx[dir]; 
    nextPy = py+dy[dir];

    if(valid()){
        //doMove
        if(push){
            //doMoveBox(&nextPx, &nextPy, &next2Px, &next2Py);
            //doMovePlayer(&nextPx, &nextPy);
            box ^= (1ULL << (nextPx*m+nextPy));
            box |= (1ULL << (next2Px*m+next2Py));
            px = nextPx;
            py = nextPy;
        }else if(pull){
            x = px;
            y = py;
            //doMovePlayer(&nextPx, &nextPy);
            px = nextPx;
            py = nextPy;
            box ^= (1ULL << (lastPx*m+lastPy));
            box |= (1ULL << (x*m+y));
            //doMoveBox(&lastPx, &lastPy, &x, &y);
        }else{
            //doMovePlayer(&nextPx, &nextPy);
            px = nextPx;
            py = nextPy;
        }

        return true;
    }
    //cout << "invalid move" << endl;
    return false;
}


vector<shiro_board*>* shiro_board::possibleSoultion(){
    char possibleGoal[N_MAX+5][M_MAX+5];
    vector<int> p;
    //cout << "in 1" << endl;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(state[i][j]=='@' || state[i][j]=='$'){
                possibleGoal[i][j] = '-';
            }else if(state[i][j] == '.' || state[i][j] == '+'){
                possibleGoal[i][j] = '*';
            }else if(state[i][j] == '*'){
                possibleGoal[i][j] = '*';
            }else if(state[i][j] == '#'){
                possibleGoal[i][j] = '#';
            }else{
                possibleGoal[i][j] = '-';
            }
        }
    }

    for(unsigned long long int i =0; i < n; ++i){
        for(unsigned long long int j = 0; j < m; ++j){
            if(possibleGoal[i][j]=='*'){
                for(unsigned long long int ii = i-1; ii < i+2; ii+=2){
                    if(inBound(&ii,&j)){
                        if(possibleGoal[ii][j]=='-'){
                            p.push_back(ii);
                            p.push_back(j);
                            possibleGoal[ii][j] = '?';
                        }
                    }
                    
                }
                for(unsigned long long int jj = j-1; jj < j+2; jj+=2){
                    if(inBound(&i,&jj)){
                        if(possibleGoal[i][jj]=='-'){
                            p.push_back(i);
                            p.push_back(jj);
                            possibleGoal[i][jj] = '?';
                        }
                    }
                }       
            }
        }
    }
    for(unsigned long long int i =0; i < n; ++i){
        for(unsigned long long int j = 0; j < m; ++j){
            if(possibleGoal[i][j]=='?'){
                possibleGoal[i][j]='-';
            }
        }
    }

    vector<shiro_board*>* possibleBoard = new vector<shiro_board*>();
    for(auto b = p.begin(); b != p.end() ; b+=2){
        //build board
        shiro_board* newBoard = new shiro_board();
        //set size
        newBoard->n = n;
        newBoard->m = m;
        //copy possible solution
        for ( int i=0; i<newBoard->n; ++i ) {
			for ( int j=0; j<newBoard->m; ++j ) {
				newBoard->state[i][j] = possibleGoal[i][j];
			}
		}
        //put player
        newBoard->px = *b;
        newBoard->py = *(b+1);
        newBoard->state[newBoard->px][newBoard->py]='@';
        newBoard->construct();

        //push_back
        possibleBoard->push_back(newBoard);
    }
    return possibleBoard;
}
        
        //delete
void shiro_board::deletePossibleSolution(vector<shiro_board*>* v){
    for(auto i = v->begin(); i < v->end(); ++i){
        delete *i;
    }
    delete v;
    return;
}

