﻿#pragma once
#include <boost/signals2/signal.hpp>

#include "AbstractWindow.h"

namespace Windows {
	class AbstractWindow;

	class WindowsRenderService {
	private:
		std::list<std::unique_ptr<AbstractWindow>> windows_;
		std::map<std::string, std::function<void()>> renderList_;
		bool mouseShown_ = false;
		bool renderWindows_ = true;

		WindowsRenderService() = default;
		WindowsRenderService(const WindowsRenderService &root) = delete;
		WindowsRenderService& operator=(const WindowsRenderService &) = delete;

	public:
		static WindowsRenderService& getInstance() {
			static WindowsRenderService instance;
			return instance;
		}

		void Init() const;

		void render() const;
		static void style();

		bool& isRenderWindows();
		void setRenderWindows(bool renderWindows);
		void Reset();

		std::list<std::unique_ptr<AbstractWindow>>& getWindows();
		bool& isMouseShown();
		void setMouseShown(bool mouseShown);

		template <typename Base>
		void toggleWindow(bool status) const;

		template <typename Base>
		Base* getWindow() const;

		void addRender(std::string name, std::function<void()> renderFunc);
		void removeRender(std::string name);

		void closeAllWindows() const;
	};

	template <typename Base>
	void WindowsRenderService::toggleWindow(bool status) const {
		AbstractWindow *window = getWindow<Base>();
		if (status)
			window->open();
		else
			window->close();
	}

	template <typename Base>
	Base* WindowsRenderService::getWindow() const {
		for (const auto &window : windows_) {
			if (dynamic_cast<const Base*>(window.get()) != nullptr) {
				return reinterpret_cast<Base*>(window.get());
			}
		}
		throw std::exception("not found window");
	}
}
