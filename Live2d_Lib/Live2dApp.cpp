#include <LAppDelegate.hpp>
#include <string>
#include <Live2dApp.h>
#include <LAppLive2DManager.hpp>
#include <LAppView.hpp>

using namespace System::Runtime::InteropServices;

typedef System::IntPtr(__stdcall *LoadFileCall)(System::String^ filePath, unsigned int% outSize);
LoadFileCall _loadfile;



public ref class Live2dApp
{
public:
	delegate System::IntPtr(LoadFile)(System::String^ filePath, unsigned int% outSize);
	static LoadFile^ _loadfile;

	void SetCall(LoadFile^ loadfile)
	{
		_loadfile = loadfile;
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
		LAppLive2DManager::GetInstance()->ChangeScene(dir, name);
	}

	void SetPosX(float value) 
	{
		LAppView*  view = LAppDelegate::GetInstance()->GetView();
		view->ViewPosX(value);
	}

	void SetPosY(float value)
	{
		LAppView* view = LAppDelegate::GetInstance()->GetView();
		view->ViewPosY(value);
	}

	void SetScale(float value)
	{
		LAppView* view = LAppDelegate::GetInstance()->GetView();
		view->ScaleView(value);
	}

	void SetScale(float x, float y)
	{
		LAppView* view = LAppDelegate::GetInstance()->GetView();
		view->ScaleView(x, y);
	}
};

System::IntPtr LoadFileDO(System::String^ filePath, Csm::csmSizeInt* outSize)
{
	return Live2dApp::_loadfile(filePath, *outSize);
}