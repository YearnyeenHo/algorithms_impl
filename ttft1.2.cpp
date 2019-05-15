/*
构建2-3-4树

从一个数据文件中依次读入关键字序列构建树，测试建树时间；//建树ko
测试遍历产生顺序输出序列需要的时间；//中序遍历ko
测试构建完成树后删除指定 关键字集合 的时间//搜索ko、删除
*/

#include <iostream>
#include <fstream>
using namespace std;

template <class Key, class value_type>
 struct btPair
 {
    Key m_key;
    value_type m_value;

    btPair()
    {
    }

    btPair(const Key& key, const value_type& v)
    {
        m_key = key;
        m_value = v;
    }

    Key& key()
    {
        return m_key;
    }

    value_type& value()
    {
        return m_value;
    }

    void print()
    {
        cout << "tv " << m_key << endl;
    }

    btPair<Key, value_type>& operator=(btPair<Key, value_type> pa)
    {
        m_key = pa.m_key;
        m_value = pa.m_value;
        return *this;
    }
 };

template <class Val>//btPair
struct Node
{
    Val m_content;//当Val是指针类型时，析构deleteNode指针之前应该先delete m_content这个指针
    Node* m_next;
    Node* m_pre;

    Node()
    {
        m_next = 0;
        m_pre = 0;
    }

    Node(const Val& v)
    {
        m_content = v;
        m_next = 0;
        m_pre = 0;
    }
};

template<class Val>
struct ArrList
{
    Node<Val>* m_header;
    Node<Val>* m_cur;
    int m_curIdx;
    int m_num;
    ArrList()
    {
        m_header = new Node<Val>;
        m_cur = 0;
        m_num = 0;
        m_curIdx = 0;
    }
//+++++++++  push_back  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void push_back(const Val& v)
    {
        Node<Val>* pNode = new Node<Val>(v);
        push_back(pNode);
    }

    void push_back(Node<Val>* pNode)
    {
        if(pNode != 0)
        {
            if(0 == first())
            {
                first() = pNode;
                last() = pNode;
                pNode->m_pre = 0;
                pNode->m_next = 0;
            }
            else
            {
                pNode->m_pre = last();
                last()->m_next = pNode;
                last() = pNode;
                pNode->m_next = 0;
            }
            ++m_num;
            m_cur = first();
            m_curIdx = 0;
        }
        else
        {
            cout<<"insert node empty!!!"<<endl;
        }
    }

    void push_back(Node<Val>* pNode1, Node<Val>* pNode2, int num)
    {
        if(pNode1 != 0 && pNode2 != 0)
        {
            if(0 == first())
            {
                first() = pNode1;
                last() = pNode2;
                pNode1->m_pre = 0;
                pNode2->m_next = 0;
            }
            else
            {
                pNode1->m_pre = last();
                last()->m_next = pNode1;
                last() = pNode2;
                pNode2->m_next = 0;
            }
            m_num += num;
            m_cur = first();
            m_curIdx = 0;
        }
        else
        {
            cout<<"insert node empty!!!"<<endl;
        }
    }
//+++++++++++  insert ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void insert(const Val& v, int pos)
    {
        Node<Val>* pNode = new Node<Val>(v);
        insert(pNode, pos);
    }

    void insert(Node<Val>* pNode, int pos)
    {
        if(pos < m_num && pos >= 0)
        {
            Node<Val>* p = (*this)[pos];
            if(p)
            {
                if(p == first())
                    first() = pNode;
                else
                    p->m_pre->m_next = pNode;

                pNode->m_pre = p->m_pre;
                pNode->m_next = p;
                p->m_pre = pNode;
                m_cur = first();
                m_curIdx = 0;
                ++m_num;
            }
        }
        else
        {
            push_back(pNode);
        }
    }

