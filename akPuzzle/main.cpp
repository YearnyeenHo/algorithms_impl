/*
author:Xinyan He
date:2015-12-13
*/
#include <iostream>
#include "chessboard.h"
#include "square.h"
#include <time.h>
using namespace std;



int main()
{
    int n;
    Chessboard c("data2.txt", 14, 24);
    c.print();
    float t1 = clock();
    c.findAnswer(0);
    float t2 = clock();
    c.printChess();
    c.printChess2();
    cout<<"total time: "<<t2-t1<<"ms"<<endl;
    return 0;
}
