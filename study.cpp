#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>

// 引入命名空间 std 中的 cout 函数
using std::cout;
using std::endl;

// typedef type alias;
typedef int IntType;

void SevenBasicType();
void EnumType();
void NameStandard();
void ConstDataType();
void LoopExpression();
void ReferValue(int &type);
void LambdaExpression();
void RandomData();
void ArrayExpression();
void Pointer();
void StructType();

int global_variable = 4; // 全局变量

int main()
{
	void (*function_ptr)() = &SevenBasicType;
	void (*function_ptr2)() = SevenBasicType;
	cout << SevenBasicType << endl;
	cout << &SevenBasicType << endl;
	cout << function_ptr << endl;
	cout << function_ptr2 << endl;
	function_ptr();
	function_ptr2();
	(*function_ptr2)();
	(*function_ptr)();
	(*SevenBasicType)();
	(&SevenBasicType)();

	return 0;
}

void SevenBasicType()
{							   // void 类型
	bool bool_type = false;	   // bool 类型
	char char_type = 's';	   // char 类型
	IntType int_type = 0;	   // int 类型
	float float_type = 0.0;	   // float 类型
	double double_type = 0.0;  // double 类型
	wchar_t wchart_t_type = 4; // 宽字符型 相当于 short int
	cout << "EEE" << endl;
}

void EnumType()
{
	enum class Color
	{
		kRed,
		kGreen,
		kBlue
	} color;
	color = Color::kBlue;
}

void NameStandard()
{
	// 文件名：my_name.cc
	// 常量：kMyName
	// 普通变量：my_name
	// 类数据变量：my_name_
	// 结构体变量：my_name
	// 函数名：MyName
	// 类型名：MyName
	// 枚举项：kMyName
	// 宏命名：MY_NAME
}

void ConstDataType()
{
	// 常量
	const int kLength = 5;
}

void LoopExpression()
{
	for (int i = 0; i < 5; i++)
	{
	}
	int i = 0;
	while (i < 5)
	{
		cout << "print: " + i << endl;
		i++;
	}
}

void ReferValue(int &type)
{
	// 按引用传值
	int int_val = 17;
	int &refer_value = int_val;
}

void LambdaExpression()
{
	auto func = [](int x, int y)
	{ return x < y; };
	/*
	 *   []      // 没有定义任何变量。使用未定义变量会引发错误。
	 *   [x, &y] // x以传值方式传入（默认），y以引用方式传入。
	 *   [&]     // 任何被使用到的外部变量都隐式地以引用方式加以引用。
	 *   [=]     // 任何被使用到的外部变量都隐式地以传值方式加以引用。
	 *   [&, x]  // x显式地以传值方式加以引用。其余变量以引用方式加以引用。
	 *   [=, &z] // z显式地以引用方式加以引用。其余变量以传值方式加以引用。
	 */
}

void RandomData()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < 10; i++)
	{
		int random_int = rand();
		cout << "随机数:  " << random_int << endl;
	}
}

void ArrayExpression()
{
	double data[5] = {1000.0, 2.0, 3.4, 7.0, 50.0};
	double *p = data;
	for (int i = 0; i < sizeof(data) / sizeof(double); i++)
	{
		cout << *p << endl;
		p++;
	}
}

void Pointer()
{
	int var1;
	char var2[10];

	cout << "var1 变量的地址： ";
	cout << &var1 << endl;

	cout << "var2 变量的地址： ";
	cout << &var2 << endl;
	cout << var2 << endl;
}

void StructType()
{
	typedef struct MyStruct
	{
		int val;
	} MyStruct;

	MyStruct my_struct;
	my_struct.val = 2;
	cout << my_struct.val << endl;
}
