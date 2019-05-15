/*
构建2-3-4树
从一个数据文件中依次读入关键字序列构建树，测试建树时间；//建树ko
测试遍历产生顺序输出序列需要的时间；//中序遍历ko
测试构建完成树后删除指定 关键字集合 的时间//搜索ko、删除
*/
#include <iostream>
#include <fstream>
#include <stdlib>
#include <time.h>
using namespace std;
class Node
{
    public:
        int keys[3];
        Node* child[4]; 
        int numOfKeys;
        int numOfChild;
        Node()
        {
            numOfKeys = 0;
            numOfChild = 0;
        } 
        
}

class SimpBtree
{
  public:
    SimpBtree(){ root = NULL;};
    void treeCreate();
    void treeSearch(Node* x, int key, Node* &y, int &keyIdx);
    void treeSplitChild(Node* x, int i ,Node* y);
    void treeInsert(SimpBtree tree, int key);
    void treeInsertNonfull(Node* x, int key);
    void treeDelete(Node* x, int key);
    void treeTravel(Node* x);
    Node*& root(){return root};
    bool leaf(Node* x){ return 0 == x->numOfChild;}
  private:
    Node* root;   
}

void SimpBtree::treeTravel(Node* x)
{
    for(int i = 0; i < x->numOfKeys; ++i)
    {
       if(!leaf(x))
            treeTravel(x->child[i]);
       cout<<x->keys[i];
    }
    if(!leaf(x))
        treeTravel(x->child[x->numOfChild - 1]);    
}
void SimpBtree::treeSearch(Node* x, int key, Node* &y, int &keyIdx)
{
    int i = 0;
    while(i < x->numOfKeys && key > x->keys[i]) 
    {
        ++i;
    }
    if (i < x->numOfKeys; && key == x->keys[i])
    {
        y = x;
        keyIdx = i;
    }    
    if (leaf(x))
    {
        y = NULL;
        keyIdx = -1; 
    }
    else
        treeSearch(x->child[i], key, y, keyIdx);  
}

void SimpBtree::treeCreate()
{
    root = new Node;
}

void SimpBtree::treeSplitChild(Node* x, int i, Node* y)
{
    Node* z = new Node;
    z->numOfKeys = 1;//t-1,here t=2.
    int j = 0;
    z->keys[j] = y->keys[j+2];//j+t
    z->numOfKeys++;
    y->numOfKeys--;
    if (!leaf(y))
    {
        for(j = 0; j<2; ++j)
        {
            z->child[j] = y->child[j+2];//j+t
            z->numOfChild++;
            y->numOfChild--;
        }
     }  
     for(j = x->numOfKeys - 1; j > i; --j)
        x->child[j+1] = x->child[j];
    x->child[i+1] = z;
    x->numOfChild++;
    for(j = x->numOfKeys - 1; j > i-1; --j)
      x->keys[j+1] = x->keys[j];
    x->keys[i] = y->keys[2];
    y->numOfKeys--;
    x->numOfKeys++;  
}

void SimpBtree::treeInsert(SimpBtree tree, int key)
{
    Node* r = tree.root();
    if(r->numOfKeys == 3)//2t-1
    {
        Node* s = new Node;
        tree.root() = s;
        s->child[0] = r;
        s->numOfChild++;
        treeSplitChild(s, 0, r);
        treeInsertNonfull(s, key);
    }
    else
        treeInsertNonfull(r, key);   
}

void SimpBtree::treeInsertNonfull(Node* x, int key)
{
    int i = x->numOfKeys;
    if(leaf(x))
    {
        while(i >= 0 && key < x->keys[i])
        {
            x->keys[i+1] = x->keys[i];
            --i;
        }
        x->keys[i+1] = key;
        x->numOfKeys++;
    }
    else
    {
        while(i >= 0 && k < x->keys[i])
            --i;
        ++i;
        if((x->child[i])->numOfKeys == 3)
        {
            treeSplitChild(x, i, x->child[i]);
            if(key > x->keys[i])
                ++i;
        }
        treeInsertNonfull(x->child[i], key);           
    }
}

