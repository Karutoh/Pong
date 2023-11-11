#include <LWE/LWE.h>
#include <LWE/Log.h>
#include <LWE/GarbageCollector.h>
#include <LWE/GameLoop.h>
#include <LWE/IO/RenderWindow.h>
#include <LWE/IO/Console.h>

#include "Levels/Game.h"
#include "LWE/IO/HID/GenericInputHandler.h"

void LogCallback(const lwe::Log& log)
{
	lwe::Console::Write_8(log.ToStr());
}

lwe::SInt_32 Main(lwe::Str_8* appName, lwe::Str_8* appVerId, lwe::Version* appVer)
{
	*appName = "Pong";
	*appVerId = "Alpha";
	*appVer = {0, 0, 1};

	lwe::Console::Attach();

	lwe::Log::SetCallback(LogCallback);

	#if defined(LWE_OS_WINDOWS)
		lwe::GpuInstance::AddExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	#elif defined(LWE_OS_LINUX)
		lwe::GpuInstance::AddExtension(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
	#endif

	lwe::GpuInstance::AddExtension(VK_KHR_SURFACE_EXTENSION_NAME);

	lwe::GpuInstance::Initialize(true);

	lwe::GpuDevice primary = lwe::GpuDevice::GetBest();

	lwe::GpuQueueFamily* family = primary.GetQueueFamily(lwe::QueueType::GRAPHICS);

	lwe::GpuInterface inf(&primary);
	inf.AddExtensions(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	lwe::GpuQueue queue(family, &inf, 1.0f);

	inf.Initialize();

	lwe::RenderWindow win(&inf, &queue);
	win.Create_8(*appName, {0, 0}, {1024, 768});

	lwe::GameLoop gl(&win, 6, 0);
	gl.GetInput()->AddHandler(new lwe::GenericInputHandler());

	lwe::Frame* mainFrame = new lwe::Frame("Main");

	gl.AddFrame(mainFrame);

	mainFrame->AddLevel(new Game());

	gl.Initialize();
	gl.Start();
	gl.UnInitialize();

	win.Close();

	inf.Release();

	lwe::GpuInstance::Release();

	return 0;
}