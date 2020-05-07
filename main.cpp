/*如果运行程序时，窗口的汉字显示为“？？？”，请右键窗口，在属性中勾选 “使用旧版控制台”。*/
#include"Huffman.h"
using namespace std;
int main()
{
	cout << "请输入待编码字符:";
	Huffman huff;
	huff.CreateCodeTable();
	cout << "编码结果：" ;
    huff.Encode();
	cout << "解码结果：";
	huff.Decode();
	cout << endl;
	huff.Analyze();
	cout << endl;
}
