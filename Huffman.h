#pragma
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

//�����
struct TNode
{
	int weight;//Ȩֵ
	int lchild, rchild, parent;
};
//�������
struct CNode
{
	char data;//����������
	string code;//����
};

//Huffman������
class Huffman
{
private:
	TNode *node;//�����
	CNode *codeTable;//�洢�����
	int length;//�ַ�������
	int N; //Ҷ�������=��ͬ�ַ�����
	string str;//�洢�������ַ���
	int en_length;//�洢������ַ�������
	char name[128];//��stringת��Ϊ�ַ����鲢�洢
	void Initial();//��������ַ�������Ƶ��ͳ�ƣ���ʼ����Ӧ�Ĳ���
	void DouMin(int &x, int &y,int a,int b);//ȡȨֵ��С���������
	void Reverse(string &s);//���ñ���
public:
	Huffman();//����������������ӡ
	void CreateCodeTable();//��������ӡ�����
	void Encode();//���룬����ӡ���
	void Decode();//����,����ӡ���
	void Analyze();//����Huffman�����Ч��
	~Huffman();//�������ͷŶѿռ�
};

//��������ַ�������Ƶ��ͳ�ƣ���ʼ����Ӧ�Ĳ���
void Huffman::Initial()
{//ʱ�临�Ӷȣ�O(n)
	length = 0;
	int nNum[256] = { 0 };
	int ch = cin.get();
	//��ʼ��length��name[]
	while ((ch != '\r') && (ch != '\n'))
	{
		nNum[ch]++;//����ACSII��ͳ��Ƶ��
		name[length] = (char)ch;//�洢�ַ�����ÿ���ַ�
		length++;
		ch = cin.get();//��ȡ��һ���ַ�
	}
	//��ʼ��N--�ַ�����
	N = 0;
	for (int i = 0; i < 256; i++)
	{
		if (nNum[i] != 0)N++;
	}
	//��ʼ�������ͱ������
	node = new TNode[2 * N - 1];//����Huffman�����
	codeTable = new CNode[N];//�����������
	int i = 0;
	for (int j = 0; j < 256; j++)
	{
		if (nNum[j] != 0)
		{
			node[i].weight = nNum[j];
			node[i].lchild = -1;//��ʼ����㣬���Ϊδʹ��
			node[i].rchild = -1;
			node[i].parent = -1;
			codeTable[i].data = (char)j;
			codeTable[i].code = "";
			i++;
		}
	}
}

//ȡȨֵ��С���������
void Huffman::DouMin(int &x,int &y,int a,int b)
{//ʱ�临�Ӷȣ�O(n)
	struct tempNode
	{
		int weight;//Ȩ��
		int position;//λ��
	};
	tempNode *temp=new tempNode[b];
	int j = 0;
	for (int i = a; i < b; i++)
	{//ѡ�����п�ʹ�õĽ�㲢��temp[]�洢
		if (node[i].parent == -1)
		{
			temp[j].weight = node[i].weight;
			temp[j].position = i;
			j++;
		}
	}
	int m1 = 0 , m2 = 0;
	//���һ����Сֵ
	for (int i = 0; i < j; i++)
		if (temp[i].weight < temp[m1].weight)
			m1 = i;
	//��ڶ�����Сֵ	
	for (int i = 0; i < j; i++)
	{
		if (m1 == m2) 
			m2++;
		if (temp[i].weight <= temp[m2].weight)//ע������Ҫȡ�Ⱥ�
			m2 = i;
	}
	x = temp[m1].position;
	y = temp[m2].position;
	delete[]temp;
}

//����������������ӡ
Huffman::Huffman()
{//ʱ�临�Ӷȣ�O(n)
	Initial();//��ʼ����������������ݳ�Ա
	if (N <= 1)//�쳣����
		cout<<"Error!Cannot create a Huffman tree!";
	int x=0, y=0;
	for (int i = N; i < 2 * N - 1; i++)
	{//����Huffman������̬��
		DouMin(x, y,0,i);//ѡ��Ȩֵ��С��������㣬��x<y
		node[x].parent = i;
		node[y].parent = i;
		node[i].weight = node[x].weight + node[y].weight;
		node[i].lchild = x;
		node[i].rchild = y;
		node[i].parent = -1;
	}
	cout << "Huffman tree: " << endl;
	//��ӡHuffman���ľ�̬���
	for (int i = 0; i < 2 * N - 1; i++)
	{
		cout << "-----------------------" << endl;
		cout << setiosflags(ios::left) << setw(3) << i << "��" << " " << setw(3) << node[i].weight
			<< "��" << " " << setw(3) << node[i].lchild << "��" << " " << setw(3) << node[i].rchild
			<< "��" << " " << setw(3) << node[i].parent << endl;
	}
	cout << "-----------------------" << endl;
}

//���ñ���
void Huffman::Reverse(string &s) 
{//ʱ�临�Ӷȣ�O(n)
	int length = s.size();
	char temp;
	for (int i = 0; i < length / 2; i++) 
	{
		temp = s[i];
		s[i] = s[length - i - 1];
		s[length - i - 1] = temp;
	}
}

//��������ӡ�����
void Huffman::CreateCodeTable()
{//ʱ�临�Ӷȣ�O(n^2)
	cout << "The code table is: " << endl;
	for (int i = 0; i < N; i++)
	{
		int child = i;//��Ҷ��㿪ʼ����,�����ñ���
		int parent = node[i].parent;
		int temp = 0;
		while (node[child].parent != -1)//�ж��Ƿ�Ϊ�����
		{
			if (child == node[parent].lchild)//���ӣ�����ȡ0
				codeTable[i].code += '0';
			else if(child == node[parent].rchild)//�Һ��ӣ�����ȡ1
				codeTable[i].code += '1';
			temp = child;
			child = parent;
			parent = node[temp].parent;
		}
		Reverse(codeTable[i].code);//����
		//��ӡ�����
		cout << "------------" << endl;
		cout << setiosflags(ios::left) << ' ' << setw(4) << codeTable[i].data
			<< "��" << ' ' << setw(4) << codeTable[i].code << endl;
	}
	cout << "------------" << endl;
}

//���룬����ӡ���
void Huffman::Encode()
{//ʱ�临�Ӷȣ�O(n)
	string en_str;//������
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

//����,����ӡ���
void Huffman::Decode()
{//ʱ�临�Ӷȣ�O(n)
	string de_str;
	int x = str.length();
	for (int i = 0; i < x; i++)
	{
		int parent = 2 * N - 2;//���ڵ��ھ�̬�����������е�����
		int j = 0;
		while (node[parent].lchild != -1)//����Ҷ���
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

//����Huffman�����Ч��
void Huffman::Analyze()
{
	cout << "����ǰ�ַ�����ռ�ڴ棨��λ��bit����" << length*8<<endl;
	cout << "������ַ�����ռ�ڴ棨�����ö����Ʊ��룬��λ��bit����" << en_length*8<<endl;
	cout << "������ַ�����ռ�ڴ棨������ö����Ʊ��룬��λ��bit����" << en_length<<endl;
	cout << "���ۣ�ֻ�в��ö����Ʊ������ѹ��Ч����" << endl;
}

//�������ͷŶѿռ�
Huffman::~Huffman()
{//ʱ�临�Ӷȣ�O(1)
	delete[]node;
	delete[]codeTable;
}