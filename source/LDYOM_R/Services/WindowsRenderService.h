#pragma once
#include <stack>
#include <boost/signals2/signal.hpp>

#include "AbstractWindow.h"
#include "WindowsRenderCommand.h"

namespace Windows {
	class AbstractWindow;

	class WindowsRenderService {
	private:
		std::list<std::unique_ptr<AbstractWindow>> windows;
		std::map<std::string, std::function<void()>> renderList;
		bool mouseShown_ = false;
		bool renderWindows_ = true;
		std::stack<std::unique_ptr<WindowsRenderCommand>> commands;


		WindowsRenderService() = default;
		WindowsRenderService(const WindowsRenderService &root) = delete;
		WindowsRenderService& operator=(const WindowsRenderService &) = delete;

	public:
		static WindowsRenderService& getInstance() {
			static WindowsRenderService instance;
			return instance;
		}

		void Init();

		void render();
		static void style();

		bool& isRenderWindows();
		void setRenderWindows(bool renderWindows);
		void Reset();

		std::list<std::unique_ptr<AbstractWindow>>& getWindows();
		bool& isMouseShown();
		void setMouseShown(bool mouseShown);

		template <typename Base>
		void toggleWindow(bool status);

		template <typename Base, typename Replace>
		void replaceWindow();

		void undoLastCommand();

		template <typename Base>
		Base* getWindow() const;

		void addRender(std::string name, std::function<void()> renderFunc);
		void removeRender(std::string name);

		void closeAllWindows();
	};

	template <typename Base>
	void WindowsRenderService::toggleWindow(bool status) {
		this->commands.push(std::make_unique<ToggleWindowsRenderCommand>(getWindow<Base>(), status));
		this->commands.top()->execute();
	}

	template <typename Base, typename Replace>
	void WindowsRenderService::replaceWindow() {
		this->commands.push(std::make_unique<ReplaceWindowsRenderCommand>(getWindow<Base>(), getWindow<Replace>()));
		this->commands.top()->execute();
	}

	template <typename Base>
	Base* WindowsRenderService::getWindow() const {
		for (const auto &window : windows) {
			if (dynamic_cast<const Base*>(window.get()) != nullptr) {
				return reinterpret_cast<Base*>(window.get());
			}
		}
		throw std::exception("not found window");
	}
}
