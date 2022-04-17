#pragma once
#include <boost/signals2/signal.hpp>

#include "../Windows/IWindow.h"
#include "../VMTHooker/vmt.h"

namespace Windows {
	class IWindow;

	class WindowsRenderService {
	private:
		std::list<std::unique_ptr<IWindow>> windows_;
		bool mouseShown_ = false;
		std::unique_ptr<VMTHookManager> vmtHooks_;

		void shutdown() const;
		void mouseProcess() const;
		void process() const;

		WindowsRenderService() = default;
		WindowsRenderService(const WindowsRenderService& root) = delete;
		WindowsRenderService& operator=(const WindowsRenderService&) = delete;
	public:
		static WindowsRenderService& getInstance() {
			static WindowsRenderService instance;
			return instance;
		}

		void Init();
		void style();

		void render() const;

		std::list<std::unique_ptr<IWindow>>& getWindows();
		bool& isMouseShown();
		void setMouseShown(bool mouseShown);

		template<typename Base>
		void toggleWindow(bool status) const;

		template<typename Base>
		Base* getWindow() const;

		void closeAllWindows() const;
	};

	template <typename Base>
	void WindowsRenderService::toggleWindow(bool status) const {
		IWindow *window = getWindow<Base>();
		if (status)
			window->open();
		else
			window->close();
	}

	template <typename Base>
	Base* WindowsRenderService::getWindow() const {
		for (const auto& window : windows_) {
			if (dynamic_cast<const Base*>(window.get()) != nullptr)
			{
				return reinterpret_cast<Base*>(window.get());
			}
		}
		throw std::exception("not found window");
	}


}

