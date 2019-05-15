/*
author:Xinyan He
date:2015-12-13
*/
#ifndef CHESSBOARD_H_INCLUDED
#define CHESSBOARD_H_INCLUDED
#define MAX_LEN 100
#include "square.h"
#include <fstream>
using namespace std;
enum dir{LEFT, UP, RIGHT, DOWN};
class Chessboard
{
public:
    Square* ch[MAX_LEN][MAX_LEN];
    int blackIdx[MAX_LEN*MAX_LEN][2];
    int whiteOffIdx[MAX_LEN*MAX_LEN][2];
    int limitedIdx[MAX_LEN*MAX_LEN][2];
    int localWhiteOffIdx[MAX_LEN][2];
    int localLimitedIdx[MAX_LEN][2];
    int lnumWhiteOff;
    int lnumlimit;
    bool result;
    bool lresult;
    int numBlack;
    int numWhiteOff;
    int numlimit;
    int numRow;
    int numCol;
    Chessboard(const char* str, int n, int m)
    {
        lnumWhiteOff = 0;
        lnumlimit = 0;
        result = false;
        lresult = false;
        numBlack  = 0;
        numWhiteOff = 0;
        numlimit = 0;
        numRow = n;
        numCol = m;
        int temp;
        char c;

        fstream file(str, ios::in);
        for(int i = 0; i < numRow; ++i)
        {
            for(int j = 0; j < numCol; ++j)
            {
                file >> temp;
                if((temp >= 0 && temp <= 4) || (temp == BLOCK))
                 {
                     ch[i][j] = new BlackSquare(temp);
                     blackIdx[numBlack][0] = i;
                     blackIdx[numBlack][1] = j;
                     ++numBlack;
                 }
                else
                    ch[i][j] = new WhiteSquare;
            }
        }
    }

    void setLimited(int x, int y)
    {
        if(x >= 0 && y >= 0 && x < numRow && y < numCol)
            ch[x][y]->setLimit();
    }

    void unLimited(int x, int y)
    {
        if(x >= 0 && y >= 0 && x < numRow && y < numCol)
            ch[x][y]->unLimit();
    }

    bool withdrawBulb(int x,int y, int i)
    {
        bool bres = false;
        switch (i)
        {
        case LEFT:
            bres = withdrawBulb(x, y-1);
            break;
        case UP:
            bres = withdrawBulb(x-1, y);
            break;
        case RIGHT:
            bres = withdrawBulb(x, y+1);
            break;
        case DOWN:
            bres = withdrawBulb(x+1, y);
            break;
        }
        return bres;
    }

    bool setBulb(int x,int y, int i)
    {
        bool bres = false;
        switch (i)
        {
        case LEFT:
            bres = setBulb(x, y-1);
            break;
        case UP:
            bres = setBulb(x-1, y);
            break;
        case RIGHT:
            bres = setBulb(x, y+1);
            break;
        case DOWN:
            bres = setBulb(x+1, y);
            break;
        }
        return bres;
    }

    void unLimitAround(int x, int y)
    {
        unLimited(x+1, y);
        unLimited(x-1, y);
        unLimited(x, y+1);
        unLimited(x, y-1);
    }

    void setLimitedAround(int x, int y)
    {
        setLimited(x+1, y);
        setLimited(x-1, y);
        setLimited(x, y+1);
        setLimited(x, y-1);
    }

    bool checkBulb(int x, int y)
    {
        bool bres = false;
        if(x >= 0 && y >= 0 && x < numRow && y < numCol)
        {
            if(ch[x][y]->bulb())
                bres = true;
        }
        return bres;
    }

    int checkAround(int x, int y)
    {
        if(checkBulb(x, y-1))
            ch[x][y]->placeOne();
        if(checkBulb(x, y+1))
            ch[x][y]->placeOne();
        if(checkBulb(x+1, y))
            ch[x][y]->placeOne();
        if(checkBulb(x-1, y))
            ch[x][y]->placeOne();
        return ch[x][y]->have();
    }

