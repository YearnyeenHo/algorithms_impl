#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <time.h>
#include <cstdlib>

using namespace std;
const int MAXN=550;
const double eps=1e-8;

struct Point
{
    double x,y,z;
    Point(){}

    Point(double xx,double yy,double zz):x(xx),y(yy),z(zz){}

    //两向量之差
    Point operator -(const Point p1)
    {
        return Point(x-p1.x,y-p1.y,z-p1.z);
    }

    //两向量之和
    Point operator +(const Point p1)
    {
        return Point(x+p1.x,y+p1.y,z+p1.z);
    }

    //叉乘
    Point operator *(const Point p)
    {
        return Point(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);
    }

    Point operator *(double d)
    {
        return Point(x*d,y*d,z*d);
    }

    Point operator / (double d)
    {
        return Point(x/d,y/d,z/d);
    }

    //点乘
    double  operator ^(Point p)
    {
        return (x*p.x+y*p.y+z*p.z);
    }
};

struct point3D
{
    float x;
    float y;
    float z;

    void set(const point3D & p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
    point3D()
    {
    }
    //copy constructor
    point3D(const point3D & p)
    {
        set(p);
    }
    //assign operator
    point3D& operator=(const point3D & p)
    {
        set(p);
        return *this;
    }

    void printPoint()const
    {
         cout << "("<< this->x << "," << this->y << "," << this->z << ")" <<  endl;
    }

    bool operator==(const point3D & p)
    {
        bool bRes = false;
        if(this->x == p.x && this->y == p.y && this->z == p.z)
        {
            bRes = true;
        }
        return bRes;
    }

    bool operator<(const point3D & p)const
    {
        bool bRes = false;
        if(this->x > p.x)
        {
            bRes = true;
        }
        else if(this->x == p.x)
        {
            if(this->y > p.y)
            {
                bRes = true;
            }
            else if(this->y == p.y)
            {
                if(this->z > p.z)
                {
                    bRes = true;
                }
            }
        }
        return bRes;
    }
};

struct CH3D
{
    struct face
    {
        //表示凸包一个面上的三个点的编号
        int a,b,c;
        //表示该面是否属于最终凸包上的面
        bool ok;
    };
    //初始顶点数
    int n;
    //初始顶点
    Point P[MAXN];
    //凸包表面的三角形数
    int num;
    //凸包表面的三角形
    face F[8*MAXN];
    //凸包表面的三角形
    int g[MAXN][MAXN];
    //向量长度
    double vlen(Point a)
    {
        return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
    }
    //叉乘
    Point cross(const Point &a,const Point &b,const Point &c)
    {
        return Point((b.y-a.y)*(c.z-a.z)-(b.z-a.z)*(c.y-a.y),
                     (b.z-a.z)*(c.x-a.x)-(b.x-a.x)*(c.z-a.z),
                     (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x)
                     );
    }
    //三角形面积*2
    double area(Point a,Point b,Point c)
    {
        return vlen((b-a)*(c-a));
    }
    //四面体有向体积*6
    double volume(Point a,Point b,Point c,Point d)
    {
        return (b-a)*(c-a)^(d-a);
    }
    //正：点在面同向
    double dblcmp(Point &p,face &f)
    {
        Point m=P[f.b]-P[f.a];
        Point n=P[f.c]-P[f.a];
        Point t=p-P[f.a];
        return (m*n)^t;
    }
    void deal(int p,int a,int b)
    {
        int f=g[a][b];//搜索与该边相邻的另一个平面
        face add;
        if(F[f].ok)
        {
            if(dblcmp(P[p],F[f])>eps)
              dfs(p,f);
            else
            {
                add.a=b;
                add.b=a;
                add.c=p;//这里注意顺序，要成右手系
                add.ok=true;
                g[p][b]=g[a][p]=g[b][a]=num;
                F[num++]=add;
            }
        }
    }
    void dfs(int p,int now)//递归搜索所有应该从凸包内删除的面
    {
         F[now].ok=0;
         deal(p,F[now].b,F[now].a);
         deal(p,F[now].c,F[now].b);
         deal(p,F[now].a,F[now].c);
    }
    bool same(int s,int t)
    {
        Point &a=P[F[s].a];
        Point &b=P[F[s].b];
        Point &c=P[F[s].c];
        return fabs(volume(a,b,c,P[F[t].a]))<eps &&
               fabs(volume(a,b,c,P[F[t].b]))<eps &&
               fabs(volume(a,b,c,P[F[t].c]))<eps;
    }
    //构建三维凸包
    void create()
    {
        int i,j,tmp;
        face add;

        num=0;
        if(n<4)return;
    //**********************************************
        //此段是为了保证前四个点不共面
        bool flag=true;
        for(i=1;i<n;i++)
        {
            if(vlen(P[0]-P[i])>eps)
            {
                swap(P[1],P[i]);
                flag=false;
                break;
            }
        }
        if(flag)return;
        flag=true;
        //使前三个点不共线
        for(i=2;i<n;i++)
        {
            if(vlen((P[0]-P[1])*(P[1]-P[i]))>eps)
            {
                swap(P[2],P[i]);
                flag=false;
                break;
            }
        }
        if(flag)return;
        flag=true;
        //使前四个点不共面
        for(unsigned i=3;i<n;i++)
        {
            if(fabs((P[0]-P[1])*(P[1]-P[2])^(P[0]-P[i]))>eps)
            {
                swap(P[3],P[i]);
                flag=false;
                break;
            }
        }
        if(flag)return;
    //*****************************************
        for(i=0;i<4;i++)
        {
            add.a=(i+1)%4;
            add.b=(i+2)%4;
            add.c=(i+3)%4;
            add.ok=true;
            if(dblcmp(P[i],add)>0)swap(add.b,add.c);
            g[add.a][add.b]=g[add.b][add.c]=g[add.c][add.a]=num;
            F[num++]=add;
        }
        for(i=4;i<n;i++)
        {
            for(j=0;j<num;j++)
            {
                if(F[j].ok&&dblcmp(P[i],F[j])>eps)
                {
                    dfs(i,j);
                    break;
                }
            }
        }
        tmp=num;
        for(i=num=0;i<tmp;i++)
          if(F[i].ok)
            F[num++]=F[i];

    }
    //表面积
    double area()
    {
        double res=0;
        if(n==3)
        {
            Point p=cross(P[0],P[1],P[2]);
            res=vlen(p)/2.0;
            return res;
        }
        for(int i=0;i<num;i++)
          res+=area(P[F[i].a],P[F[i].b],P[F[i].c]);
        return res/2.0;
    }
    double volume()
    {
        double res=0;
        Point tmp(0,0,0);
        for(int i=0;i<num;i++)
           res+=volume(tmp,P[F[i].a],P[F[i].b],P[F[i].c]);
        return fabs(res/6.0);
    }
    //表面三角形个数
    int triangle()
    {
        return num;
    }
    //表面多边形个数
    int polygon()
    {
        int i,j,res,flag;
        for(i=res=0;i<num;i++)
        {
            flag=1;
            for(j=0;j<i;j++)
              if(same(i,j))
              {
                  flag=0;
                  break;
              }
            res+=flag;
        }
        return res;
    }
    //三维凸包重心
    Point barycenter()
    {
        Point ans(0,0,0),o(0,0,0);
        double all=0;
        for(int i=0;i<num;i++)
        {
            double vol=volume(o,P[F[i].a],P[F[i].b],P[F[i].c]);
            ans=ans+(o+P[F[i].a]+P[F[i].b]+P[F[i].c])/4.0*vol;
            all+=vol;
        }
        ans=ans/all;
        return ans;
    }
    //点到面的距离
    double ptoface(Point p,int i)
    {
        return fabs(volume(P[F[i].a],P[F[i].b],P[F[i].c],p)/vlen((P[F[i].b]-P[F[i].a])*(P[F[i].c]-P[F[i].a])));
    }
};






/*
-1 0 0
5 0 0
5 5 2
1 2 -5
0 10 2
0 0 5
*/
/*
4 4 4
0 0 0
0 4 4
4 4 0
4 0 4
0 0 4
4 0 0
0 4 0
*/

point3D vectorInverse(const point3D& vec)
{
    point3D v;
    v.x = -vec.x;
    v.y = -vec.y;
    v.z = -vec.z;
    return v;
}

void getData(vector< point3D > &testData, vector< point3D > &allPoints)
{
    int num = testData.size();
    allPoints = testData;
}

int getData( vector< point3D > &allPoints, int num)
{
    float x, y, z;
    int i = 0;
    while( i < num &&  cin >> x >> y >> z )
    {
        point3D point;
         point.x = x;
         point.y = y;
         point.z = z;
        allPoints.push_back(point);
        ++i;
    }
    return 0;
}

//vector p1p2
point3D genVector(const point3D &p1, const point3D &p2)
{
    point3D ptRes;
    ptRes.x = p2.x - p1.x;
    ptRes.y = p2.y - p1.y;
    ptRes.z = p2.z - p1.z;
    return ptRes;
}

float vectorModuius(point3D vec)
{
    return sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0) + pow(vec.z, 2.0));
}