    void insert(Node<Val>* pNode1, Node<Val>* pNode2, int pos, int num)
    {
        if(pos < m_num && pos >= 0)
        {
            Node<Val>* p = (*this)[pos];
            if(p)
            {
                if(p == first())
                    first() = pNode1;
                else
                    p->m_pre->m_next = pNode1;

                pNode1->m_pre = p->m_pre;
                pNode2->m_next = p;
                p->m_pre = pNode2;

                m_cur = first();
                m_curIdx = 0;
                m_num += num;
            }
        }
        else
        {
            push_back(pNode1, pNode2, num);
        }
    }

//++++++++++++++++++++   remove   ++++++++++++++++++++++++++++++++++++++++++++++++++
    void conectForRemove(Node<Val>* pNode)
    {
        if(pNode != 0)
        {
            Node<Val>* t = pNode->m_pre,* t1=0,* t2=0;
            if(pNode->m_pre)
            {
                pNode->m_pre->m_next = pNode->m_next;
            }
            else
            {
                first() = pNode->m_next;
                if(first())
                    first()->m_pre = 0;
            }

            if(pNode->m_next)
            {
                pNode->m_next->m_pre = pNode->m_pre;
            }
            else
            {
                last() = pNode->m_pre;
                if(last())
                    last()->m_next = 0;
            }
        }
        m_cur = first();
        m_curIdx = 0;
    }

    void conectForRemove(Node<Val>* pNode1, Node<Val>* pNode2)
    {
        if(pNode1 != 0 && pNode2 != 0)
        {
            if(pNode1->m_pre)
            {
                pNode1->m_pre->m_next = pNode2->m_next;
            }
            else
                first() = pNode2->m_next;
            if(pNode2->m_next)
            {
                pNode2->m_next->m_pre = pNode1->m_pre;
            }
            else
                last() = pNode1->m_pre;
        }
        m_cur = first();
        m_curIdx = 0;
    }

    Node<Val>* remove(int pos1)
    {
        Node<Val>* pNode  = (*this)[pos1];
        conectForRemove(pNode);
        --m_num;
        return pNode;
    }

    void remove(int pos1, int pos2, Node<Val>*& p1, Node<Val>*& p2)
    {
        p1  = (*this)[pos1];
        p2  = (*this)[pos2];
        conectForRemove(p1, p2);
        m_num = m_num - ((pos2 - pos1) + 1);
        p1->m_pre = 0;
        p2->m_next = 0;
    }

//++++++++++++++++++++++++ transfer ++++++++++++++++++++++++++++++++++++++

    void transfer(int pos1, ArrList<Val>& list, int pos2)
    {
        if(pos1 < m_num && list.size() >= pos2)
        {
            Node<Val>* pNode = remove(pos1);
            list.insert(pNode, pos2);
        }
    }

    void rangeTransfer(int pos1, int pos2, ArrList<Val>& list, int pos3)
    {
        if(pos1 <= pos2 && pos2 < m_num && pos3 <= list.size())
        {
            Node<Val>* p1 = 0, *p2 = 0;
            remove(pos1, pos2, p1, p2);
            int num = pos2 - pos1 + 1;
            list.insert(p1, p2, pos3, num);
        }
    }
//+++++++++++++++++++++++++++++  other  +++++++++++++++++++++++++++++++++++++++++++++
    int size()
    {
        return m_num;
    }

    Node<Val>*& first()
    {//the first node
        return m_header->m_pre;
    }

    Node<Val>*& last()
    {//the last node
        return m_header->m_next;
    }

    Node<Val>* incre()
    {
        if(m_cur != 0 && m_curIdx < m_num -1)
        {
            m_cur = m_cur->m_next;
            ++m_curIdx;
        }
        return m_cur;
    }

    Node<Val>* decre()
    {
        if(m_cur != 0 && m_curIdx > 0)
        {
            m_cur = m_cur->m_pre;
            --m_curIdx;
        }
        return m_cur;
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Node<Val>*& operator[](int idx)
    {
        Node<Val>* pRes = 0;
        if(idx < m_num && idx >= 0)
        {
            if(idx < m_curIdx)
            {
                while(m_curIdx > idx)
                    decre();
            }
            else if(idx > m_curIdx)
            {
                while(m_curIdx < idx)
                    incre();
            }
            pRes = m_cur;
        }
        return pRes;
    }

    void erase(Node<Val>* pNode)
    {
        conectForRemove(pNode);
        delete pNode;
        m_cur = first();
        m_curIdx = 0;
        --m_num;
        cout<<"wakaka"<<endl;
    }

    ~ArrList()
    {
        while(first() != 0)
        {
            erase(first());
        }
        erase(m_header);
    }
};

template<class Val>
struct Child;

//*************************************************************************************************************************************
template<class Val>//btPair
struct bNode
{
    ArrList<Val> m_keysArr;
    ArrList< Child<Val> > m_childsArr;
    bool isLeaf;

