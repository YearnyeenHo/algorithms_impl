#include <fstream>
#include <string>
#include <iostream>
#include <time.h>
using namespace std;

typedef bool RbColorType;
const RbColorType rbRed = false;
const RbColorType rbBlack = true;

//reb-black Node
struct rbNodeBase
{
	RbColorType m_color;
	rbNodeBase* m_parent;
	rbNodeBase* m_left;
	rbNodeBase* m_right;

	rbNodeBase()
	{
	    m_color = rbBlack;
	    m_parent = 0;
	    m_left = 0;
	    m_right = 0;

	}

	static rbNodeBase* minimum(rbNodeBase* pCur)
	{
		while(pCur->m_left != 0)
		{
			pCur = pCur->m_left;
		}
		return pCur;
	}

	static rbNodeBase* maximum(rbNodeBase* pCur)
	{
		while(pCur->m_right != 0)
		{
			pCur = pCur->m_right;
		}
		return pCur;
	}

	virtual ~rbNodeBase()
	{

	}
};

template <class Val>
struct rbTreeNode:public rbNodeBase
{
	Val m_value;
};

//Iterator
struct rbIteratorBase
{
	rbNodeBase* m_pNodeBase;
	//输出按中序遍历顺序中的的下一个结点
    rbIteratorBase(rbNodeBase* pNode)
	{
	    m_pNodeBase = pNode;
	}

	rbIteratorBase(const rbIteratorBase& it)
	{
		this->m_pNodeBase = it.m_pNodeBase;
	}

	void operator=(rbIteratorBase it)
	{
		this->m_pNodeBase = it.m_pNodeBase;
	}

    bool operator!=(rbIteratorBase it)
    {
        return it.m_pNodeBase != m_pNodeBase;
    }

    bool operator==(rbIteratorBase it)
    {
        return it.m_pNodeBase == m_pNodeBase;
    }

    int diff(rbIteratorBase it2)
    {
    	int count = 0;
    	while((*this) != it2)
    	{
    		(*this).incre();
    		++count;
    	}
    	return ++count;
    }

	void incre()
	{
		if(m_pNodeBase->m_right != 0)
		{
			m_pNodeBase = m_pNodeBase->m_right;
			while(m_pNodeBase->m_left != 0)
			{
				m_pNodeBase = m_pNodeBase->m_left;
			}
		}
		else
		{
			rbNodeBase* pParent = m_pNodeBase->m_parent;
			while(pParent->m_right == m_pNodeBase) //本身是右子节点，上溯。直到不为右子节点或达到pParent为NULL，m_pNodeBase到了root
			{
				m_pNodeBase = pParent;
				pParent = pParent->m_parent;
			}
			if(m_pNodeBase->m_right != pParent)//当m_pNodeBase原本就是最后一个结点，上溯到最后等于root，而pParent等于NULL（即，一直沿右子树上溯到root），将当前node设为NULL
			{								 //或者当node本身就是root，没有右子节点，此时pParent等于NULL，
				m_pNodeBase = pParent;
			}

		}
	}
	void decre()
	{
		if(rbRed == m_pNodeBase->m_color && m_pNodeBase->m_parent->m_parent == m_pNodeBase)
		{
			m_pNodeBase = m_pNodeBase->m_right;
		}
		else if (m_pNodeBase->m_left != 0)
		{
			m_pNodeBase = m_pNodeBase->m_left;
			while(m_pNodeBase->m_right != 0)
			{
				m_pNodeBase = m_pNodeBase->m_right;
			}
		}
		else
		{
			rbNodeBase* pParent = m_pNodeBase->m_parent;
			while(pParent->m_left == m_pNodeBase)
			{
				m_pNodeBase = pParent;
				pParent = pParent->m_parent;
			}
			m_pNodeBase = pParent;
		}
	}
};

template <class Val>
struct rbTreeIterator:public rbIteratorBase
{
	typedef rbTreeIterator<Val> classtype;
	rbTreeIterator(rbNodeBase* pNode): rbIteratorBase(pNode)
	{

	}

	Val& operator*()const
	{
		return ((rbTreeNode<Val>*)(m_pNodeBase))->m_value;
	}

	Val* operator->()const
	{
		return &(operator*());
	}

	classtype& operator++()
	{
		incre();
		return *this;
	}

