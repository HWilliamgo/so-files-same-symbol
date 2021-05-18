#include <iostream>
#include <dlfcn.h>

using namespace std;

static const char *const LIB_PATH = "./libcaculate.so";
static const char *const LIB_FAKE_PATH = "./libfake_cal.so";

typedef int (*CACULATE_FUNC)(int, int);

int main()
{

	void *symAdd = nullptr;
	char *errorMsg = nullptr;

	void *symAddFake = nullptr;

	dlerror();
	//1.打开动态库，拿到一个动态库句柄
	void *handle = dlopen(LIB_PATH, RTLD_NOW);
	void *handleFake = dlopen(LIB_FAKE_PATH, RTLD_NOW);

	if (handle == nullptr)
	{
		cout << "load error!" << endl;
		return -1;
	}
	if (handleFake == nullptr)
	{
		cout << "load handleFake error!" << endl;
		return -1;
	}

	// 查看是否有错误
	if ((errorMsg = dlerror()) != nullptr)
	{
		cout << "errorMsg:" << errorMsg << endl;
		return -1;
	}

	cout << "load success!" << endl;

	//2.通过句柄和方法名获取方法指针地址
	symAdd = dlsym(handle, "add");
	if (symAdd == nullptr)
	{
		cout << "dlsym failed!" << endl;
		if ((errorMsg = dlerror()) != nullptr)
		{
			cout << "error message:" << errorMsg << endl;
			return -1;
		}
	}
	symAddFake = dlsym(handleFake, "add");
	if (symAddFake == nullptr)
	{
		cout << "dlsym symAddFake failed!" << endl;
		if ((errorMsg = dlerror()) != nullptr)
		{
			cout << "error symAddFake  message:" << errorMsg << endl;
			return -1;
		}
	}

	//3.将方法地址强制类型转换成方法指针
	CACULATE_FUNC addFunc = reinterpret_cast<CACULATE_FUNC>(symAdd);
	CACULATE_FUNC addFuncFake = reinterpret_cast<CACULATE_FUNC>(symAddFake);
	//4.调用动态库中的方法
	cout << "1 + 2 = " << addFunc(1, 2) << endl;
	cout << "1 + 2 = " << addFuncFake(1, 2) << endl;
	//5.通过句柄关闭动态库
	dlclose(handle);
	dlclose(handleFake);
	return 0;
}
