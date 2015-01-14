#pragma once
class test_variableTemplate
{
public:
	test_variableTemplate(void);
	~test_variableTemplate(void);
};
template<int N>
struct TAddOne
{
	enum{Value=N,};
};
template<typename T>
struct IsVoidType
{
	enum {is_Void = 1};
};
template<>
struct IsVoidType<void>
{
	enum {is_Void = 0};
};
template<typename T>
void addType(int& sum)
{
	sum+=TAddOne<IsVoidType<T>::is_Void>::Value;
}