	classtype& operator--()
	{
		decre();
		return *this;
	}

};

template <class Key, class Val>
struct rbPair
{
	Key m_key;
	Val m_value;
	rbPair()
	{

	}

    rbPair(rbPair<Key, Val>& v)
	{
		m_key = v.key();
		m_value = v.value();
	}

	rbPair(const Key& k, const Val& v)
	{
	    m_key = k;
	    m_value = v;
	}
	Key& key()
	{
		return m_key;
	}

	Val& value()
	{
		return m_value;
	}
};

template <class Key, class Val>
struct KeyValue
{
	Key& operator()(Val& value)
	{
		return value.key();
	}
};

template <class Key>
struct tmplCompare
{
	bool operator()(const Key& k1, const Key& k2)
	{
		bool bRes = false;
		if(k1 < k2)
			bRes = true;
		return bRes;
	}
};

//rbTree<int, pair<int, string>, KeyofValue<int, pair<int, string>, Compare<int>>
template <class Key, class value_type>
class rbTree
{
public:
	typedef rbPair<Key, value_type> Val;
	typedef KeyValue<Key, Val> KeyofValue;
	typedef tmplCompare<Key> Compare;

protected:
	/*树结点的构造，析构*/
	rbNodeBase* getNode()
	{
		return new rbTreeNode<Val>; //allocate();
	}

	void putNode(rbNodeBase*& pNode)
	{
		//dealloctor(pNode);
		delete pNode;
		pNode = 0;
	}

	rbNodeBase* createNode(const Val& value)
	{
		rbTreeNode<Val>* pTmp = (rbTreeNode<Val>*)getNode();
		if(pTmp != 0)
		{
			//construct(&pTmp->m_value, value);//构造value的object
			/*cnstruct失败，异常处理？*/
			pTmp->m_value = value;//copy construct
		}
		return pTmp;
	}

	void destroyNode(rbNodeBase* pNode)
	{
		//destroy(pNode->m_value);//先析构value object
		putNode(pNode);//释放内存
	}

    void clear()
    {
    	dfsClear(root());

    }

    void dfsClear(rbNodeBase* pNode)
    {
    	if(pNode != 0 )
    	{
	    	dfsClear(pNode->m_left);
    		dfsClear(pNode->m_right);
    		putNode(pNode);
    	}

    }

    void dfsCheck(rbNodeBase* p, int& counter)
    {
    	int c1 = 0,c2 = 0;
    	if(p != 0)
    	{
    		if(p->m_color == rbBlack) ++counter;
    		dfsCheck(p->m_left, c1);
    		dfsCheck(p->m_right, c2);
    		if(c1 == c2)
    		{
    			counter += c1;
    		}
    		else
    		{
    			cout<<"tree property error!"<<endl;
    		}
    	}
    	else
    		++counter;
    }

    void preOrderCheck(rbNodeBase* pNode)
    {
    	if(pNode != 0)
    	{
		   	if(pNode->m_left && pNode->m_right)
		   	{
		   		if(pNode->m_left->m_color == rbBlack && pNode->m_right->m_color == rbRed)
		   		{
		   			if(pNode->m_left->m_left == 0 && pNode->m_left->m_right == 0 &&
		   				pNode->m_right->m_left == 0 && pNode->m_right->m_right == 0)
		   				cout<<"something wrong!!!!! in the tree"<<endl;
		   		}
		   		else if(pNode->m_left->m_color == rbRed && pNode->m_right->m_color == rbBlack)
		   		{
		   			if(pNode->m_left->m_left == 0 && pNode->m_left->m_right == 0 &&
		   				pNode->m_right->m_left == 0 && pNode->m_right->m_right == 0)
		   				cout<<"something wrong!!!!! in the tree"<<endl;
		   		}

		   	}
		   	preOrderCheck(pNode->m_left);
    		preOrderCheck(pNode->m_right);
    	}
    }
protected:
	size_t m_nodeCount;
	rbNodeBase* m_header;
	Compare m_keyCompare;//function object???

	rbNodeBase*& root()const
	{
		return (rbNodeBase*&)m_header->m_parent;
	}

	rbNodeBase*& leftmost()const
	{
		return (rbNodeBase*&)m_header->m_left;
	}

