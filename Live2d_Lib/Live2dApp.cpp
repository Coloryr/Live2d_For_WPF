#include <LAppDelegate.hpp>
#include <string>
#include <Live2dApp.h>
#include <LAppLive2DManager.hpp>

using namespace System::Runtime::InteropServices;

typedef System::IntPtr(__stdcall *LoadFileCall)(System::String^ filePath, unsigned int% outSize);
LoadFileCall _loadfile;

Csm::csmByte* LoadFileDO(std::string filePath, Csm::csmSizeInt* outSize)
{
	System::IntPtr temp = _loadfile(gcnew System::String(filePath.c_str()), *outSize);
	return (Csm::csmByte*)(void*)temp;
}

public ref class Live2dApp
{
public:
	delegate System::IntPtr(LoadFile)(System::String^ filePath, unsigned int% outSize);

	void SetCall(LoadFile^ loadfile)
	{
		System::IntPtr ip = Marshal::GetFunctionPointerForDelegate(loadfile);
		_loadfile = (LoadFileCall)(ip.ToPointer());
	}
	bool Start(int width, int height)
	{
		return LAppDelegate::GetInstance()->Initialize(width, height) != GL_FALSE;
	}

	void Tick(int width, int height, double time)
	{
		LAppDelegate::GetInstance()->Run(width, height, time);
	}

	void Close() 
	{
		LAppDelegate::GetInstance()->Release();
		LAppDelegate::ReleaseInstance();
	}

	void MouseEvent(int action) 
	{
		LAppDelegate::GetInstance()->OnMouseCallBack(action);
	}

	void MouseMove(double x, double y) 
	{
		LAppDelegate::GetInstance()->OnMouseCallBack(x, y);
	}

	void LoadModel(System::String^ dir, System::String^ name)
	{
		name += ".model3.json";
		char* str1 = (char*)(void*)Marshal::StringToHGlobalAnsi(dir);
		char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
		LAppLive2DManager::GetInstance()->ChangeScene(Csm::csmString(str1), Csm::csmString(str2));
		Marshal::FreeHGlobal((System::IntPtr)str1);
		Marshal::FreeHGlobal((System::IntPtr)str2);
	}
};