point3D normalVector(const point3D &p1, const point3D &p2, const point3D &p3)
{
    point3D v1 = genVector(p1, p2);
    point3D v2 = genVector(p1, p3);
    point3D vRes;
    vRes.x = v1.y * v2.z - v1.z * v2.y;
    vRes.y = v1.z * v2.x - v1.x * v2.z;
    vRes.z = v1.x * v2.y - v1.y * v2.x;
    return vRes;
}

point3D genUnitVec(const point3D &p1, const point3D &p2, const point3D &p3)
{
    point3D resVec = normalVector(p1, p2, p3);
    float len = vectorModuius(resVec);
    resVec.x = resVec.x/len;
    resVec.y = resVec.y/len;
    resVec.z = resVec.z/len;
    return resVec;
}

float pointDis(const point3D &p1, const point3D &p2)
{
    float sum = 0.0;
    sum = pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0) + pow(p1.z - p2.z, 2.0);
    return sqrt(sum);
}

float point2PlaneDis(const point3D &p, const point3D &p1, const point3D &p2, const point3D &p3)
{
    point3D nVec = normalVector(p1, p2, p3);
    float D = -(nVec.x * p1.x + nVec.y * p1.y + nVec.z * p1.z);
    float dis = fabs(nVec.x * p.x + nVec.y * p.y + nVec.z * p.z + D);
    return dis/sqrt(pow(nVec.x, 2.0) + pow(nVec.y, 2.0) + pow(nVec.z, 2.0));

}