	rbNodeBase*& rightmost()const
	{
		return m_header->m_right;
	}

	rbNodeBase*& sibling(rbNodeBase* pNode)
	{
		if(pNode->m_parent->m_left == pNode)
			return pNode->m_parent->m_right;
		else
			return pNode->m_parent->m_left;
	}
public:
	static int decount;
	static void countIncre();

	//constructor
	rbTree(const Compare& comp = Compare()) : m_nodeCount(0), m_keyCompare(comp)
	{//默认参数为，comp等于一个Compare Object
		__init();
	}

	~rbTree()
	{
		clear();
		putNode(m_header);
		rbTree<Key, value_type>::countIncre();
	}

public:
	typedef rbTreeIterator <Val> iterator;

	iterator begin()const
	{
		return leftmost();//leftmost返回的是一个指针，但是这里会被用来构造一个匿名iterator对象作为返回值
	}

	iterator end()const
	{
		return m_header;
	}

	bool empty()const
	{
		return 0 == m_nodeCount;
	}

	size_t size()const
	{
		return m_nodeCount;
	}

	Key& key(rbNodeBase*& pNode)
	{
		return KeyofValue()(((rbTreeNode<Val>*)pNode)->m_value);
	}

	size_t maxSize()const
	{
		return size_t(-1);
	}

	iterator insertEqual(Val& value)
	{
		rbNodeBase* p = m_header;
		rbNodeBase* n = root();
		while(n != 0)
		{
			p = n;
			n = m_keyCompare(KeyofValue()(value), key(n)) ? n->m_left : n->m_right;
		}
		return __insert(n, p, value);//n为新插入节点，y为插入点的父结点
	}

	void copyByPreoderTravs(rbNodeBase*& p)
	{
		if(p != 0)
		{
			insertEqual(((rbTreeNode<Val>*)p)->m_value);
			copyByPreoderTravs(p->m_left);
			copyByPreoderTravs(p->m_right);
		}
	}

    void insertBatch(rbTree<Key,value_type>& vec)
	{
		copyByPreoderTravs(vec.root());
	}

	iterator find(const Key& k)
	{
		rbNodeBase* pNode = m_header;
		rbNodeBase* pCurNode = root();
		while(pCurNode != 0)
		{
			if(!m_keyCompare(key(pCurNode), k))
			{//pCurNode的键值 > k
				pNode = pCurNode;
				pCurNode = pCurNode->m_left;
			}
			else
			{
				pCurNode = pCurNode->m_right;
			}
		}
        iterator it = iterator(pNode);
        return (it == end()|| m_keyCompare(k, key(it.m_pNodeBase))) ? end() : it;
	}

	void eraseByKey(Key& k)
	{
		iterator it = find(k);
		deletion(it);
	}

	void eraseByKeys(Key& k1, Key& k2)
	{//[k1, k2]范围
		if(m_keyCompare(k1, k2))
		{
			while(find(k1) == end()) ++k1;
			iterator it1 = find(k1);
			while(find(k2) == end()) --k2;
			iterator it2 = find(k2);
			iterator it3 = it2;
			while((*it3).key() == k2) ++it3;
			it2 = --it3;
			//erase(it1, it2);
			deletion(it1, it2);
		}
	}

	void eraseById(int idx)
	{
		int i = 0;
		iterator it = begin();
		for(; i< idx-1; ++i)
		{
			++it;
		}
		deletion(it);
	}

void eraseByIds(int nfirst, int nlast)
{
    if(nfirst <= nlast)
		{
			iterator it1 = begin();
			iterator it2 = begin();
			for(int i = 0; i < nfirst-1; ++i)
			{
				++it1;
			}
			for(int i = 0; i < nlast-1; ++i)
			{
				++it2;
			}
			deletion(it1, it2);
		}

}