    void restoreStateAround(int x, int y, int preHave)
    {
        if(x >= 0 && y >= 0 && x < numRow && y < numCol)
        {
            while(preHave)
            {
                ch[x][y]->withDrawOne();
                --preHave;
            }
        }
    }

    bool checkDebug()
    {
        bool b = false;
        if(ch[8][20]->bulb()&&ch[6][20]->bulb()&&ch[7][21]->bulb()&&ch[8][4]->bulb()&&ch[2][13]->bulb()&&ch[1][6]->bulb()&&ch[2][5]->bulb()&&ch[0][4]->bulb()&&ch[1][3]->bulb()&&ch[1][10]->bulb()&&ch[1][15]->bulb()&&ch[1][17]->bulb()&&ch[1][19]->bulb()&&ch[0][22]->bulb()&&ch[1][23]->bulb())
            b = true;
        return b;
    }

    void findAnswerForBlack(int idx, int i, int preHave = 0)
    {
        if(i < 4 && idx < numBlack)
        {
            int x =  blackIdx[idx][0];
            int y =  blackIdx[idx][1];
            if(i == 0) preHave = checkAround(x, y);
            if(i == 0 && ch[x][y]->have() == ch[x][y]->need())
            {
                if(ch[x][y]->need() < 4)
                    setLimitedAround(x, y);
                findAnswer(idx + 1);
            }
            else if(ch[x][y]->have() < ch[x][y]->need())
            {
                bool flag = false;
                int k = i;
                do
                {
                    if(result)
                        break;

                    if(setBulb(x, y, k))//黑格返回false，白格当有light和limit时失败，原本有bulb，由于有bulb的一个也会亮，有bulb了也会设不成功
                    {
                        flag = true;
                        ch[x][y]->placeOne();
                        findAnswerForBlack(idx, k+1, preHave);
                    }
                    if(ch[x][y]->have() == ch[x][y]->need())
                    {
                        if(ch[x][y]->need() < 4)
                            setLimitedAround(x, y);
                        findAnswer(idx + 1);
                    }
                    if(flag && !result)
                    {
                        if(ch[x][y]->have() == ch[x][y]->need())
                        {
                            if(ch[x][y]->need() < 4)
                                 unLimitAround(x, y);
                        }
                        if(!withdrawBulb(x, y, k))
                            break;
                        ch[x][y]->withDrawOne();
                        flag = false;
                    }
                    ++k;
                }while(k <= (4 - (ch[x][y]->need() - preHave) + i));
            }
            if(!result && i == 0)
             {
                 if(preHave == ch[x][y]->need())
                 {
                     if(ch[x][y]->need() < 4)
                        unLimitAround(x, y);
                 }
                 restoreStateAround(x, y, preHave);
             }

        }
    }

    void findAnswer(int idx)
    {
        if(idx < numBlack)
        {
            int x =  blackIdx[idx][0];
            int y =  blackIdx[idx][1];
            if(!ch[x][y]->isBlock())
            {
                if(ch[x][y]->need())
                {//1,2,3,4

                    findAnswerForBlack(idx, LEFT);

                }
                else
                {//0
                    setLimitedAround(x, y);
                    findAnswer(idx + 1);
                }
            }
            else
            {
                findAnswer(idx + 1);
            }
        }
        else
        {//the last blackSquare has been deal, finish the left white-OFF square
             if(!checkRules())
             {
                if(checkUnlimitAccessable())
                {
                    findWhiteOffAnswer(0);
                }
             }
             else
             {
                result = true;
             }
        }
    }

