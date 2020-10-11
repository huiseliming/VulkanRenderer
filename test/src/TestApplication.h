#pragma once
class Application
{
public:
	Application() = default;
	virtual ~Application() = default;

	void Run();


	void StartUp();
	void MainLoop();
	void Cleanup();


private:

};