    void check()
    {
    	preOrderCheck(root());
    	int counter = 0;
    	dfsCheck(root(), counter);
    }

private:
	void __init()
	{
		m_header = getNode();
		m_header->m_color = rbRed;
		root() = 0;
		leftmost() = m_header;
		rightmost() = m_header;
	}
//不太理解。。。
	iterator __insert(rbNodeBase* n, rbNodeBase* p, Val& value)
	{
		rbNodeBase* pTmp;
		if(p == m_header || n != 0 || m_keyCompare(KeyofValue()(value),key(p)))
		{//KeyofValue是一个function object class，而在这个class后面加一个括号表明创建一个匿名对象并调用它重载的（）运算子
			pTmp = createNode(value);
			p->m_left = pTmp;
			if(p == m_header)
			{
				root() = pTmp;
				rightmost() = pTmp;
			}
			else if(p == leftmost())
			{
				leftmost() = pTmp;
			}
		}
		else
        {
            pTmp = createNode(value);
            p->m_right = pTmp;
            if(p == rightmost())
            {
                rightmost() = pTmp;
            }
        }
        pTmp->m_parent = p;
        pTmp->m_left = 0;
        pTmp->m_right = 0;

        rbTreeReblance(pTmp, m_header->m_parent);
        ++m_nodeCount;
        return iterator(pTmp);
	}

	void rbTreeReblance(rbNodeBase*& n, rbNodeBase*& root)
	{
		n->m_color = rbRed;
		while(n != root && n->m_parent->m_color == rbRed)
		{
			if(n->m_parent == n->m_parent->m_parent->m_left)
			{//插入点在G的左子树
				rbNodeBase* u = n->m_parent->m_parent->m_right;
				if(u && u->m_color == rbRed)
				{//有u并且u为红色
					n->m_parent->m_color = rbBlack;
					u->m_color = rbBlack;
					n->m_parent->m_parent->m_color = rbRed;
					n = n->m_parent->m_parent;
				}
				else
				{//没有u，或u为黑色结点
					if(n == n->m_parent->m_right)
					{//若为内侧插入，要先左旋，达到外侧插入状态
						n = n->m_parent;
						rotateLeft(n, root);
					}
					n->m_parent->m_color = rbBlack;
					n->m_parent->m_parent->m_color = rbRed;
					rotateRight(n->m_parent->m_parent, root);
				}
			}
			else
			{//插入点在G的右子树
				rbNodeBase* g = n->m_parent->m_parent;
				rbNodeBase* u = g->m_left;
				if(u && u->m_color == rbRed)
				{//有u并且u为红色
					g->m_color = rbRed;
					n->m_parent->m_color = rbBlack;
					u->m_color = rbBlack;
					n = g;//go on check
				}
				else
				{//没有u，或u为黑色结点
					if(n == n->m_parent->m_left)
					{//若为内侧插入，要先右旋，达到外侧插入状态
						n = n->m_parent;
						rotateRight(n, root);
					}
					n->m_parent->m_color = rbBlack;
					g->m_color = rbRed;
					rotateLeft(g, root);
				}
			}
		}
		root->m_color = rbBlack;
	}

	void rotateLeft(rbNodeBase* pNode, rbNodeBase*& root)
	{//pNode为旋转点
            rbNodeBase* pRNode = pNode->m_right;
            //先将右子女中的左子树易主
            pNode->m_right = pRNode->m_left;
            if(pRNode->m_left != 0)
            {
                pRNode->m_left->m_parent = pNode;
            }
            //右子结点上升（并更新上层结点指向），再使旋转中心自由下落
            pRNode->m_parent = pNode->m_parent;
            if(root == pNode)
            {
                root = pRNode;
            }
            else if(pNode == pNode->m_parent->m_left)
            {//左子结点还是右子结点
                pNode->m_parent->m_left = pRNode;
            }
            else
            {
                pNode->m_parent->m_right = pRNode;
            }
            //使旋转中心成为左子结点
            pRNode->m_left = pNode;
            pNode->m_parent = pRNode;
	}

	void rotateRight(rbNodeBase* pNode, rbNodeBase*& root)
	{
            rbNodeBase* pTmp = pNode->m_left;
            pNode->m_left = pTmp->m_right;
            if(pTmp->m_right != 0)
            {
                pTmp->m_right->m_parent = pNode;
            }

            pTmp->m_parent = pNode->m_parent;
            if(pNode == root)
            {
                root = pTmp;
            }
            else if(pNode == pNode->m_parent->m_left)
            {
                pNode->m_parent->m_left = pTmp;
            }
            else
            {
                pNode->m_parent->m_right = pTmp;
            }
            pTmp->m_right = pNode;
            pNode->m_parent = pTmp;
	}

