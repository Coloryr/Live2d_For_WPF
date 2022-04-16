#include <LAppDelegate.hpp>
#include <string>
#include <Live2dApp.h>
#include <LAppLive2DManager.hpp>
#include <LAppView.hpp>
#include <LAppModel.hpp>
#include <Motion/ACubismMotion.hpp>

using namespace System::Runtime::InteropServices;

char* MotionGroupIdle; // アイドリング
char* MotionGroupTapBody; // 悶をタップしたとき

char* HitAreaNameHead;
char* HitAreaNameBody;

public ref class Live2dApp
{
private:
	void InitName() 
	{
		MotionGroupIdle = (char*)(void*)Marshal::StringToHGlobalAnsi(gcnew System::String("Idle"));
		MotionGroupTapBody = (char*)(void*)Marshal::StringToHGlobalAnsi(gcnew System::String("TapBody"));
		HitAreaNameHead = (char*)(void*)Marshal::StringToHGlobalAnsi(gcnew System::String("Head"));
		HitAreaNameBody = (char*)(void*)Marshal::StringToHGlobalAnsi(gcnew System::String("Body"));
	}
public:
	delegate System::IntPtr(LoadFile)(System::String^ filePath, unsigned int% outSize);
	delegate void(LoadDone)(System::String^ filePath);
	static LoadFile^ _loadfile;
	static LoadDone^ _loaddone;

	Live2dApp(LoadFile^ loadfile, LoadDone^ loaddone)
	{
		InitName();

		_loadfile = loadfile;
		_loaddone = loaddone;
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
		LAppLive2DManager::GetInstance()->IsLoad = true;
		Marshal::FreeHGlobal((System::IntPtr)MotionGroupIdle);
		Marshal::FreeHGlobal((System::IntPtr)MotionGroupTapBody);
		Marshal::FreeHGlobal((System::IntPtr)HitAreaNameHead);
		Marshal::FreeHGlobal((System::IntPtr)HitAreaNameBody);

		InitName();

		name += ".model3.json";
		LAppLive2DManager::GetInstance()->ChangeScene(dir, name);

		LAppLive2DManager::GetInstance()->IsLoad = false;
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
	array<System::String^>^ GetExpressions()
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return nullptr;
		LAppModel*  model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return nullptr;
		Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>* list = model->GetExpressions();
		array<System::String^>^ list1 = gcnew array<System::String^>(list->GetSize());
		int a = 0;
		for (Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>::const_iterator iter = list->Begin(); iter != list->End(); ++iter)
		{
			list1[a++] = gcnew System::String(iter->First.GetRawString());
		}

		return list1;
	}
	array<System::String^>^ GetMotions()
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return nullptr;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return nullptr;
		Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>* list = model->GetMotions();
		array<System::String^>^ list1 = gcnew array<System::String^>(list->GetSize());
		int a = 0;
		for (Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>::const_iterator iter = list->Begin(); iter != list->End(); ++iter)
		{
			list1[a++] = gcnew System::String(iter->First.GetRawString());
		}

		return list1;
	}
	bool StartExpression(int index)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return false;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return false;
		Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>* list = model->GetExpressions();
		if (list->GetSize() <= index)
			return false;
		Csm::csmString* item = nullptr;
		for (Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>::const_iterator iter = list->Begin(); iter != list->End(); ++iter)
		{
			if (index == 0)
			{
				item = &iter->First;
				break;
			}
			index--;
		}
		if (item == nullptr)
			return false;
		model->SetExpression(item->GetRawString());

		return true;
	}
	bool StartMotion(System::String^ group, int no, int priority)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return false;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return false;
		char* str1 = (char*)(void*)Marshal::StringToHGlobalAnsi(group);

		model->StartMotion(str1, no, priority);

		Marshal::FreeHGlobal((System::IntPtr)str1);

		return true;
	}
	void SetRandomMotion(bool value) 
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->NeedRandom = value;
	}
	void SetIdParamAngleX(System::String^ name)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->SetIdParamAngleX(name);
	}
	void SetIdParamAngleY(System::String^ name)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->SetIdParamAngleY(name);
	}
	void SetIdParamAngleZ(System::String^ name)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->SetIdParamAngleZ(name);
	}
	void SetIdParamBodyAngleX(System::String^ name)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->SetIdParamBodyAngleX(name);
	}
	void SetIdParamEyeBallX(System::String^ name)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->SetIdParamEyeBallX(name);
	}
	void SetIdParamEyeBallY(System::String^ name)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->SetIdParamEyeBallY(name);
	}
	void SetHitAreaNameHead(System::String^ name)
	{
		Marshal::FreeHGlobal((System::IntPtr)HitAreaNameHead);
		HitAreaNameHead = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
	}
	void SetHitAreaNameBody(System::String^ name)
	{
		Marshal::FreeHGlobal((System::IntPtr)HitAreaNameBody);
		HitAreaNameBody = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
	}
	void SetMotionGroupIdle(System::String^ name)
	{
		Marshal::FreeHGlobal((System::IntPtr)MotionGroupIdle);
		MotionGroupIdle = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
	}
	void SetMotionGroupTapBody(System::String^ name)
	{
		Marshal::FreeHGlobal((System::IntPtr)MotionGroupTapBody);
		MotionGroupTapBody = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
	}
};

System::IntPtr LoadFileDO(System::String^ filePath, Csm::csmSizeInt* outSize)
{
	return Live2dApp::_loadfile(filePath, *outSize);
}

void LoadDone(System::String^ file)
{
	return Live2dApp::_loaddone(file);
}