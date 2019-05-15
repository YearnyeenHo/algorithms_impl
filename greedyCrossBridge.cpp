/*
n
t1
t2
t3
t4
t5
...
tn

1 2
2 3
2 4
2 5
2 6
2 7
2 8
2 9
2 10
T
*/
#define MAX_LEN 1000
#include <iostream>
#include <fstream>
using namespace std;
struct person
{
	int id;
	int time;
	int k;
	person& operator=(const person& p)
	{
	    this->id = p.id;
	    this->time = p.time;
	    return *this;
	}

};

person g_arr[MAX_LEN];
int g_arrBank[MAX_LEN];
int n;
bool sorted;

void readData()
{
	fstream file("test1.txt", ios::in);
	int t;
	file>>n;
	int i = 0;
	while(file>>t)
	{
		g_arr[i].id = i;
		g_arr[i].time = t;
		++i;
	}
	file.close();
	sorted = false;
}

void kReset()
{
    int i = 0;
    while(i < n)
    {
        g_arr[i].k = 0;
        ++i;
    }
}

void ressetState()
{
    int i = 0;
    while(i < n)
    {
        g_arrBank[i++] = 0;
    }
}

void swap(person& a, person& b)
{
	person tmp = a;
	a = b;
	b = tmp;
}

int partition(person arr[], int s, int e)
{
	int pivot = arr[s].time;
	int i = s-1;
	int j = e+1;
	while(true)
	{
		do
		{
			--j;
		}while(arr[j].time > pivot);
		do
		{
			++i;
		}while(arr[i].time < pivot);
		if(i < j)
		{
			swap(arr[i], arr[j]);
		}
		else
		{
			return j;
		}
	}
}

void quickSort(person arr[], int s, int e)
{
	int p = 0;
	if(s < e && !sorted)
	{
		p = partition(arr, s, e);
		quickSort(arr, s, p);
		quickSort(arr, p+1, e);
	}
}

int bridgeQes01()
{
	quickSort(g_arr, 0, n-1);
	sorted = true;
	int totalTime = 0;
	for(int i = 1; i < n-1; ++i)
	{
		totalTime += g_arr[i].time;
		totalTime += g_arr[0].time;
	}
	totalTime += g_arr[n-1].time;
	return totalTime;
}

void proOne(int i, int& total)
{
	total += g_arr[i].time;
	total += g_arr[0].time;
	total += g_arr[i-1].time;
	total += g_arr[0].time;
}

void proTwo(int i, int& total)
{
	total += (g_arr[1].time + g_arr[0].time + g_arr[i].time + g_arr[1].time);
}

void proTwo2(int k_limit,int& total )
{
	int curIdx = 0;
	int i =  n-1;

	while(curIdx+1 < i)
	{
		if(k_limit - g_arr[curIdx].k <= 2)
		{
			if(k_limit - g_arr[curIdx+1].k <= 2)
			{
				total += 2*g_arr[curIdx+2].time;
				g_arr[curIdx+2].k += 2;
			}
			else
			{
				total += 2*g_arr[curIdx+1].time;
				g_arr[curIdx+1].k += 2;
			}
			++curIdx;
		}
		else
		{
			total += g_arr[curIdx+1].time + g_arr[curIdx].time;
			total += g_arr[i].time + g_arr[curIdx + 1].time;
			i -= 2;
			g_arr[curIdx].k += 2;
			g_arr[curIdx + 1].k += 2;
		}
	}
	total += g_arr[curIdx+1].time;
}
int bridgeQes1()
{
	quickSort(g_arr, 0, n-1);
	sorted = true;
	int totalTime = 0;
	int i = n - 1 ;
	while(i>2)
	{
		if(2*g_arr[1].time > g_arr[0].time + g_arr[i-1].time)
			proOne(i, totalTime);
		else
			proTwo(i, totalTime);
		i -= 2;
	}
	if(i == 2)
	{
		totalTime += g_arr[i].time;
		totalTime += g_arr[0].time;
	}
	totalTime += g_arr[1].time ;//+ g_arr[0].time;


	return totalTime;
}

void crossBridge(int idx)
{
	if(g_arrBank[idx])
	{
		cout<<"ha? "<<idx<<" you are going to cross the Bridge again!"<<endl;
	}
	++g_arrBank[idx];

}

void proOne3(int k_limit,int& total,int& curIdx, int& i)
{
    crossBridge(i);//过桥了
    total += g_arr[i--].time + g_arr[curIdx].time;
    g_arr[curIdx].k += 2;
    if(k_limit - g_arr[curIdx].k > 2)
     {
         crossBridge(i);//过桥了
         total += g_arr[i--].time + g_arr[curIdx].time;
     }
    else
    {
        crossBridge(curIdx);//过桥了
        ++curIdx;
        total += 2*g_arr[curIdx].time;
        g_arr[curIdx].k += 2;
    }

}

