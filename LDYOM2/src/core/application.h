#pragma once

#include "lua/lua_task_manager.h"
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

	LuaTaskManager& luaTaskManager() { return m_luaTaskManager; }

	static Application& instance() {
		static Application instance;
		return instance;
	}

  private:
	LuaTaskManager m_luaTaskManager;
	int m_welcomeMessageDelay = -1;
};