	void nodeValExchang(rbNodeBase* p1, rbNodeBase* p2)
	{
		Val vtmp = ((rbTreeNode<Val>*)p1)->m_value;
		((rbTreeNode<Val>*)p1)->m_value = ((rbTreeNode<Val>*)p2)->m_value;
		((rbTreeNode<Val>*)p2)->m_value = vtmp;
	}

    iterator deletion(iterator it)
	{
		//rbNodeBase* y = _Rb_tree_rebalance_for_erase(it.m_pNodeBase, m_header->m_parent, m_header->m_left, m_header->m_right);
		iterator rt = it;
		rbNodeBase* y = rbErase(it.m_pNodeBase);
		if(y == it.m_pNodeBase)
        {
            rt = ++it;
        }
		putNode(y);
		--m_nodeCount;
		return rt;
	}

	void deletion(iterator it1, iterator it2)
	{
        if (it1 == begin() && it2 == end())
    		clear();
  		else
        {
            Val v2 = ((rbTreeNode<Val>*)it2.m_pNodeBase)->m_value;
            while (((rbTreeNode<Val>*)it1.m_pNodeBase)->m_value.key() != v2.key())
            {
               it1 = deletion(it1);
            }
            deletion(it1);
        }
	}
/*
*/
	rbNodeBase* rbErase(rbNodeBase* z)
	{
		rbNodeBase* c = 0;
		rbNodeBase*cparent = 0;
		BSTdeletion(z, c, cparent);
		if(z->m_color != rbRed)
		{
			rbRebalanceForEarse(c, cparent);
		}
		return z;
	}

	RbColorType nodeColor(rbNodeBase* p)
	{
		if(p != 0)
		 	return p->m_color;
		else
			return rbBlack;
	}

	void rbRebalanceForEarse(rbNodeBase* c, rbNodeBase* cparent)
	{
		rbNodeBase* s = 0;
		while(c != root() && nodeColor(c) == rbBlack)
		{
			if(cparent->m_left == c)
			{
				s = cparent->m_right;
				if(nodeColor(s) == rbRed)
				{
					cparent->m_color = rbRed;
					s->m_color = rbBlack;
					rotateLeft(cparent, root());
					s = cparent->m_right;
				}//case 1
				if(nodeColor(s->m_left) == rbBlack && nodeColor(s->m_right) == rbBlack)
				{
					s->m_color = rbRed;
					c = cparent;
					cparent = c->m_parent;
				}
				else
				{
					if(nodeColor(s->m_right) == rbBlack)
					{
						s->m_color = rbRed;
						s->m_left->m_color = rbBlack;
						rotateRight(s, root());
						s = cparent->m_right;
					}

					s->m_right->m_color = rbBlack;
					s->m_color = cparent->m_color;
					cparent->m_color = rbBlack;
					rotateLeft(cparent, root());
					c = root();
				}
			}
			else
			{
				s = cparent->m_left;
				if(nodeColor(s) == rbRed)
				{
					cparent->m_color = rbRed;
					s->m_color = rbBlack;
					rotateRight(cparent, root());
					s = cparent->m_left;
				}
				if(nodeColor(s->m_left) == rbBlack && nodeColor(s->m_right) == rbBlack)
				{
					s->m_color = rbRed;
					c = cparent;
					cparent = c->m_parent;
				}
				else
				{
					if(nodeColor(s->m_left) == rbBlack)
					{
						s->m_color = rbRed;
						s->m_right->m_color = rbBlack;
						rotateLeft(s, root());
						s = cparent->m_left;
					}
					s->m_color = cparent->m_color;
					s->m_left->m_color = rbBlack;
					cparent->m_color = rbBlack;
					rotateRight(cparent, root());
					c = root();
				}

			}
		}
		c->m_color = rbBlack;
	}

