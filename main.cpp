/*������г���ʱ�����ڵĺ�����ʾΪ���������������Ҽ����ڣ��������й�ѡ ��ʹ�þɰ����̨����*/
#include"Huffman.h"
using namespace std;
int main()
{
	cout << "������������ַ�:";
	Huffman huff;
	huff.CreateCodeTable();
	cout << "��������" ;
    huff.Encode();
	cout << "��������";
	huff.Decode();
	cout << endl;
	huff.Analyze();
	cout << endl;
}
