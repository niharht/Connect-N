// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

class HumanPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
    bool determineBestMove( Scaffold& m, int N, int color, bool self, int level, int& bestMove, int& bestScore);
    
private:
    Timer t;              ///having timer for smartplayer to check if over 10 seconds
    bool startwrappingup();       ///checking if time is up
    int numcalls=0;
    bool wrap=false;        //still has time
};
bool completed(const Scaffold& s, int N,int& winner)//needed completed function for Smart player
{
    for(int i=1; i<=s.cols();i++){   //going through scaffold cols
        int blackcount=0, redcount=0;//will count blacks and reds
        int level=1;                 //start on first level of each column
        while(level<=s.levels())       //checking within column
        {
            if(s.checkerAt(i, level)==RED){
                blackcount=0;          //if red, then blackcount gets reset
                redcount++;
                if(redcount==N){     //if reaches goal then set winner to red and return
                    winner=RED;
                    return true;
                }
                
            }
            else if(s.checkerAt(i, level)==BLACK){          //same as for red, doing for black
                blackcount++;
                redcount=0;
                if(blackcount==N){
                    winner=BLACK;
                    return true;
                }
                
            }
            else{                 ///if neither red or black rest counter and break
                redcount=0;
                blackcount=0;
                break;
            }
            
            level++;
        }
    }
    for(int i=1;i<=s.levels();i++){           //checking within levels
        int blackcount=0, redcount=0;           //also using blackcount and redcount for each level
        for(int j=1; j<=s.cols();j++){
            if(s.checkerAt(j, i)==RED){         //same idea as for columns, except traversing through levels
                blackcount=0;
                redcount++;
                if(redcount==N){
                    winner=RED;
                    return true;
                }
                
            }
            else if(s.checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==N){
                    winner=BLACK;
                    return true;
                }
                
            }
            else{
                redcount=0;             //if not found all in a row, resetting count until sees entire column
                blackcount=0;
            }
            
        }
    }
    //checking diagonally left bottom to top right
    for(int k=1; k<=s.levels();k++){                     //as going through each level
        int blackcount=0, redcount=0;
        for( int j=1, i=k; (j<=s.cols()&& i<=s.levels()); i++, j++){        //checking at that column and level but increasing each by one to check diagonally
            if(s.checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==N){
                    winner=RED;
                    return true;
                }
                
            }
            else if(s.checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==N){
                    winner=BLACK;
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
            
        }
    }
    for(int k=2; k<=s.cols();k++){
        int blackcount=0, redcount=0;
        for( int j=k, i=1; (j<=s.cols()&& i<=s.levels()); i++, j++){ //checking one column to the right above and diagonal
            if(s.checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==N){
                    winner=RED;
                    return true;
                }
                
            }
            else if(s.checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==N){
                    winner=BLACK;
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
        }
    }
    //checking left top to right bottom
    for(int k=s.levels(); k>=1;k--){
        int blackcount=0, redcount=0;
        for( int j=1, i=k; (j<=s.cols()&& i>=1); i--, j++){      //checking from top left and then incremeting column, decrementing level
            if(s.checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==N){
                    winner=RED;
                    return true;
                }
                
            }
            else if(s.checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==N){
                    winner=BLACK;
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
        }
    }
    for(int k=2; k<=s.cols();k++){                  //checking next column over
        int blackcount=0, redcount=0;
        for( int j=k, i=s.levels(); (j<=s.cols()&& i>=1); i--, j++){
            if(s.checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==N){
                    winner=RED;
                    return true;
                }
                
            }
            else if(s.checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==N){
                    winner=BLACK;
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
        }
    }
    
    
    if(s.numberEmpty()==0){        //tie game only if scaffold full and no winner
        winner=TIE_GAME;
        return true;
    }
    return false;
}


bool SmartPlayerImpl::startwrappingup(){
    if(wrap == true) {
        return true;
    }
    this->numcalls++;
    if(numcalls%1000==0){         //checking every 1000
        if(t.elapsed()>=5000){      //if time elapsed is greater than 5000 milliseconds
            wrap = true;
            return wrap;
        }
    }
    return false;
}


