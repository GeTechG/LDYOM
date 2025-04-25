#pragma once

#include <string>
#include <vector>

class Application {
  public:
	Application() = default;
	~Application() = default;

	Application(const Application&) = delete;
	Application(Application&&) = delete;

	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void Initialize();
	void Shutdown();

	static void RenderFrames();

	static Application& Instance() {
		static Application instance;
		return instance;
	}
};