    bNode()
    {
        isLeaf = true;
    }

    int childNum()
    {
        return (int)m_childsArr.size();
    }

    int keyNum()
    {
        return (int)m_keysArr.size();
    }

    void InsertKey(const Val& k, int pos)
    {
        m_keysArr.insert(k, pos);
    }

    void InsertChild(bNode<Val>*& c, int pos)
    {
        m_childsArr.insert(c, pos);
        isLeaf = false;
    }

    void childsTransfer(int pos1, int pos2, bNode<Val>* x, int pos3)
    {
        m_childsArr.rangeTransfer(pos1, pos2, x->m_childsArr, pos3);
    }

    void keysTransfer(int pos1, int pos2, bNode<Val>* x, int pos3)
    {
        m_keysArr.rangeTransfer(pos1, pos2, x->m_keysArr, pos3);
    }

    void childTransfer(int pos1, bNode<Val>* x, int pos2)
    {
        m_childsArr.transfer(pos1, x->m_childsArr, pos2);
    }

    void keyTransfer(int pos1, bNode<Val>* x, int pos2)
    {
        m_keysArr.transfer(pos1, x->m_keysArr, pos2);
    }
};

template<class Val>
struct Child
{
    bNode<Val>* m_pointer;
    int* counter;

    Child()
    {
        m_pointer = 0;
        counter = new int(0);
    }

    Child(bNode<Val>* p)
    {
        m_pointer = p;
        counter = new int(0);
    }

    Child(Child<Val>& ch)
    {
        m_pointer = ch.m_pointer;
        counter = ch.counter;
        ++(*counter);
    }

    Child& operator=(const Child<Val>& ch)
    {
        m_pointer = ch.m_pointer;
        counter = ch.counter;
        ++(*counter);
        return *this;
    }

    void delChildPointer()
    {
        delete m_pointer;
    }

    ~Child()
    {
        if(!counter)
        {
            delChildPointer();
            delete counter;
        }
        --(*counter);
        //delete m_pointer;
    }
};

template < class Key, class value_type >
 class bTree
{
public:
    typedef btPair<Key, value_type> Val;
    int t;
    bNode<Val>* m_root;

    void __DISK_WRITE(bNode<Val>* p)
    {

    }
    void __DISK_READ(bNode<Val>* p)
    {

    }
    bTree(int n)
    {
        t = n;
        bTreeCreate();
    }

    void bTreeCreate()
    {
        m_root = new bNode<Val>;
        __DISK_WRITE(m_root);
    }

    bNode<Val>*& root()
    {
        return m_root;
    }

public:
    void bTreeSplitChild(bNode<Val>*& x, int i, bNode<Val>*& y)
    {//为bTreeInsert保证递归始终不会降至一个满节点上
        bNode<Val>* z = new bNode<Val>;
        z->isLeaf = y->isLeaf;
        y->keysTransfer(t, 2*t-2, z, 0);
        if(!z->isLeaf)
        {
            y->childsTransfer(t, 2*t-1, z, 0);//从第t+1个child开始到第2t个child
        }

        x->InsertChild(z, i + 1);
        y->keyTransfer(t-1, x, i);

        __DISK_WRITE(x);
        __DISK_WRITE(y);
        __DISK_WRITE(z);
    }

    void bTreeInsert(Val& v)
    {
        bTreeInsert(root(), v);
    }

    void bTreeInsert(bNode<Val>*& root, Val& v)
    {

        bNode<Val>* r = root, *s;

        if(root->m_keysArr.size() == 2*t-1)
        {
            s = new bNode<Val>;
            s->InsertChild(root,0);
            root = s;
            //s->isLeaf = false;
            bTreeSplitChild(s, 0, r);
            bTreeNonFull(s, v);
        }
        else
        {
            bTreeNonFull(r, v);
        }
    }

