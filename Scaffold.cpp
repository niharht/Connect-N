// Scaffold.cpp
#include <vector>
#include "provided.h"
#include <iostream>
#include <stack>
using namespace std;



class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const;
    void display() const;
    bool makeMove(int column, int color);
    int undoMove();
private:
    int n_Columns;
    int n_Levels;
    vector<vector<int>> grid;
    stack<int> recentcolumn;
    stack<int> recentlevel;
    
    
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    if(nColumns <1 || nLevels<1){
        cerr<<"Cannot have negative columns or levels."<<endl;     //ensuring scaffold created is valid
        return;
    }
    n_Columns=nColumns;
    n_Levels=nLevels;
    grid.resize(nColumns);  // grid now has M empty rows         //creating a 2D vector
    for (int i = 0; i < nColumns; i++)
        grid[i].resize(nLevels);  // row i now has N columns
    
    // Now we have an M by N "array" of doubles.
    // We can say, for example,
    
    for (int i = 0; i < nColumns; i++)
        for (int j = 0; j < nLevels; j++)
            grid[i][j] = VACANT;                         //making every position vacant
    
}

int ScaffoldImpl::cols() const
{
    return n_Columns;
}

int ScaffoldImpl::levels() const
{
    return n_Levels ;
}

int ScaffoldImpl::numberEmpty() const
{
    int count=0;
    for(int i=0; i<n_Columns;i++){
        for( int j=0; j<n_Levels;j++){
            if(grid[i][j]==VACANT){
                count++;              //counting number of positions that are vacant
            }
        }
    }
    return count;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    if(column<1|| level<1){
        cerr<<"Cannot have less than 1 column or level"<<endl;
        exit(1);
    }                                                               //ensuring valid column and level are passed
    if(column>n_Columns||level>n_Levels){
        cerr<<"The column or level requested does not exist"<<endl;
        exit(1);
    }
    if(grid[column-1][level-1]==RED){
        return RED;
    }
    else if(grid[column-1][level-1]==BLACK){
        return BLACK;
    }
    return VACANT;  //  This is not always correct; it's just here to compile
}

void ScaffoldImpl::display() const
{
    
    for(int i=n_Levels-1; i>=0;i--){           //since vector is created so that position 1,1 on scaffold is index 0,0 in vector have to print in reverse to print top first then rest of scaffold
        for( int j=0;j<n_Columns;j++){
            cout<<"|";
            if(grid[j][i]==RED){
                cout<<"R";
            }
            else if(grid[j][i]==BLACK){
                cout<<"B";
            }
            else
                cout<<" ";
        }
        cout<<"|"<<endl;
    }
    for(int i=0; i<n_Columns;i++){
        cout<<"+-";
    }
    cout<<"+"<<endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if( column>n_Columns || column<1){             //ensuring valid column
        return false;
    }
    for(int i=0;i<n_Levels; i++){
        if(grid[column-1][i]!=BLACK && grid[column-1][i]!=RED){      //traversing through column to find empty spot and set spot to color
            grid[column-1][i]=color;
            recentcolumn.push(column);
            recentlevel.push(i+1);
            return true;
        }
    }
    return false;
}

int ScaffoldImpl::undoMove()
{
    if(recentlevel.size()!=0 &&recentcolumn.size()!=0){           //using stacks to push most recent column and level from make move and popping here to set that position in 2d vector to vacant
        int column=recentcolumn.top();
        recentcolumn.pop();
        int level=recentlevel.top();
        recentlevel.pop();
        grid[column-1][level-1]=VACANT;
        return column;
    }
    
    return 0;  //  This is not always correct; it's just here to compile
}
/*int main(){
 Scaffold s(3, 2);
 s.makeMove(2, RED);
 s.makeMove(1, BLACK);
 s.makeMove(1, RED);
 s.display();
 //  |R| | |
 //  |B|R| |
 //  +-+-+-+
 s.undoMove();
 s.display();
 //  | | | |
 //  |B|R| |
 //  +-+-+-+
 s.undoMove();
 s.display();
 //  | | | |
 //  | |R| |
 //  +-+-+-+
 
 }*/

//******************** Scaffold functions *******************************

// These functions simply delegate to ScaffoldImpl's functions.
// You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}