int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty()==0){           //if full scaffold then can't play
        return -1;
    }
    int column;
    do {
        cout<<"Please enter a valid column."<<endl;
        cin>>column;
    } while (column>s.cols()|| column<1 || s.checkerAt(column, s.levels())!=VACANT);      //ensuring column valid before inserting piece
    
    
    return column;  //  This is not always correct; it's just here to compile
}


int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)       //just function for finding bugs, goes to left most available position
{
    if(s.numberEmpty()==0){
        return -1;
    }
    for(int i=1; i<=s.cols();i++){
        if(s.checkerAt(i, s.levels())==VACANT){
            return i;
        }
    }
    return -1;
}


bool SmartPlayerImpl::determineBestMove( Scaffold& m, int N, int color, bool self, int level, int& bestMove, int& bestScore){
    bestScore=0;
    bestMove=-1;
    int alternateMove=-1;
    int result=-1;
    if(this->startwrappingup()==true){
        return false;                //checking if time is up
    }
    int currentcolor=color;
    if(!self){
        if(color==BLACK){
            currentcolor=RED;           //checking if smart player or no using parameter self
        }
        else if(color==RED){
            currentcolor=BLACK;
        }
    }
    bool scoreinitalized=false;

    //iterate through all possible moves
    // in loop, make move and check if win
    //create list and as iterating through
    //go through moves set best move each time there is a move with a higher score
    //
    //check timer by calling and seeing if less than 10, check after every 10 calls
    
    for(int i=1; i<=m.cols();i++){
        int score=0;
        int move=-1;
        
        if(m.makeMove(i, currentcolor)==true){
            if(level==1){               //if in level 1, time is up, then will give alternte move as most recent colum
                alternateMove=i;
            }

            if(completed(m, N, result)==true ){         //checking if game completed
                move=i;
                if(result==currentcolor&& self==true){        //if winning and checking which player it is that is winning
                    score=1000-level;
                    }
        
                else if(result==currentcolor &&self==false){
                    score=-(1000-level);
                } else{
                    score=0;
                }

                
            } else{
                this->determineBestMove(m, N,color, !self, level+1, move,score);//recursive call by moving over one
            }
            m.undoMove();
            
            if(scoreinitalized==false ){        //if no best score or best move to do yet, then saying score is initialized in order to compare other scores
                bestScore=score;
                bestMove=move;
                scoreinitalized=true;
                
            } else if(self && bestScore<=score){            //setting best score to be ranking of score for that move
                bestScore=score;
                bestMove=move;
            } else if(!self && bestScore>=score){        //checking if score will lead to better move
                    bestScore=score;
                    bestMove=move;
                
            }
            if(level==1){
                //cout<<"Score is "<<score<<" and best score is"<<bestScore<<endl;
                //cout<<"Move is "<<move<<" and best move is"<<bestMove<<endl;
            }

        }
    }
 
    if(bestMove==-1){       //ran out of time and no best move, will give most recent
        bestMove=alternateMove;
    }
    if(bestMove==-1){         //if still no alternate move
        return false;
    }
    
    return true;
}


int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    t.start();         //starting timer and not finished
    wrap=false;
    if(completed(s, N, color)==true){         //checkig if game is completed
        cout<<"completed"<<endl;
        return -1;
    }
    
    int winningscore=0;
    int winningmove=0; ///will be the number of the column for the move
    //go through all of the columns
    //using make move check if move is possible
    //if possible check chooseMove for opponent
    Scaffold m=s;         //can't pass const, using copy constructor
    if(determineBestMove(m, N, color, true, 1, winningmove, winningscore)){
        return winningmove;
    }
    else{
        
    }
    return -1;  //  This is not always correct; it's just here to compile
}

//******************** Player derived class functions *************************

// These functions simply delegate to the Impl classes' functions.
// You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
: Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
: Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
: Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