    void genLocalAreaAndFix(int idx)
    {
        if(idx < numWhiteOff)
        {
            int x = whiteOffIdx[idx][0];
            int y = whiteOffIdx[idx][1];
            if(!ch[x][y]->light())
            {
                lresult = false;
                lnumWhiteOff = 0;
                lnumlimit = 0;

                localWhiteOffIdx[lnumWhiteOff][0] = x;
                localWhiteOffIdx[lnumWhiteOff][1] = y;
                ++lnumWhiteOff;
                int k = 0;
                while(k < lnumWhiteOff)
                {
                   // cout<<"lunWhiteOff"<<lnumWhiteOff<<endl;
                    x = localWhiteOffIdx[k][0];
                    y = localWhiteOffIdx[k][1];
                    int j = x - 1;
                    while(j >= 0)
                    {
                        if(ch[j][y]->color() == BLACK)
                            break;
                        else if(!ch[j][y]->light() && !ch[j][y]->haveVisited())
                        {
                            ch[j][y]->visit();
                            if(!ch[j][y]->isLimited())
                            {
                                localWhiteOffIdx[lnumWhiteOff][0] = j;
                                localWhiteOffIdx[lnumWhiteOff][1] = y;
                                ++lnumWhiteOff;
                            }
                            else
                            {
                                localLimitedIdx[lnumlimit][0] = j;
                                localLimitedIdx[lnumlimit][1] = y;
                                ++lnumlimit;
                            }
                        }
                        --j;
                    }
                    j = x + 1;
                    while(j < numRow)
                    {
                        if(ch[j][y]->color() == BLACK)
                          break;
                        else if(!ch[j][y]->light() && !ch[j][y]->haveVisited())
                        {
                            ch[j][y]->visit();
                            if(!ch[j][y]->isLimited())
                            {
                                localWhiteOffIdx[lnumWhiteOff][0] = j;
                                localWhiteOffIdx[lnumWhiteOff][1] = y;
                                ++lnumWhiteOff;
                            }
                            else
                            {
                                localLimitedIdx[lnumlimit][0] = j;
                                localLimitedIdx[lnumlimit][1] = y;
                                ++lnumlimit;
                            }
                        }
                        ++j;
                    }
                    j = y - 1;
                    while(j >= 0)
                    {
                        if(ch[x][j]->color() == BLACK)
                          break;
                        else if(!ch[x][j]->light() && !ch[x][j]->haveVisited())
                        {
                            ch[x][j]->visit();
                            if(!ch[x][j]->isLimited())
                            {
                                localWhiteOffIdx[lnumWhiteOff][0] = x;
                                localWhiteOffIdx[lnumWhiteOff][1] = j;
                                ++lnumWhiteOff;
                            }
                            else
                            {
                                localLimitedIdx[lnumlimit][0] = x;
                                localLimitedIdx[lnumlimit][1] = j;
                                ++lnumlimit;
                            }
                        }
                        --j;
                    }
                    j = y + 1;
                    while(j < numCol)
                    {
                        if(ch[x][j]->color() == BLACK)
                           break;
                        else if(!ch[x][j]->light() && !ch[x][j]->haveVisited())
                        {
                            ch[x][j]->visit();
                            if(!ch[x][j]->isLimited())
                            {
                                localWhiteOffIdx[lnumWhiteOff][0] = x;
                                localWhiteOffIdx[lnumWhiteOff][1] = j;
                                ++lnumWhiteOff;
                            }
                            else
                            {
                                localLimitedIdx[lnumlimit][0] = x;
                                localLimitedIdx[lnumlimit][1] = j;
                                ++lnumlimit;
                            }
                        }
                        ++j;
                    }
                    ++k;
                }
                //区域生成完毕，对当前区域递归求解
                findLocalAnswer(0);
            }
        }

    }

    void findLocalAnswer(int idx)
    {
        if(idx < lnumWhiteOff)
        {
            bool flag = false;
            int x, y;
            for(int i = 0; i < lnumWhiteOff; ++i)
            {
                if(lresult)
                    break;
                x = localWhiteOffIdx[i][0];
                y = localWhiteOffIdx[i][1];

                if(setBulb(x, y))
                {
                    flag = true;
                    findLocalAnswer(idx + 1);

                }
                if(flag)
                {
                    lresult = true;
                    for(int i = 0; i < lnumlimit; ++i)
                    {
                        int x = localLimitedIdx[i][0];
                        int y = localLimitedIdx[i][1];
                        if(!ch[x][y]->light())
                            lresult = false;
                    }
                }
                if(flag && !lresult)
                {
                    withdrawBulb(x, y);
                    flag = false;
                }
            }
            if(!flag)
            {
                lresult = true;
                for(int i = 0; i < lnumlimit; ++i)
                {
                    int x = localLimitedIdx[i][0];
                    int y = localLimitedIdx[i][1];
                    if(!ch[x][y]->light())
                        lresult = false;
                }
            }
        }
    }

