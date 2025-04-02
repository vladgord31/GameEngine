#include <iostream>
#include <memory>
#include <Core/Application.hpp>

class MyApp : public Engine::Application
{
	virtual void on_update()
	{
		//std::cout << "Update frame: " << ++frame << std::endl;
	}

	int frame = 0;
};

int main()
{
	auto myApp = std::make_unique<MyApp>();

	int returnCode = myApp->start(1024, 768, "GAME ENGINE");

	return 0;
}