void SimpBtree::treeDelete(Node* x, int key)
{
    int i = 0;
    if(!x)
        return;
    while(i < x->numOfKeys && key > x->keys[i])
        ++i;
    //(1)
    if(i < x->numOfKeys && key == x->keys[i])
    {
        if(leaf(x))
        {
            for(int j = i;  j < x->numOfKeys - 1; ++j)
                x->keys[j] = x->keys[j+1];
            x->numOfKeys--;   
        }//(2)
        else
        {   //a)
           if(x->child[i]->numOfKeys >= 2)
           {
               Node* tmp = x->child[i];
               while(!leaf(tmp))
               {
                   tmp = tmp->child[tmp->numOfChild - 1];
               }
               x->keys[i] = tmp->keys[tmp->numOfKeys - 1];
               treeDelete(tmp, tmp->keys[tmp->numOfKeys - 1]);
           }//b)
           else if(x->child[i+1]->numOfKeys >= 2)
           {
               Node* tmp = x->child[i+1];
               while(!leaf(tmp))
               {
                   tmp = tmp->child[0];
               }
               x->keys[i] = tmp->keys[0];
               treeDelete(tmp, tmp->keys[0]);
           }//c)
           else
           {
               Node* y = x->child[i];
               Node* z = x->child[i+1];
               y->keys[y->numOfKeys] = key;
               y->numOfKeys++;
               for(int j = 0; i < z->numOfKeys; ++j)
               {
                    y->keys[y->numOfKeys+j] = z->keys[j];
                    y->numOfKeys++;
               }   
               for(int j = 0; j < z->numOfChild; ++z)
               {
                    y->child[y->numOfChild+j] = z->child[j];
                    y->child++
               }
               for(int j = i; j < x->numOfKeys-1; ++j)
               {
                   x->keys[j] = x->keys[j+1];
               }
               for(int j = i+1; j < x->numOfChild-1; ++j)
               {
                   x->child[j] = x->child[j+1];
               }
               x->child[x->numOfChild-1] = NULL;
               x->numOfChild--;
               x->numOfKeys--;
               delete z;
               treeDelete(y, key);
           }
        }
    }//(3)
    else
    {
        if(leaf(x))
            return;
        Node* c = x->child[i];
        Node* l = NULL;
        if(i-1 >= 0)
            l = x->child[i-1];
        Node* r = NULL;
        if(i < x->numOfChild)
            r = x->child[i+1];
            
        if(c->numOfKeys <= 1)
        {   //a)
            if(l && l->numOfKeys >= 2)
            {
                for(int j = c->numOfKeys; j > 0; --j)
                    c->keys[j] = c->keys[j-1];
                c->keys[0] = x->keys[i-1];
                c->numOfKeys++;
                x->keys[i-1] = l->keys[l->numOfKeys - 1];
                l->numOfKeys--;
                for(int j = c->numOfChild; j > 0; --j)
                    c->child[j] = c->child[j-1];
                l->child[l->numOfChild - 1] = NULL;
                c->child[0] = l->child[l->numOfChild - 1];
                c->numOfChild++;
                l->numOfChild--;
            }
            else if(r && r->numOfKeys >= 2)
            {
                c->keys[c->numOfKeys] = x->keys[i];
                c->numOfKeys++;
                x->keys[i] = r->keys[0];
                for(int j = 0; j < r->numOfKeys - 1; ++j)
                    r->keys[j] = r->keys[j+1]
                r->numOfKeys--;
                c->child[c->numOfChild] = r->child[0];
                c->numOfChild++;
                for(int j = 0; j < r->numOfChild - 1; ++j)
                    r->keys[j] = r->keys[j+1];
                r->child[l->numOfChild - 1] = NULL;
                r->numOfChild--;
            }//b)
            else
            {
                if(l)
                {
                    l->keys[l->numOfKeys] = x->keys[i-1];
                    l->numOfKeys++;
                    for(int j = i-1; j < x->numOfKeys - 1; ++j)
                        x->keys[j] = x->keys[j+1];
                    x->numOfKeys--;
                    for(int j = 0; j < c->numOfKeys; ++j)
                    {
                        l->keys[l->numOfKeys] = c->keys[j];
                        l->numOfKeys++;
                    }
                    for(int j = 0; j < c->numOfChild; ++j)
                    {
                        l->child[l->numOfChild] = c->child[j];
                        l->numOfChild++;
                    }    
                    for(int j = i; j < x->numOfChild-1; ++j)
                        x->child[j] = x->child[j+1];
                    x->child[x->numOfChild - 1] = NULL;
                    x->numOfChild--;
                    delete c;
                    c = l;
                }
                else if(r)
                {
                    c->keys[c->numOfKeys] = x->keys[i];
                    c->numOfKeys++;
                    for(int j = i; j < x->numOfKeys - 1; ++j)
                        x->keys[j] = x->keys[j+1];
                    x->numOfKeys--;
                    for(int j = 0; j < r->numOfKeys; ++j)
                    {
                        c->keys[c->numOfKeys] = r->keys[j];
                        c->numOfKeys++;
                    }
                    for(int j = 0; j < r->numOfChild; ++j)
                    {
                        c->child[c->numOfChild] = r->child[j];
                        c->numOfChild++;
                    }
                    for(int j = i+1; j < x->numOfChild - 1; ++j)
                        x->child[j] = x->child[j+1];
                    x->child[x->numOfChild - 1] = NULL;
                    x->numOfChild--;
                    delete r;
                }
            }
        }
        treeDelete(c, key);
    }
                
             
        
}


int main()
{
    float t1,t2;
    SimpBtree tree;
    tree.treeCreate();
    fstream file("test.txt");
    int key;
    t1 = clock();
    while(file >> key)
        tree.treeInsert(tree, key);
    t2 = clock();
    cout<<"build time : "<<t2 - t1 <<"ms";
    t1 = clock();
    tree.treeTravel(tree.root);
    t2 = clock();
    cout<<"traversal :"<<t2 - t1<<"ms";
    file.close();
    file.open("delete.txt");
    t1 = clock();
    while(file >> key)
        tree.treeDelete(tree.root, key);
    t2 = clock();
    cout<<"delete time:"<<t2-t1<<"ms";
    file.close();
    return 0;
}