    void findWhiteOffAnswer(int idx)
    {
        result = true;
        for(int i = 0; i < numWhiteOff; ++i)
        {
            genLocalAreaAndFix(i);
            if(!lresult)
            {
                result = false;
                break;
            }
        }
        if(!result)
            unvisit();
    }

    void unvisit()
    {
        for(int i = 0; i < numWhiteOff; ++i)
        {
            ch[whiteOffIdx[i][0]][whiteOffIdx[i][1]]->unvisit();
        }
        for(int i = 0; i < numlimit; ++i)
        {
            ch[limitedIdx[i][0]][limitedIdx[i][1]]->unvisit();
        }
    }


//优先考虑被limited的grid，若果无法是这些grid亮，剪枝
    void findRestAnswer(int idx)
    {
        if(idx < numWhiteOff)
        {
            bool flag = false;
            int x, y;
            for(int i = 0; i < numWhiteOff; ++i)
            {
                if(result)
                    break;
                x = whiteOffIdx[i][0];
                y = whiteOffIdx[i][1];

                if(setBulb(x, y))
                {
                    flag = true;
                    findRestAnswer(idx + 1);
                }
                if(flag)
                {
                    result = true;
                    for(int i = 0; i < numlimit; ++i)
                    {
                        int x = limitedIdx[i][0];
                        int y = limitedIdx[i][1];
                        if(!ch[x][y]->light())
                            result = false;
                    }
                }
                if(flag && !result)
                {
                    withdrawBulb(x, y);
                    flag = false;
                }
            }
            if(!flag)
            {
                result = true;
                for(int i = 0; i < numlimit; ++i)
                {
                    int x = limitedIdx[i][0];
                    int y = limitedIdx[i][1];
                    if(!ch[x][y]->light())
                        result = false;
                }
            }

        }
    }

    //reset
    bool withdrawBulb(int x, int y)
    {
        bool bres = false;
        if(x >= 0 && y >= 0 && x < numRow && y < numCol)
        {
            if(ch[x][y]->withDrawBulb())
            {
                bres = true;
                int i = x + 1;
                while(i < numRow)
                {
                    if(ch[i][y]->color() == WHITE)
                        ch[i][y]->lightOff();
                    else
                        break;
                    ++i;
                }
                i = x - 1;
                while(i >= 0)
                {
                    if(ch[i][y]->color() == WHITE)
                        ch[i][y]->lightOff();
                    else
                        break;
                    --i;
                }
                i = y + 1;
                while(i < numCol)
                {
                    if(ch[x][i]->color() == WHITE)
                        ch[x][i]->lightOff();
                    else
                        break;
                    ++i;
                }
                i = y - 1;
                while(i >= 0)
                {
                    if(ch[x][i]->color() == WHITE)
                        ch[x][i]->lightOff();
                    else
                        break;
                    --i;
                }
            }
        }
        return bres;
    }

    bool setBulb(int x, int y)
    {
       bool bres = false;
       if(x >= 0 && y >= 0 && x < numRow && y < numCol)
        {
            if(ch[x][y]->setBulb())
            {
                bres = true;
                int i = x + 1;
                while(i < numRow)
                {
                    if(ch[i][y]->color() == WHITE)
                        ch[i][y]->lightOn();
                    else
                        break;
                    ++i;
                }
                i = x - 1;
                while(i >= 0)
                {
                    if(ch[i][y]->color() == WHITE)
                        ch[i][y]->lightOn();
                    else
                        break;
                    --i;
                }
                i = y + 1;
                while(i < numCol)
                {
                    if(ch[x][i]->color() == WHITE)
                        ch[x][i]->lightOn();
                    else
                        break;
                    ++i;
                }
                i = y - 1;
                while(i >= 0)
                {
                    if(ch[x][i]->color() == WHITE)
                        ch[x][i]->lightOn();
                    else
                        break;
                    --i;
                }
            }
        }
        return bres;
    }

