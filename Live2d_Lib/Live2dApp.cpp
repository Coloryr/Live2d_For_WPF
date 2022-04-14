#include <LAppDelegate.hpp>

public ref class Live2dApp
{
public:
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
};