    void bTreeNonFull(bNode<Val>*& x, Val& v)
    {
        int idx = x->m_keysArr.size() - 1;
        if(x->isLeaf)
        {
            while(idx >=0 && idx <= (2*t-2) && x->m_keysArr[idx]->m_content.key() > v.key())
            {
                --idx;
            }
            x->InsertKey(v, idx + 1);
            __DISK_WRITE(x);
        }
        else
        {
            while(idx >= 0 && idx <= (2*t-2) && (x->m_keysArr[idx]->m_content).key() > v.key())
            {
                --idx;
            }
            ++idx;
            bNode<Val>* c = (x->m_childsArr[idx]->m_content).m_pointer;
            __DISK_READ(c);
            if(c->m_keysArr.size() == 2*t-1)
            {
                bTreeSplitChild(x, idx, c);
                Node<Val>* t = x->m_keysArr[idx];
                if(v.key() > (x->m_keysArr[idx]->m_content).key())
                {
                    ++idx;
                }
                c  = (x->m_childsArr[idx]->m_content).m_pointer;
            }
            bTreeNonFull(c, v);
        }
    }

    void traversal()
    {
        int sz = root()->m_childsArr.size();
        inoderTraversal(root());
    }

    void inoderTraversal(bNode<Val>* p)
    {
        if(!p->isLeaf)
        {
            int i = 0;
            int sz = p->m_keysArr.size();
            for(; i < p->m_keysArr.size(); ++i)
            {
                inoderTraversal(p->m_childsArr[i]->m_content.m_pointer);
                p->m_keysArr[i]->m_content.print();
            }
            inoderTraversal(p->m_childsArr[i]->m_content.m_pointer);
        }
        else
        {
            for(int i = 0; i < p->m_keysArr.size(); ++i)
            {
                p->m_keysArr[i]->m_content.print();
            }
        }
    }

    void bTreeSearch(bNode<Val>* p, const Key& k, bNode<Val>*& pRes, int& idx)
    {
        int i = 0;
        while(i < p->m_keysArr.size() && k > p->m_keysArr[i]->m_content.key())
        {
            ++i;
        }

        if(i < p->m_keysArr.size() && k == p->m_keysArr[i]->m_content.key())
        {
            pRes = p;
            idx = i;
        }
        else if(p->isLeaf)
        {
            pRes = 0;
            idx = 0;
        }
        else
        {
            __DISK_READ(p->m_childsArr[i]->m_content.m_pointer);
            bTreeSearch(p->m_childsArr[i]->m_content.m_pointer, k, pRes, idx);
        }
    }

    void preceeding(bNode<Val>* pNode, int i, bNode<Val>*& pRes, int& idx)
    {
        if(!pNode->isLeaf)
        {
            bNode<Val>* p = pNode->m_childsArr[i]->m_content.m_pointer;
            while(p->m_childsArr.size())
            {
                p = p->m_childsArr.last()->m_content.m_pointer;
            }
            pRes = p;
            idx = p->m_keysArr.size() - 1;
        }
        else
        {
            pRes = pNode;
            idx = (i-1 >= 0)? i-1:i;
        }
    }

    void successor(bNode<Val>* pNode, int i, bNode<Val>*& pRes, int& idx)
    {//结点的第i个key值得后继key
        if(!pNode->isLeaf)
        {
            bNode<Val>* p = pNode->m_childsArr[i + 1]->m_content.m_pointer;
            while(p->m_childsArr.size())
            {
                p = p->m_childsArr[0]->m_content.m_pointer;
            }
            pRes = p;
            idx = 0;
        }
        else
        {
            pRes = pNode;
            idx = (i + 1 < pNode->m_keysArr.size())? i+1:i;
        }
    }

    void bTreeErase(Key& k)
    {
        int sz = root()->m_childsArr.size();
        erase(root(),k);
    }