    bool checkUnlimitAccessable()
    {
        int x,y,j;
        bool bRes = true;
        for(int i = 0; i < numlimit; ++i)
        {
            bool accessCol = false;
            bool accessRow = false;
            x = limitedIdx[i][0];
            y = limitedIdx[i][1];
            j = x - 1;
            while(j >= 0)
            {
                if(ch[j][y]->color() == BLACK)
                    break;
                else if(!ch[j][y]->light() && !ch[j][y]->isLimited())
                {
                    accessCol = true;
                    break;
                }
                --j;
            }
            j = x + 1;
            while(j < numRow)
            {
                if(ch[j][y]->color() == BLACK)
                    break;
                else if(!ch[j][y]->light() && !ch[j][y]->isLimited())
                {
                    accessCol = true;
                    break;
                }
                ++j;
            }
            j = y - 1;
            while(j >= 0)
            {
                if(ch[x][j]->color() == BLACK)
                    break;
                else if(!ch[x][j]->light() && !ch[x][j]->isLimited())
                {
                    accessRow = true;
                    break;
                }
                --j;
            }
            j = y + 1;
            while(j < numCol)
            {
                if(ch[x][j]->color() == BLACK)
                    break;
                if(!ch[x][j]->light() && !ch[x][j]->isLimited())
                {
                    accessRow = true;
                    break;
                }
                ++j;
            }
            if(!accessCol && !accessRow)
            {
                bRes = false;
                break;
            }
        }
        return bRes;
    }

    bool checkRules()
    {
        bool bRes = true;
        numWhiteOff = 0;
        numlimit = 0;
        for(int i = 0; i < numRow; ++i)
        {
            for(int j = 0; j < numCol; ++j)
            {
                if(!ch[i][j]->isStable())
                {
                    if(ch[i][j]->color() == WHITE)
                    {
                       if(ch[i][j]->isLimited())
                       {
                           limitedIdx[numlimit][0] = i;
                           limitedIdx[numlimit][1] = j;
                           ++numlimit;
                       }
                       else
                       {
                            whiteOffIdx[numWhiteOff][0] = i;
                            whiteOffIdx[numWhiteOff][1] = j;
                            ++numWhiteOff;
                       }
                    }
                    bRes = false;
                }
            }
        }
        return bRes;
    }

    void print()
    {
        for(int i = 0; i < numRow; ++i)
        {
            for(int j = 0; j < numCol; ++j)
            {
                if(ch[i][j]->color() == WHITE)
                    cout<<"- ";
                else
                    cout<<ch[i][j]->need()<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    void printChess()
    {
        for(int i = 0; i < numRow; ++i)
        {
            for(int j = 0; j < numCol; ++j)
            {
               if(ch[i][j]->color() == WHITE)
               {
                   if(ch[i][j]->bulb())
                       cout<<"@ ";
                   else
                   {
                        if(ch[i][j]->light())
                            cout<<"+ ";
                        else
                            cout<<"- ";
                   }
               }
               else
               {
                   if(ch[i][j]->isBlock())
                        cout<<"X ";
                    else
                    {
                        cout<<ch[i][j]->need()<<" ";
                    }
               }
            }
            cout<<endl;
        }
        cout<<endl;
    }

    void printChess2()
    {
         for(int i = 0; i < numRow; ++i)
        {
            for(int j = 0; j < numCol; ++j)
            {
               if(ch[i][j]->color() == WHITE && ch[i][j]->bulb())
                   cout<<"1 ";
               else
                    cout<<"0 ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    ~Chessboard()
    {
        for(int i = 0; i < numRow; ++i)
        {
            for(int j = 0; j < numCol; ++j)
            {
                delete ch[i][j];
            }
        }
    }
};

#endif // CHESSBOARD_H_INCLUDED
