#pragma
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

//树结点
struct TNode
{
	int weight;//权值
	int lchild, rchild, parent;
};
//编码表结点
struct CNode
{
	char data;//被编码内容
	string code;//编码
};

//Huffman编码类
class Huffman
{
private:
	TNode *node;//树结点
	CNode *codeTable;//存储编码表
	int length;//字符串长度
	int N; //叶结点数量=不同字符种类
	string str;//存储编码后的字符串
	int en_length;//存储编码后字符串长度
	char name[128];//将string转化为字符数组并存储
	void Initial();//对输入的字符串进行频次统计，初始化相应的参数
	void DouMin(int &x, int &y,int a,int b);//取权值最小的两个结点
	void Reverse(string &s);//逆置编码
public:
	Huffman();//创建哈夫曼树并打印
	void CreateCodeTable();//创建并打印编码表
	void Encode();//编码，并打印结果
	void Decode();//解码,并打印结果
	void Analyze();//分析Huffman编码的效果
	~Huffman();//析构，释放堆空间
};

//对输入的字符串进行频次统计，初始化相应的参数
void Huffman::Initial()
{//时间复杂度：O(n)
	length = 0;
	int nNum[256] = { 0 };
	int ch = cin.get();
	//初始化length和name[]
	while ((ch != '\r') && (ch != '\n'))
	{
		nNum[ch]++;//利用ACSII码统计频次
		name[length] = (char)ch;//存储字符串中每个字符
		length++;
		ch = cin.get();//读取下一个字符
	}
	//初始化N--字符种类
	N = 0;
	for (int i = 0; i < 256; i++)
	{
		if (nNum[i] != 0)N++;
	}
	//初始化树结点和编码表结点
	node = new TNode[2 * N - 1];//构建Huffman树结点
	codeTable = new CNode[N];//创建编码表结点
	int i = 0;
	for (int j = 0; j < 256; j++)
	{
		if (nNum[j] != 0)
		{
			node[i].weight = nNum[j];
			node[i].lchild = -1;//初始化结点，标记为未使用
			node[i].rchild = -1;
			node[i].parent = -1;
			codeTable[i].data = (char)j;
			codeTable[i].code = "";
			i++;
		}
	}
}

//取权值最小的两个结点
void Huffman::DouMin(int &x,int &y,int a,int b)
{//时间复杂度：O(n)
	struct tempNode
	{
		int weight;//权重
		int position;//位置
	};
	tempNode *temp=new tempNode[b];
	int j = 0;
	for (int i = a; i < b; i++)
	{//选出所有可使用的结点并用temp[]存储
		if (node[i].parent == -1)
		{
			temp[j].weight = node[i].weight;
			temp[j].position = i;
			j++;
		}
	}
	int m1 = 0 , m2 = 0;
	//求第一个最小值
	for (int i = 0; i < j; i++)
		if (temp[i].weight < temp[m1].weight)
			m1 = i;
	//求第二个最小值	
	for (int i = 0; i < j; i++)
	{
		if (m1 == m2) 
			m2++;
		if (temp[i].weight <= temp[m2].weight)//注意这里要取等号
			m2 = i;
	}
	x = temp[m1].position;
	y = temp[m2].position;
	delete[]temp;
}

//创建哈夫曼树并打印
Huffman::Huffman()
{//时间复杂度：O(n)
	Initial();//初始化哈夫曼树相关数据成员
	if (N <= 1)//异常处理
		cout<<"Error!Cannot create a Huffman tree!";
	int x=0, y=0;
	for (int i = N; i < 2 * N - 1; i++)
	{//创建Huffman树（静态）
		DouMin(x, y,0,i);//选出权值最小的两个结点，且x<y
		node[x].parent = i;
		node[y].parent = i;
		node[i].weight = node[x].weight + node[y].weight;
		node[i].lchild = x;
		node[i].rchild = y;
		node[i].parent = -1;
	}
	cout << "Huffman tree: " << endl;
	//打印Huffman树的静态表格
	for (int i = 0; i < 2 * N - 1; i++)
	{
		cout << "-----------------------" << endl;
		cout << setiosflags(ios::left) << setw(3) << i << "│" << " " << setw(3) << node[i].weight
			<< "│" << " " << setw(3) << node[i].lchild << "│" << " " << setw(3) << node[i].rchild
			<< "│" << " " << setw(3) << node[i].parent << endl;
	}
	cout << "-----------------------" << endl;
}

//逆置编码
void Huffman::Reverse(string &s) 
{//时间复杂度：O(n)
	int length = s.size();
	char temp;
	for (int i = 0; i < length / 2; i++) 
	{
		temp = s[i];
		s[i] = s[length - i - 1];
		s[length - i - 1] = temp;
	}
}

//创建并打印编码表
void Huffman::CreateCodeTable()
{//时间复杂度：O(n^2)
	cout << "The code table is: " << endl;
	for (int i = 0; i < N; i++)
	{
		int child = i;//从叶结点开始编码,再逆置编码
		int parent = node[i].parent;
		int temp = 0;
		while (node[child].parent != -1)//判断是否为根结点
		{
			if (child == node[parent].lchild)//左孩子，编码取0
				codeTable[i].code += '0';
			else if(child == node[parent].rchild)//右孩子，编码取1
				codeTable[i].code += '1';
			temp = child;
			child = parent;
			parent = node[temp].parent;
		}
		Reverse(codeTable[i].code);//逆置
		//打印编码表
		cout << "------------" << endl;
		cout << setiosflags(ios::left) << ' ' << setw(4) << codeTable[i].data
			<< "│" << ' ' << setw(4) << codeTable[i].code << endl;
	}
	cout << "------------" << endl;
}

//编码，并打印结果
void Huffman::Encode()
{//时间复杂度：O(n)
	string en_str;//编码结果
	for (int i = 0; i < length; i++)
	{
		for(int j = 0; j < N; j++)
		{
			if (name[i] == codeTable[j].data)
			{
				en_str += codeTable[j].code;
			}
		}
	}
	str = en_str;
	cout << en_str << endl;
	en_length = en_str.length();
}

//解码,并打印结果
void Huffman::Decode()
{//时间复杂度：O(n)
	string de_str;
	int x = str.length();
	for (int i = 0; i < x; i++)
	{
		int parent = 2 * N - 2;//根节点在静态哈夫曼树表中的索引
		int j = 0;
		while (node[parent].lchild != -1)//不是叶结点
		{
			if (str[i + j] == '0')
				parent = node[parent].lchild;
			else
				parent = node[parent].rchild;
			j++;
		}
		i += j - 1;
		de_str += codeTable[parent].data;
	}
	cout << de_str << endl;
}

//分析Huffman编码的效果
void Huffman::Analyze()
{
	cout << "编码前字符串所占内存（单位：bit）：" << length*8<<endl;
	cout << "编码后字符串所占内存（不采用二进制编码，单位：bit）：" << en_length*8<<endl;
	cout << "编码后字符串所占内存（假设采用二进制编码，单位：bit）：" << en_length<<endl;
	cout << "结论：只有采用二进制编码才有压缩效果。" << endl;
}

//析构，释放堆空间
Huffman::~Huffman()
{//时间复杂度：O(1)
	delete[]node;
	delete[]codeTable;
}