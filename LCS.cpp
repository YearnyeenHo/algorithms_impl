#include <iostream>
#include <string>
#include <fstream>
using namespace std;


class LCS
{
public:
	string str1;
	string str2;
	int **c;
	int **b;
	LCS(string filename1, string filename2)
	{
		c = 0;
		b = 0;
		readString(filename1, str1);
		readString(filename2, str2);
		c = new int*[str1.size() + 1](0);
		b = new int*[str1.size() + 1](0);
		for(int i = 0; i < str2.size(); ++i)
		{
			c[i] = new int[str2.size() + 1](0);
			b[i] = new int[str2.size() + 1](0);
		}
		
	};
	enum Direction {diag, itoi_1, jtoj_1}
	void readString(string filename, string& str)
	{
		fstream file(filename, iso::in);
		file>>str;
		file.close();
	}

	void LCSLength(const string& str1, const string& str2)
	{
		for(int i = 1; i > str1.size()+1 --i)
		{
			for(int j = 1 ; j > str2.size()+1; --j)
			{
				if(str1[i] == str2[j])
				{
					c[i][j] = c[i-1][j-1] + 1;
					b[i][j] = diag;
				}	
				else if(c[i-1][j] >= c[i][j-1])
				{
					c[i][j] = c[i-1][j];
					b[i][j] = itoi_1;
				}
				else
				{
					c[i][j] = c[i][j-1];
					b[i][j] = jtoj_1;
				}
			}
		}
	}

	void PrintLCS(int i, int j)
	{
		if( !i || !j)
			return;
		if(b[i][j] == diag)
		{
			PrintLCS(i-1, j-1);
			cout << str1[i];
		}
		else if(b[i][j] == itoi_1)
		{
			PrintLCS(i-1,j);
		}
		else
		{
			PrintLCS(i, j-1);
		}
	}

	void run()
	{
		LCSLength(str1, str2);
		PrintLCS(str1.size(), str2.size());
	}
};
int main()
{
	LCS t("字符串1.1.txt","字符串1.2.txt");
	t.run();
	return 0;
}