	void BSTdeletion(rbNodeBase*& z, rbNodeBase*& c, rbNodeBase*& cparent)
	{
		rbNodeBase* m = z;
		if(z->m_left != 0 && z->m_right != 0)
		{
			m = z->m_right;
			while(m->m_left != 0) m = m->m_left;
			c = m->m_right;
		}
		else if(z->m_left != 0)
		{
			c = z->m_left;
		}
		else
		{
			c = z->m_right;
		}

		//if(m == z)
		//{
			cparent = m->m_parent;
			if(c) c->m_parent = cparent;

			if(leftmost() == m )
			{
				if(c)
					leftmost() = rbNodeBase::minimum(c);
				else
					leftmost() = m->m_parent;
			}
			else if(rightmost() == m)
			{
				if(c)
					rightmost() = rbNodeBase::maximum(c);
				else
					rightmost() = m->m_parent;
			}

			if(root() == m)
			{
				root() = c;
			}
			else if(m->m_parent->m_left == m)
			{
				cparent->m_left = c;
			}
			else
				cparent->m_right = c;
		//}
		//else
		if(m != z)
		{
			//cparent = m->m_parent;
			//if(c) c->m_parent = cparent;
			//m->m_parent->m_left = c;
			nodeValExchang(z, m);
			z = m;
		}
	}

//************************************************************************************

};

class mkString
{
public:
    string m_str;
    void append(char cstr[])
    {
        m_str.append(cstr);
    }

    ostream& operator<<(mkString& mkstr)const
    {
        return cout<<m_str;
    }
};
template<typename Key, typename value_type>
int rbTree<Key, value_type>::decount = 0;
template<typename Key, typename value_type>
void rbTree<Key, value_type>::countIncre()
{
    ++decount;
    //cout<<decount<<endl;
}



int main()
{
	rbTree<int, string> t, tree;
	fstream file("测试数据1.txt",fstream::in);
    int n;
    char cstr1[5], cstr2[9], cstr3[11], cstr4[41];
	while(file>>n>>cstr1>>cstr2>>cstr3>>cstr4)
    {
        string str;
        //cout<<n<<" "<<cstr1<<" "<<cstr2<<" "<<cstr3<<" "<<cstr4<<endl;
        str.append(cstr1);
        str.append(cstr2);
        str.append(cstr3);
        str.append(cstr4);
        rbPair<int, string> pairTmp(n, str);
        t.insertEqual(pairTmp);
    }
    file.close();


     float t1 = clock();
    //批量插入
    cout<<"批量插入"<<endl;
    tree.insertBatch(t);

    //逐个插入
    cout<<"逐个插入"<<endl;
    file.open("测试数据1中待插入数据.txt",fstream::in);
    while(file>>n>>cstr1>>cstr2>>cstr3>>cstr4)
    {
        string str;
        //cout<<n<<" "<<cstr1<<" "<<cstr2<<" "<<cstr3<<" "<<cstr4<<endl;
        str.append(cstr1);
        str.append(cstr2);
        str.append(cstr3);
        str.append(cstr4);
        rbPair<int, string> pairTmp(n, str);
        tree.insertEqual(pairTmp);
    }
    file.close();

    //按指定int32整型值删除记录；//earseByKey
    cout<<"按指定int32整型值删除记录"<<endl;
    file.open("测试数据1中待删除的int整型集合数据11.txt",fstream::in);
    while(file>>n)
    {
        tree.eraseByKey(n);
    }
    file.close();
    //按指定int32整型值范围批量删除记录；//earseByKeys

    cout<<"按指定int32整型值范围批量删除记录；"<<endl;

    int n1[3] = {1201, 200010, 528394};
    int n2[3] = {140200, 493029,763748};
    tree.eraseByKeys(n1[0], n2[0]);
    tree.eraseByKeys(n1[1], n2[1]);
    tree.eraseByKeys(n1[2], n2[2]);

    //删除指定排名数据，如第1万位；//earseById
    cout<<"删除指定排名数据，如第1万位；"<<endl;
    tree.eraseById(10000);
    //批量删除排名段数据，如第1万~第2万。//earseByIds
    cout<<"批量删除排名段数据，如第2万~第3万"<<endl;
    tree.eraseByIds(20000, 30000);

    //查询指定int32整型值的记录。//find
    cout<<"查询指定int32整型值的记录"<<endl;
    file.open("测试数据1中待查询的int整数型集合数据13.txt",fstream::in);
    while(file>>n)
    {
        tree.find(n);
            //cout<<"find!"<<endl;
    }
    float t2 = clock();
    //cout<<"dealloc!!!"<<endl;
    file.close();
    cout<<"总用时 :"<<(t2-t1)/1000.0 <<" s";
}