    void erase(bNode<Val>* x, Key& k)
    {
        int i = 0;
        while(i < x->m_keysArr.size() && k > x->m_keysArr[i]->m_content.key())
        {
            ++i;
        }

        if(i < x->m_keysArr.size() && k == x->m_keysArr[i]->m_content.key())
        {//找到,key在结点x中
            Node<Val>*  pKey =  x->m_keysArr[i];

            if(x->isLeaf)
            {//condition: 1

                x->m_keysArr.erase(pKey);
            }
            else if(!x->isLeaf)
            {//condition:2
                int idx = 0;
                bNode<Val>* y = x->m_childsArr[i]->m_content.m_pointer;
                bNode<Val>* z = x->m_childsArr[i+1]->m_content.m_pointer;
                if(y->m_keysArr.size() >= t)
                {
                    preceeding(x, i, y, idx);
                    x->m_keysArr[i]->m_content = y->m_keysArr[idx]->m_content;
                    erase(y, x->m_keysArr[i]->m_content.key());
                }
                else
                {
                    if(z->m_keysArr.size() >= t)
                    {
                        successor(x, i, z, idx);
                        x->m_keysArr[i]->m_content = z->m_keysArr[idx]->m_content;
                        erase(z,  x->m_keysArr[i]->m_content.key());
                    }
                    else
                    {
                        x->keyTransfer(i, y, t-1);
                        z->keysTransfer(0, t-2, y, t);
                        z->childsTransfer(0, t-1, y, t);
                        x->m_childsArr.erase(x->m_childsArr[i + 1]);
                        if(x->m_childsArr.size() == 1)
                        {
                            if(x == root())
                                root() = y;
                            delete x;
                            x = y;
                            if(y->m_childsArr.size() >= t && y->m_childsArr.size() <= 2*t)
                                y->isLeaf = false;
                            else
                                y->isLeaf = true;

                        }
                        erase(y, k);
                    }
                }
            }
        }
        else if(!x->isLeaf)
        {//未找到，非叶子
            bNode<Val>* c = x->m_childsArr[i]->m_content.m_pointer;
            int sz = x->m_childsArr.size();
            if(c->m_keysArr.size() < t)
            {//3a,3b
                bNode<Val>*ls = 0, *rs = 0;
                if(i-1 >= 0)
                    ls = x->m_childsArr[i-1]->m_content.m_pointer;
                if(i+1 <  x->m_childsArr.size())
                    rs = x->m_childsArr[i+1]->m_content.m_pointer;

                if(i-1 >= 0 && ls && ls->m_keysArr.size() >= t)
                {
                    x->keyTransfer(i-1, c,0);
                    ls->childTransfer(ls->m_childsArr.size()-1, c, 0);
                    ls->keyTransfer(ls->m_keysArr.size()-1, x, i-1);
                }
                else if(i+1 < x->m_childsArr.size() && rs && rs->m_keysArr.size() >= t)
                {
                    x->keyTransfer(i, c, c->m_keysArr.size());
                    rs->childTransfer(0, c, c->m_childsArr.size());
                    rs->keyTransfer(0, x, i);
                }
                else
                {
                    if(ls)
                    {
                        x->keyTransfer(i-1, c, 0);
                        ls->keysTransfer(0, t-2, c, 0);
                        ls->childsTransfer(0, t-1, c, 0);
                        x->m_childsArr.erase(x->m_childsArr[i-1]);
                        if(x->m_childsArr.size() == 1)
                        {
                            if(x == root())
                                root() = c;
                            delete x;
                            x = c;
                            if(c->m_childsArr.size() >= t && c->m_childsArr.size() <= 2*t)
                                c->isLeaf = false;
                            else
                                c->isLeaf = true;
                        }
                    }
                    else
                    {
                        x->keyTransfer(i, c, c->m_keysArr.size());
                        rs->keysTransfer(0, t-2, c, c->m_keysArr.size());
                        rs->childsTransfer(0, t-1, c, c->m_childsArr.size());
                        x->m_childsArr.erase(x->m_childsArr[i+1]);
                        if(x->m_childsArr.size() == 1)
                        {
                            if(c->m_childsArr.size() >= t && c->m_childsArr.size() <= 2*t)
                            {
                                c->isLeaf = false;
                            }
                            else
                            {
                                c->isLeaf = true;
                            }
                            if(x == root())
                                root() = c;
                            delete x;
                            x = c;
                        }
                    }
                }
            }
            erase(c, k);
        }
        //未找到，叶子，over
    }
};


int main()
{
	fstream file("/home/b8402-2/suanfa/2-3-4树测试数据.txt", fstream::in);
	bTree<int,char> bt(2);
    cout<<"hello1~"<<endl;
	int key;
	char c = 'a';
	while(file>>key)
    {
        bTree<int,char>::Val k(key, c);
        bt.bTreeInsert(k);
    }
    file.close();
    cout<<"hello2~"<<endl;
    bt.traversal();
    file.open("/home/b8402-2/suanfa/2-3-4树删除数据.txt", ios_base::in);
    while(file>>key)
    {
        bt.bTreeErase(key);
    }
    file.close();
    cout<<"dealloc"<<endl;
}