#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#define MAX_LEN 5000

class Vertex
{
public:
	Vertex()
	{
		m_nVal = -1;
	}

	Vertex(int num)
	{
		m_nVal = num;
	}

	int m_nVal;


};

template<class T> class Node
{
public:
	T val;
	Node* next;
	Node* pre;
	Node(T tval)
	{
		this->val = tval;
		pre = NULL;
		next = NULL;
	}
};

template<class T> class ArrList
{
public:
	Node<T>* head;
	Node<T>* tail;
	Node<T>* cur;
	int curPos;
	int sz;
	ArrList()
	{
		cur = tail = head = NULL;
		sz = 0;
		curPos = -1;
	}

	void insert(T val)
	{
		Node<T>* pNode = new Node<T> (val);
		if(NULL == head)
		{
			cur = tail = head = pNode;
			curPos = 0;
		}
		else
		{
			tail->next = pNode;
			pNode->pre = tail;
			tail = tail->next;
		}

		++sz;
	}

	void erase()
	{
		if(head != NULL)
		{
			if(head == tail)
            {
                head = NULL;
            }
			Node<T>* pTemp = tail;
			tail = tail->pre;
			delete pTemp;
			if(tail != NULL)
				tail->next = NULL;
			--sz;
		}
	}

	T getCurNodeVal()
	{
		return cur->val;
	}

	bool next()
	{
		bool bRes = false;
		if(cur != NULL)
		{
			cur = cur->next;
			++curPos;
			bRes = true;
		}
		return bRes;
	}

	void deleteAll()
	{
		while(sz)
		{
			erase();
		}
	}

	~ArrList()
	{
		deleteAll();
	}
};

class undirectedGraph
{
public:
	undirectedGraph()
	{
		m_numV = MAX_LEN;
		m_nTime = 0;
		m_numWhite = 0;
		m_adjTable = new ArrList< int > [MAX_LEN];
		m_nVertices = new Vertex [MAX_LEN];
		m_nArrColor = new int [MAX_LEN];
		m_nArrD = new int [MAX_LEN];
		m_nArrLow = new int [MAX_LEN];
		m_pre = new int [MAX_LEN];
		init();
	}
	undirectedGraph(int num)
	{
		m_numV = num;
		m_numWhite = 0;
		m_nTime = 0;
		m_adjTable = new ArrList< int > [num];
		m_nVertices = new Vertex [num];
		m_nArrColor = new int [num];
		m_nArrD = new int [num];
		m_nArrLow = new int [num];
		m_pre = new int [num];
		init();
	}

	void init()
	{
		for(int i = 0; i < m_numV; ++i)
		{
			m_nArrColor[i] = BLACK;
			m_pre[i] = -1;
		}


	}

	void addVertex(int idx, int nVal)
	{
		if(idx < m_numV && m_nVertices[idx].m_nVal == -1)
		{
			Vertex vt(nVal);
			m_nVertices[idx] = vt;
			m_nArrColor[idx] = WHITE;
			++m_numWhite;
		}
	}

	void addEdge(int ix, int iy)
	{
		m_adjTable[ix].insert(iy);
		m_adjTable[iy].insert(ix);
	}

	void runArtPt()
	{
		while(m_numWhite > 0)
		{
			int k;
			for(int i = 0; i < m_numV; ++i)
			{
				if(m_nArrColor[i] == WHITE)
				{
					k = i;
					break;
				}
			}
			std::cout<<"k "<<k<<std::endl;
			ArtPt(k);
		}

	}

	//depth first search
	void ArtPt(int u)
	{
        --m_numWhite;
		m_nArrColor[u] = GRAY;
		m_nArrLow[u] = m_nArrD[u] = ++m_nTime;
		for(int i = 0; i < m_adjTable[u].sz; ++i)
		{
			int v = m_adjTable[u].getCurNodeVal();
			if(m_nArrColor[v] == WHITE)
			{
				m_pre[v] = u;
				ArtPt(v);
				m_nArrLow[u] = min(m_nArrLow[u], m_nArrLow[v]);

				if(m_pre[u] == -1)
				{
				    std::cout<<"root hello"<<std::endl;
				    std::cout<<"curpos"<<m_adjTable[u].curPos<<std::endl;
					if(m_adjTable[u].curPos == 1)
					{
						std::cout<<"root = "<<u<<"; curpos"<<m_adjTable[u].curPos<<std::endl;
						m_artList.insert(u);
					}
				}
				else if (m_nArrD[u] <= m_nArrLow[v])
				{
					//std::cout<<"add"<<std::endl;
					m_artList.insert(u);
				}
			}
			else if(v != m_pre[u])//(u, v) is back edge
			{
				m_nArrLow[u] = min(m_nArrLow[u], m_nArrD[v]);
			}
			m_adjTable[u].next();
		}
		m_nArrColor[u] = BLACK;
	}

	int min(int a, int b)
	{
		int m = a;
		if(a > b)
		{
			m = b;
		}
		return m;
	}

	void printArt()
	{
		std::set<int> rset;
		for(int i = 0; i < m_artList.sz; ++i)
		{
			rset.insert( m_artList.getCurNodeVal());
			//std::cout << "art point = " << m_artList.getCurNodeVal() << std::endl;
			m_artList.next();

		}
/*
		for(std::set<int>::iterator it = rset.begin(); it != rset.end(); ++it)
        {
            std::cout << "art point " << *it << std::endl;
        }
*/
        std::cout << "total num " << rset.size() << std::endl;
	}

	~undirectedGraph()
	{
		delete [] m_adjTable;
		delete [] m_nArrLow;
		delete [] m_nArrColor;
		delete [] m_nArrD;
	}
private:
	enum colorType{WHITE, GRAY, BLACK};
	ArrList<int>*  m_adjTable;//存放顶点对应index顶点的临街邻接关系
	ArrList<int> m_artList;//list of articulate points
	Vertex* m_nVertices;
	int* m_nArrColor;
	int* m_nArrD;
	int* m_nArrLow;
	int* m_pre;
	int  m_numV;
	int  m_nTime;
	int m_numWhite;
};

int main()
{
	//int num;
	//std::cin >> num;

	std::ifstream infile;
	infile.open("关节点测试数据3.txt", std::ifstream::in);
    undirectedGraph g;
	char c1,c2,c3;
	int i1, i2, counter = 0;

    std::cout<<"reading"<<std::endl;
	while(infile>>c1>>i1>>c2>>i2>>c3)
	{

		++counter;
		g.addVertex(i1, i1);
		g.addVertex(i2, i2);
		g.addEdge(i1, i2);
	}
	infile.close();

/*
	int num;
	std::cin>>num;
	undirectedGraph g(num);
	int ar[] = {0,1,2,3,4,5,6,7,8,9};
	for(int i = 0; i < num; ++i)
	{
		int idx, val;
		//std::cin >> idx >> val;
		//g.addVertex(idx, val);
		g.addVertex(ar[i], ar[i]);
	}
	std::cin >> num;
	int i = 0;
	int id1, id2;
	int v1[] = {0,1,2,3,4,2,0,1,2,0,7,8,9};
	int v2[] = {1,2,3,4,5,5,6,6,6,7,8,9,7};
	while(i<num)
	{
		//std::cin >> id1 >> id2;
		//g.addEdge(id1, id2);
		g.addEdge(v1[i], v2[i]);
		++i;
		std::cout<<"hello~"<<i<<std::endl;
	}
	std::cout<<"hello~"<<std::endl;
*/
	g.runArtPt();
	g.printArt();
	return 0;
}
