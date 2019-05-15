/*
author:Xinyan He
date:2015-12-13
*/
#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED
enum squareType{BLOCK = 5};
enum colorType{BLACK, WHITE};

class Square
{
 public:
    int color()
    {
        return m_color;
    }

    virtual bool isStable(){ return false;}
    virtual bool setBulb(){return false;}
    virtual bool withDrawBulb(){return false;}
    virtual bool bulb(){return false;}
    virtual int light(){return 0;}
    virtual int isLimited(){return 0;}
    virtual void setLimit(){}
    virtual void unLimit(){}
    virtual void lightOn(){}
    virtual void lightOff(){}
    virtual bool haveVisited(){return false;}
    virtual void visit(){}
    virtual void unvisit(){}
    virtual bool isBlock(){ return false;}
    virtual int need(){return 0;}
    virtual int have(){return 0;}
    virtual void placeOne(){}
    virtual void withDrawOne(){}
   // int space(){return 0;}
   // void eliminateSpace(){};
protected:
       int m_color;
};

class WhiteSquare: public Square
{
public:
    WhiteSquare()
    {
        m_color = WHITE;
        m_light = 0;
        m_isLimited = 0;
        m_bulb = false;
        m_visit = false;
    }

    bool setBulb()
    {
        if(m_light || m_isLimited)
            return false;
        else
        {
          m_bulb = true;
          ++m_light;
        }
        return true;
    }

    bool withDrawBulb()
    {
        bool bres = false;
        if(m_bulb)
            m_bulb = false;
        if(m_light)
            --m_light;
        if(!m_bulb)
            bres = true;
        return bres;

    }
    bool haveVisited()
    {
        return m_visit;
    }

    void visit()
    {
        m_visit = true;
    }

    void unvisit()
    {
        m_visit = false;
    }

    bool bulb()
    {
        return m_bulb;
    }

    int light()
    {
        return m_light;
    };

    bool isStable()
    {
        if(m_light)
            return true;
        else
            return false;
    }

    int isLimited()
    {
        return m_isLimited;
    }

    void setLimit()
    {
        ++m_isLimited;
    }

    void unLimit()
    {
        if(m_isLimited)
            --m_isLimited;
    }
    //·øÉä¼ÆÊý¼Ó1
    void lightOn()
    {
        ++m_light;
    }
    //·øÉä¼ÆÊý¼õ1
    void lightOff()
    {
        if(m_light)
            --m_light;
    }

private:
    bool m_bulb;
    int m_light;
    int m_isLimited;
    bool m_visit;
};

class BlackSquare: public Square
{
public:
    int m_need;
    int m_have;//the number of bulbs have already been placed around
   // int m_space;//the number of space that currently left
    BlackSquare(int n)
    {
        m_color = BLACK;
        m_need = BLOCK;
        if(n <= 4 && n >=0)
        {
            m_need = n;
        }
        m_have = 0;
    }

    void withDrawOne()
    {
        if(m_have)
            --m_have;
    }

    void placeOne()
    {
        ++m_have;
    }

    bool isStable()
    {
        if(m_need == BLOCK || m_need == m_have)
            return true;
        else
            return false;
    }

    int need()
    {
        return m_need;
    }
    int have()
    {
        return m_have;
    }

    bool isBlock()
    {
        if(m_need == BLOCK)
            return true;
        else
            return false;
    }
};

#endif // SQUARE_H_INCLUDED
