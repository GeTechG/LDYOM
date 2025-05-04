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

	void initialize();
	void shutdown();
	void process();

	static void renderFrames();

	static Application& instance() {
		static Application instance;
		return instance;
	}
};