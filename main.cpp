// main.cpp

#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;

void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Game g(2, 2, 2, &bp1, &bp2);
    int winner=0;
    assert(!g.completed(winner));
    g.takeTurn();  // Red's first move
    assert(!g.completed(winner)  &&
           (g.checkerAt(1, 1) == RED  ||  g.checkerAt(2, 1) == RED));
    g.takeTurn();  // Black's first move
    assert(!g.completed(winner));
    g.takeTurn();  // Red's second move; Red must win
    assert(g.completed(winner)  &&  winner == RED);
}

/*int main()
{
    doGameTests();
    cout << "Passed all tests" << endl;
}*/

int main()
{
    /*Scaffold s(3,4);
    s.display();
    cout<<s.cols()<<endl;    //should print out 3
    cout<<s.levels()<<endl;   //should print out 4
    s.makeMove(3, BLACK);
    cout<<s.checkerAt(3, 1)<<endl;   //should rpint out 1 which is black
    s.undoMove();
    cout<<s.checkerAt(3, 1)<<endl;//should print out -1 vacant
    cout<<s.numberEmpty()<<endl; //should print out 12 since all spaces are vacant
    string name;*/
    
    HumanPlayer hp("Nihar");
    SmartPlayer sp("Computer");
    Game g1(3, 3, 3, &sp, &hp);  //try on smaller game to test reaction time and speed
    int winner=-1;
    g1.play();
    cout<<g1.completed(winner)<<endl; //should print out true since game  completed after play
    cout<<g1.takeTurn()<<endl;// will print out false after game ends
    g1.play();
    cout << endl << "This time I'll go first." << endl;
    Game g2(4, 4, 3, &sp, &hp); ///larger game will take longer time to respond but will be within 10 seconds. Play against to see winner. 
    g2.play();
}
