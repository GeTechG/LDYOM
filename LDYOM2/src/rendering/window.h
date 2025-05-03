#pragma once

#include "imgui.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Window {
  protected:
	std::string m_title;
	bool m_open = false;
	bool m_needLockPlayerControl = true;
	bool m_isAppering = false;
	bool m_isFocused = false;
	ImVec2 m_size = ImVec2(0, 0);
	ImVec2 m_sizeMin = ImVec2(0, 70);
	ImVec2 m_pivot = ImVec2(0, 0);
	ImVec2 m_position = ImVec2(0, 0);
	ImGuiCond m_positionCondition = ImGuiCond_FirstUseEver;
	ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;
	std::function<void(Window*)> m_renderCallback = nullptr;

	virtual void configurateWindow();

  public:
	explicit Window(const std::string& title)
		: m_title(title) {}
	virtual ~Window() = default;

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	Window(Window&&) noexcept = default;
	Window& operator=(Window&&) noexcept = default;

	virtual void open();
	virtual void close();

	virtual void render();

	const ImVec2& NewFunction();

	bool isOpen() const { return m_open; }

	bool isNeedLockPlayerControl() const { return m_needLockPlayerControl; }
	void setNeedLockPlayerControl(bool need) { m_needLockPlayerControl = need; }

	const std::string& getTitle() const { return m_title; }
	void setTitle(const std::string& title) { m_title = title; }

	void setSize(float width, float height) { m_size = ImVec2(width, height); }
	ImVec2 getSize() const { return m_size; }

	void setSizeMin(float width, float height) { m_sizeMin = ImVec2(width, height); }
	ImVec2 getSizeMin() const { return m_sizeMin; }

	void setPivot(float x, float y) { m_pivot = ImVec2(x, y); }
	ImVec2 getPivot() const { return m_pivot; }

	void setPosition(float x, float y) { m_position = ImVec2(x, y); }
	ImVec2 getPosition() const { return m_position; }

	void setPositionCondition(ImGuiCond condition) { m_positionCondition = condition; }
	ImGuiCond getPositionCondition() const { return m_positionCondition; }

	void setFlags(ImGuiWindowFlags flags) { m_flags = flags; }
	ImGuiWindowFlags getFlags() const { return m_flags; }

	bool isFocused() const { return m_isFocused; }

	template <typename T> void setRenderCallback(std::function<void(T*)> callback) {
		static_assert(std::is_base_of<Window, T>::value, "T must be derived from Window");
		m_renderCallback = [callback](Window* window) {
			T* derived = dynamic_cast<T*>(window);
			if (derived) {
				callback(derived);
			}
		};
	}

	void setRenderCallback(std::function<void(Window*)> callback) { m_renderCallback = callback; }
};