void proTwo3(int k_limit,int& total,int& curIdx, int& i)
{
    crossBridge(i);//过桥了
    crossBridge(i-1);//过桥了
    total += g_arr[curIdx+1].time + g_arr[curIdx].time;
    total += g_arr[i].time;
    total += g_arr[curIdx + 1].time;
    i -= 2;
    g_arr[curIdx].k += 2;
    g_arr[curIdx + 1].k += 2;
}

int bridgeQes2(int k_limit)
{
	quickSort(g_arr, 0, n-1);
	sorted = true;
	int totalTime = 0;
	int i = n-1;
	int curIdx = 0;
    while(curIdx+1 < i-1)
	{
        if(k_limit - g_arr[curIdx].k <= 2)
		{
			if(k_limit - g_arr[curIdx+1].k <= 2)
			{
				totalTime += 2*g_arr[curIdx+2].time;
				g_arr[curIdx+2].k += 2;
			}
			else
			{
				totalTime += 2*g_arr[curIdx+1].time;
				g_arr[curIdx+1].k += 2;
			}
			crossBridge(curIdx);//过桥了
			++g_arr[curIdx].k;
			++curIdx;
		}
		else
        {
            if(2*g_arr[curIdx + 1].time > g_arr[curIdx].time + g_arr[i-1].time)
                proOne3(k_limit, totalTime, curIdx, i);
            else
                proTwo3(k_limit, totalTime, curIdx, i);

            if(k_limit - g_arr[curIdx].k <= 2)
            {
                if(k_limit - g_arr[curIdx+1].k <= 2)
                {
                    totalTime += 2*g_arr[curIdx+2].time;
                    g_arr[curIdx+2].k += 2;
                }
                else
                {
                    totalTime += 2*g_arr[curIdx+1].time;
                    g_arr[curIdx+1].k += 2;
                }
                crossBridge(curIdx);//过桥了
                ++g_arr[curIdx].k;
                ++curIdx;
            }
        }
	}
	if(curIdx == i)
    {
        crossBridge(curIdx);

        ++g_arr[curIdx].k;

        totalTime += g_arr[curIdx].time;
    }
    else if(curIdx + 1 == i - 1)
    {
        crossBridge(curIdx+1);
        crossBridge(i);
        crossBridge(curIdx);

        ++g_arr[curIdx].k;
        ++g_arr[curIdx+1].k;
        ++g_arr[i].k;

        totalTime += g_arr[curIdx + 1].time + g_arr[curIdx].time;
        totalTime += g_arr[i].time;

    }
    else if(curIdx == i - 1)
    {
        crossBridge(curIdx);
        crossBridge(curIdx+1);

        ++g_arr[curIdx].k;
        ++g_arr[curIdx+1].k;

        totalTime += g_arr[curIdx+1].time;
    }
    else
    {
        cout<<"error~~~~~!!!!!"<<endl;
    }

    return totalTime;
}

int bridgeQes02(int k_limit)
{
	quickSort(g_arr, 0, n-1);
	sorted = true;
	person tmp = g_arr[0];
	int curIdx = 0;
	int counter = 0;
	int totalTime = 0;
	for(int i = 1; i < n-1; ++i)
	{
		if(counter < k_limit - 1)
		{
			totalTime += g_arr[i].time;
			totalTime += tmp.time;
			counter += 2;
		}
		else
		{
			totalTime += g_arr[i].time;
			tmp = g_arr[++curIdx];
			totalTime += tmp.time;
			counter = 2;
		}
	}
	totalTime += g_arr[n-1].time;
	return totalTime;
}
void printState()
{
    int i = 0;
    cout<<"state:"<<endl;
    while(i < n)
    {
        cout<<g_arrBank[i++];
    }
    cout<<endl;
}

void printTime()
{
    int i = 0;
    while(i < n)
    {
        cout<<g_arr[i++].time<<endl;
    }
}

void printK()
{
    int i = 0;
    cout<<"cross the bridge times:"<<endl;
    while(i < n)
    {
        cout<<g_arr[i++].k<<" ";
    }
}
int main()
{
	readData();
	int ttime1;
	ttime1 = bridgeQes1();
    cout<<"total time, no limit: "<<ttime1<<endl;
    ressetState();
	for(int i = 3; i<50; ++i)
    {
        kReset();
        ressetState();
        int ttime2 = bridgeQes2(i);
        if(i%2)
        {
            cout<<"total time, with k limit "<<i<<" : "<<ttime2<<endl;
            printState();
            printK();
        }
    }

   // printTime();
    //printState();
	//ttime3 = bridgeQes02(3);

}