float dotProduct(const point3D &v1, const point3D &v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


void swap( vector< point3D > &allPoints, unsigned i, unsigned j)
{
    point3D temp = allPoints[i];
    allPoints[i] = allPoints[j];
    allPoints[j] = temp;
}

bool greaterThan(const point3D& p1, const point3D &p2)
{
    bool bRes = false;
    if(p1.x > p2.x)
    {
        bRes = true;
    }
    else if(p1.x == p2.x)
    {
        if(p1.y > p2.y)
        {
            bRes = true;
        }
        else if(p1.y == p2.y)
        {
            if(p1.z > p2.z)
            {
                bRes = true;
            }
        }
    }
    return bRes;
}

int partition( vector< point3D > &allPoints, unsigned lo, unsigned hi)
{
    point3D pivoit = allPoints[lo];
    int i = lo - 1;
    int j = hi + 1;
    while(true)
    {
        do
        {
            --j;
        }while(greaterThan(allPoints[j], pivoit));

        do
        {
            ++i;
        }while(greaterThan(pivoit, allPoints[i]));
        if(i < j)
        {
            swap(allPoints, i, j);
        }
        else
            return j;
    }
}

void quickSort( vector< point3D > &allPoints, unsigned begin, unsigned sz)
{
    if(sz-begin > 1)
    {
        int j = partition(allPoints, begin, sz-1);
        quickSort(allPoints, begin, j+1);
        quickSort(allPoints, j + 1, sz);
    }
}

void findMaxPair(int &pi1, int &pi2, vector< point3D > &allPoints)
{
    vector< point3D >::size_type sz =  allPoints.size();
    quickSort(allPoints, 0, sz);
    pi1 = 0;
    pi2 = allPoints.size() - 1;
}

void findMax2LineDisPoint(const int pi1, const int pi2, int &pi3,  vector< point3D > &allPoints)
{
    float eLen = pointDis(allPoints[pi1], allPoints[pi2]);
    point3D eVec = genVector(allPoints[pi1], allPoints[pi2]);

    float maxdis = 0.0;
    for(unsigned i = 0; i < allPoints.size(); i++)
    {
        point3D pVec = genVector(allPoints[pi2], allPoints[i]);
        float projectLen = dotProduct(eVec, pVec)/eLen;
        float pLen = pointDis(allPoints[pi2], allPoints[i]);
        float dis = sqrt(pow(pLen, 2.0) - pow(projectLen, 2.0));
        if(dis > maxdis)
        {
            maxdis = dis;
            pi3 = i;
        }
    }
}

point3D findNewVertex(const point3D &p1, const point3D &p2, const point3D &p3,  vector< point3D > &curPoints)
{
    float maxdis = 0.0;
    point3D resPoint;
     vector< point3D >::iterator maxit;
    for( vector< point3D >::iterator it = curPoints.begin(); it != curPoints.end(); ++it)
    {
        float dis = point2PlaneDis(*it, p1, p2, p3);
        if(dis > maxdis)
        {
            resPoint = *it;
            maxit = it;
        }
    }

    curPoints.erase(maxit);
    return resPoint;
}
class FaceNode
{
    public:
        FaceNode(int pi1, int pi2, int pi3, const vector< point3D > &allPoints)
        {
            m_pi1 = pi1;
            m_pi2 = pi2;
            m_pi3 = pi3;
            m_nVec = normalVector(allPoints[pi1], allPoints[pi2], allPoints[pi3]);
        }

        bool canSee(const vector< point3D > &allPoints, int pi)
        {
            bool bRes = false;

            point3D vecTemp = genVector(allPoints[m_pi1], allPoints[pi]);
            if(dotProduct(m_nVec, vecTemp) > 0)
            {
                bRes = true;
            }
            return bRes;
        }
        int m_pi1, m_pi2, m_pi3;
        point3D m_nVec;
};

void merge( vector< point3D > &res1, const  vector< point3D > &res2)
{
    res1.insert(res1.end(), res2.begin(), res2.end());
}

void tinyRand(point3D &p)
{
    srand(time(NULL));
    int r1 = 0, r2 = 0, r3 = 0;
    while(r1 ==0 && r2 == 0 && r3 == 0)
    {
        r1 = rand()%10-5;
        r2 = rand()%10-5;
        r3 = rand()%10-5;
    }
    p.x += r1/100.0;
    p.y += r2/100.0;
    p.z += r3/100.0;
}

void findPlane(int &pi1, int &pi2, int &pi3,  vector< point3D > &allPoints)
{
    findMaxPair(pi1, pi2, allPoints);
    findMax2LineDisPoint(pi1, pi2, pi3, allPoints);
}

void getPointInMap(point3D &p, const  map< point3D, point3D> &oldPointMap)
{
     map< point3D, point3D>::const_iterator it;
    it = oldPointMap.find(p);
    if(it != oldPointMap.end())
    {
        p = it->second;
    }
}

void eliminatePoints( vector< point3D > &curPoints,  vector< point3D > &allPoints, const point3D &nVec,
                     const point3D &p,  map< point3D, point3D > &oldPointMap)
{
     vector<  vector< point3D >::iterator > itArr;
    for( vector< point3D >::iterator it = allPoints.begin(); it != allPoints.end(); ++it)
    {
        if(oldPointMap.size())
        {
            getPointInMap(*it, oldPointMap);
        }

        point3D vtemp = genVector(p, *it);
        float dp = dotProduct(vtemp, nVec);

        if(dp > 0)
        {
            curPoints.push_back(*it);
            itArr.push_back(it);
            //allPoints.erase(it);
        }
        else if(dp == 0)
        {
            point3D valPoint = *it, tempP = *it;
            while(dp == 0)
            {
                tempP = *it;
                tinyRand(tempP);
                vtemp = genVector(p, tempP);
                dp = dotProduct(vtemp, nVec);
            }

            if(dp > 0)
            {
                *it = tempP;
                curPoints.push_back(*it);
                itArr.push_back(it);
                oldPointMap.insert( pair< point3D, point3D >(*it, valPoint));
            }
        }
    }
    int i = 0;
    for( vector<  vector< point3D >::iterator >::iterator  it = itArr.begin(); it != itArr.end(); ++it)
    {
        allPoints.erase(*it - i);
        ++i;
    }

}

void getPointRestore(point3D &p1, point3D &p2, point3D &p3, const  map< point3D, point3D> &oldPointMap)
{
    getPointInMap(p1, oldPointMap);
    getPointInMap(p2, oldPointMap);
    getPointInMap(p3, oldPointMap);
}

vector< point3D > partCovHull(point3D &p1, point3D &p2, point3D &p3,  vector< point3D > &curPoints,
                                    set< point3D > &nVecSet,  map< point3D, point3D> &oldPointMap)
{
     vector< point3D > res1, res2, res3;
    if (curPoints.size() != 0)
    {
        point3D p = findNewVertex(p1, p2, p3, curPoints);

        point3D nVec1, nVec2, nVec3;
        nVec1 = normalVector(p1, p2, p);
        nVec2 = normalVector(p2, p3, p);
        nVec3 = normalVector(p3, p1, p);

        vector< point3D > curPoints1, curPoints2, curPoints3;
        eliminatePoints(curPoints1, curPoints, nVec1, p, oldPointMap);
        eliminatePoints(curPoints2, curPoints, nVec2, p, oldPointMap);
        eliminatePoints(curPoints3, curPoints, nVec3, p, oldPointMap);

        res1 = partCovHull(p1, p2, p, curPoints1, nVecSet, oldPointMap);
        res2 = partCovHull(p2, p3, p, curPoints2, nVecSet, oldPointMap);
        res3 = partCovHull(p3, p1, p, curPoints3, nVecSet, oldPointMap);
        merge(res1, res2);
        merge(res1, res3);

        res1.push_back(p);
    }
    else
    {
        getPointRestore(p1, p2, p3, oldPointMap);
        point3D unitVec = genUnitVec(p1, p2, p3);
        nVecSet.insert(unitVec);
    }
    return res1;
}

int convexHull3D(vector< point3D > &allPoints)
{
    vector< FaceNode > resFace;
    vector< vector<int> > matx(allPoints.size());
    for(unsigned i = 0; i < allPoints.size(); ++i)
    {
        matx[i].resize(allPoints.size());
    }
    //map< point3D, point3D > oldPointMap;
    int pi1, pi2, pi3;
    findPlane(pi1, pi2, pi3, allPoints);
    FaceNode fn1(pi1, pi2, pi3, allPoints), fn2(pi2, pi1, pi3, allPoints);
    resFace.push_back(fn1);
    resFace.push_back(fn2);

    for(unsigned pi = 0; pi <= allPoints.size(); ++pi)
    {
        if(pi == pi1 || pi == pi2 || pi == pi3)
        {
            continue;
        }
        vector< FaceNode > adjFace;
        for(unsigned fi = 0; fi != resFace.size(); ++fi)
        {
            FaceNode &curface = resFace[fi];
            bool isVis = curface.canSee(allPoints, pi);
            if(!isVis)
            {
                adjFace.push_back(curface);
            }
            matx[curface.m_pi1][curface.m_pi2] = isVis;
            matx[curface.m_pi2][curface.m_pi3] = isVis;
            matx[curface.m_pi3][curface.m_pi1] = isVis;
        }
        for(unsigned fi = 0; fi != resFace.size(); ++fi)
        {//对于每个面的三条边
             FaceNode &curface = resFace[fi];
             int p1 = curface.m_pi1, p2 = curface.m_pi2, p3 = curface.m_pi3;
             if(matx[p1][p2] !=  matx[p2][p1] && matx[p1][p2])
             {
                 FaceNode fn(p1, p2, pi, allPoints);
                 adjFace.push_back(fn);
             }
             if(matx[p2][p3] !=  matx[p3][p2] && matx[p2][p3])
             {
                FaceNode fn(p2, p3, pi, allPoints);
                adjFace.push_back(fn);
             }
            if(matx[p3][p1] !=  matx[p1][p3] && matx[p3][p1])
             {
                FaceNode fn(p3, p1, pi, allPoints);
                adjFace.push_back(fn);
             }
        }
        resFace = adjFace;
    }
    return resFace.size();
}

vector< point3D > genTestData()
{
    srand(time(NULL));
    int num = 1024;
    vector< point3D > fv;
    point3D temp;
    while(num > 0)
    {
        temp.x = 50 - rand()%100;
        temp.y = 50 - rand()%100;
        temp.z = 50 - rand()%100;
        fv.push_back(temp);
        --num;
    }
    return fv;
}

void getData(vector< point3D > &testData, CH3D &hull)
{
    hull.n = testData.size();
    int i = 0;
    for(vector< point3D >::iterator it = testData.begin(); it != testData.end(); ++it)
    {
        hull.P[i].x = it->x;
        hull.P[i].y = it->y;
        hull.P[i].z = it->z;
        ++i;
    }
}
void tinyRands(vector< point3D > &testData)
{
    for(vector< point3D >::iterator it = testData.begin(); it != testData.end(); ++it)
    {
        tinyRand(*it);
    }
}
CH3D hull;
int main()
{
   // freopen("in.txt","r",stdin);
   // freopen("out.txt","w",stdout);
    vector< point3D > testData;
    testData = genTestData();
    vector< point3D > allPoints, resPoints;
    tinyRands(testData);
    getData(testData, allPoints);

    int faceCout = convexHull3D(allPoints);
    printf("%d\n", faceCout);

    getData(testData, hull);
    hull.create();
    printf("%d\n",hull.polygon());
    return 0;
}
