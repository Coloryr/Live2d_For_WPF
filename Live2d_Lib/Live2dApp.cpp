#include <LAppDelegate.hpp>

public ref class Live2dApp
{
public:
	void Start() 
	{
		if (LAppDelegate::GetInstance()->Initialize() == GL_FALSE)

		LAppDelegate::GetInstance()->Run();
	}
};