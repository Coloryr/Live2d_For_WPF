#include <LAppDelegate.hpp>
#include <string>
#include <Live2dApp.h>
#include <LAppLive2DManager.hpp>
#include <LAppView.hpp>
#include <LAppModel.hpp>
#include <Motion/ACubismMotion.hpp>

using namespace System::Runtime::InteropServices;

char* MotionGroupIdle; // �����ɥ��
char* MotionGroupTapBody; // ��򥿥åפ����Ȥ�

char* HitAreaNameHead;
char* HitAreaNameBody;

public ref class Motion
{
public:
	/// <summary>
	/// Ⱥ����
	/// </summary>
	System::String^ Group;
	/// <summary>
	/// Ⱥ��ID
	/// </summary>
	int Number;

	System::String^ ToString() override
	{
		return gcnew System::String("") + Group + "_" + Number;
	}
};

public ref class Parameter
{
public:
	/// <summary>
	/// ID
	/// </summary>
	System::String^ Id;
	/// <summary>
	/// ��ǰֵ
	/// </summary>
	float Value;
	/// <summary>
	/// ���ֵ
	/// </summary>
	float MaximumValue;
	/// <summary>
	/// ��Сֵ
	/// </summary>
	float MinimumValue;
	/// <summary>
	/// Ĭ��
	/// </summary>
	float DefaultValue;

	System::String^ ToString() override
	{
		return gcnew System::String("Id:") + Id + " Value:" + Value;
	}
};

public ref class Part
{
public :
	System::String^ Id;
	float Opacitie;

	System::String^ ToString() override
	{
		return gcnew System::String("Id:") + Id + " Opacitie:" + Opacitie;
	}
};

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
	delegate void(Update)();
	static LoadFile^ _loadfile;
	static LoadDone^ _loaddone;
	static Update^ _update;

	Live2dApp(LoadFile^ loadfile, LoadDone^ loaddone, Update^ update)
	{
		InitName();

		_loadfile = loadfile;
		_loaddone = loaddone;
		_update = update;
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
		LAppView* view = LAppDelegate::GetInstance()->GetView();
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
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
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
	array<Motion^>^ GetMotions()
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return nullptr;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return nullptr;
		Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>* list = model->GetMotions();
		array<Motion^>^ list1 = gcnew array<Motion^>(list->GetSize());
		int a = 0;
		for (Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>::const_iterator iter = list->Begin(); iter != list->End(); ++iter)
		{
			Motion^ item = gcnew Motion();
			array<System::String^>^ temp = (gcnew System::String(iter->First.GetRawString()))->Split('_');
			item->Group = temp[0];
			item->Number = System::Int32::Parse(temp[1]);
			list1[a++] = item;
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
	array<Parameter^>^ GetParameters()
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return nullptr;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return nullptr;

		Live2D::Cubism::Framework::CubismModel* model1 = model->GetModel();
		if (model1 == nullptr)
			return nullptr;
		int count = model1->GetParameterCount();
		array<Parameter^>^ list = gcnew array<Parameter^>(count);
		Csm::csmFloat32* values = model1->ParameterValues();
		Csm::csmVector<Csm::CubismIdHandle>* ids = model1->ParameterIds();
		for (int i = 0; i < count; i++)
		{
			Parameter^ item = gcnew Parameter();
			item->Id = gcnew System::String(ids->At(i)->GetString().GetRawString());
			item->Value = values[i];
			item->MaximumValue = model1->GetParameterMaximumValue(i);
			item->MinimumValue = model1->GetParameterMinimumValue(i);
			item->DefaultValue = model1->GetParameterDefaultValue(i);
			list[i] = item;
		}

		return list;
	}
	array<Part^>^ GetParts()
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return nullptr;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return nullptr;

		Live2D::Cubism::Framework::CubismModel* model1 = model->GetModel();
		if (model1 == nullptr)
			return nullptr;
		int count = model1->GetPartCount();
		array<Part^>^ list = gcnew array<Part^>(count);
		Csm::csmFloat32* values = model1->PartOpacities();
		Csm::csmVector<Csm::CubismIdHandle>* ids = model1->PartIds();
		for (int i = 0; i < count; i++)
		{
			Part^ item = gcnew Part();
			item->Id = gcnew System::String(ids->At(i)->GetString().GetRawString());
			item->Opacitie = values[i];
			list[i] = item;
		}

		return list;
	}
	bool AddParameterValue(System::String^ name, float value)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return false;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return false;

		Live2D::Cubism::Framework::CubismModel* model1 = model->GetModel();
		if (model1 == nullptr)
			return false;

		char* str1 = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
		bool res = false;
		Csm::csmVector<Csm::CubismIdHandle>* ids = model1->ParameterIds();
		for (int a = 0; a < ids->GetSize(); a++)
		{
			Csm::CubismIdHandle item = ids->At(a);
			if (item->GetString() == str1)
			{
				model1->AddParameterValue(item, value);
				res = true;
				break;
			}
		}

		Marshal::FreeHGlobal((System::IntPtr)str1);
		return res;
	}

	bool SetPartOpacitie(System::String^ name, float value)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return false;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return false;

		Live2D::Cubism::Framework::CubismModel* model1 = model->GetModel();
		if (model1 == nullptr)
			return false;

		char* str1 = (char*)(void*)Marshal::StringToHGlobalAnsi(name);
		bool res = false;
		Csm::csmVector<Csm::CubismIdHandle>* ids = model1->PartIds();
		for (int a = 0; a < ids->GetSize(); a++)
		{
			Csm::CubismIdHandle item = ids->At(a);
			if (item->GetString() == str1)
			{
				model1->SetPartOpacity(item, value);
				res = true;
				break;
			}
		}

		Marshal::FreeHGlobal((System::IntPtr)str1);
		return res;
	}
	void SetCustomValue(bool value)
	{
		if (LAppLive2DManager::GetInstance()->GetModelNum() == 0)
			return;
		LAppModel* model = LAppLive2DManager::GetInstance()->GetModel(0);
		if (model == nullptr)
			return;

		model->CustomValue = value;
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

void TopUpdate() 
{
	Live2dApp::_update();
}