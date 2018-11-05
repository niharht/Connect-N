// Game.cpp

#include "provided.h"
#include <stack>
using namespace std;
#include <string>
#include <iostream>
class GameImpl
{
public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    bool completed(int& winner) const;
    bool takeTurn();
    void play();
    int checkerAt(int c, int r) const;
private:
    int m_columns;
    int m_levels;
    Scaffold* m_game;
    Player* m_red;
    Player* m_black;
    int m_goal;
    stack <int> players;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)    //constructing game with scaffold, columns, levels, red player, black player, and N how many needed in a row to win
{
    m_game= new Scaffold(nColumns, nLevels);
    m_columns=nColumns;
    m_levels=nLevels;
    m_red=red;
    m_black=black;
    m_goal=N;
    players.push(RED);  ///red player starts first
    
}

bool GameImpl::completed(int& winner) const
{
    for(int i=1; i<=m_columns;i++){       //checking if column has a winner by traversing through each column with a redcount and blackcount
        int blackcount=0, redcount=0;
        int level=1;
        while(level<=m_levels)
        {
            if(m_game->checkerAt(i, level)==RED){
                blackcount=0;             //if red found, then blackcount goes to 0
                redcount++;
                if(redcount==m_goal){
                    winner=RED;
                    m_game->display();        //once turn taken, display scaffold
                    return true;
                }
                
            }
            else if(m_game->checkerAt(i, level)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==m_goal){
                    winner=BLACK;
                    m_game->display();
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
                break;
            }
            
            level++;
        }
    }
    for(int i=1; i<=m_levels;i++){            //checking levels by traversing through each level to see if enough found in a row
        int blackcount=0, redcount=0;
        for(int j=1; j<=m_columns;j++){
            if(m_game->checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==m_goal){
                    winner=RED;
                    m_game->display();
                    return true;
                }
                
            }
            else if(m_game->checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==m_goal){
                    winner=BLACK;
                    m_game->display();
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
            
        }
    }
    
    //checking diagonally left bottom to top right
    for(int k=1; k<=m_levels;k++){
        int blackcount=0, redcount=0;
        for( int j=1, i=k; (j<=m_columns&& i<=m_levels); i++, j++){    //have to increment both level and row by one to move up and to the right
            if(m_game->checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==m_goal){
                    winner=RED;
                    m_game->display();
                    return true;
                }
                
            }
            else if(m_game->checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==m_goal){
                    winner=BLACK;
                    m_game->display();
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
            
        }
    }
    for(int k=2; k<=m_columns;k++){
        int blackcount=0, redcount=0;
        for( int j=k, i=1; (j<=m_columns&& i<=m_levels); i++, j++){     //checking from second position up and to the right diagonal
            if(m_game->checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==m_goal){
                    winner=RED;
                    m_game->display();
                    return true;
                }
                
            }
            else if(m_game->checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==m_goal){
                    winner=BLACK;
                    m_game->display();
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
    for(int k=m_levels; k>=1;k--){                   //checking from top left and moving down by shiring both level and column by 1 from above diagonal
        int blackcount=0, redcount=0;
        for( int j=1, i=k; (j<=m_columns&& i>=1); i--, j++){
            if(m_game->checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==m_goal){
                    winner=RED;
                    m_game->display();
                    return true;
                }
                
            }
            else if(m_game->checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==m_goal){
                    winner=BLACK;
                    m_game->display();
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
        }
    }
    for(int k=2; k<=m_columns;k++){           //starting at just to right of the diagoanl and checking up going right diagonally
        int blackcount=0, redcount=0;
        for( int j=k, i=m_levels; (j<=m_columns&& i>=1); i--, j++){
            if(m_game->checkerAt(j, i)==RED){
                blackcount=0;
                redcount++;
                if(redcount==m_goal){
                    winner=RED;
                    m_game->display();
                    return true;
                }
                
            }
            else if(m_game->checkerAt(j, i)==BLACK){
                blackcount++;
                redcount=0;
                if(blackcount==m_goal){
                    winner=BLACK;
                    m_game->display();
                    return true;
                }
                
            }
            else{
                redcount=0;
                blackcount=0;
            }
        }
    }
    
    
    if(m_game->numberEmpty()==0){
        winner=TIE_GAME;
        return true;
    }
    
    return false;
}

bool GameImpl::takeTurn()
{
    int winner=-1;
    if(m_game->numberEmpty()==0){        //if not empty and no winner then will take turn
        return false;
    }
    if(this->completed(winner)==true){
        return false;
    }
    if(players.top()==RED){                        //if red player next to player popping, choosing move, checking winner and pushing black player
        players.pop();
        int n =m_red->chooseMove(*m_game, m_goal,RED);
        m_game->makeMove(n, RED);
        if(this->completed(winner)==true){
            return false;
        }
        m_game->display();
        players.push(BLACK);
        return true;
    }
    if(players.top()==BLACK){      //if black player next to player popping, choosing move, checking winner and pushing red player
        players.pop();
        int n=m_black->chooseMove(*m_game, m_goal, BLACK);
        m_game->makeMove(n , BLACK);
        if(this->completed(winner)==true){
            return false;
        }
        m_game->display();
        players.push(RED);
        return true;
    }
    return true;  
}

void GameImpl::play()
{
    int winner=-1;
    m_game->display();          //displaying empty scaffold
    while(this->completed(winner)==false){
        this->takeTurn();         //while no winner, playing
    }
    if(winner==BLACK){       
        cout<<"The winner is black!"<<endl;
    }
    else if(winner==RED){
        cout<<"The winner is red!"<<endl;
    }
    else if(winner==TIE_GAME){
        cout<<"The game is a tie!"<<endl;
    }
    
}

int GameImpl::checkerAt(int c, int r) const
{
    return m_game->checkerAt(c, r);
}

//******************** Game functions *******************************

// These functions simply delegate to GameImpl's functions.
// You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
    delete m_impl;
}

bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
