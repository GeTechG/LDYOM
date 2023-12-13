#pragma once
#include <functional>

#include "AbstractWindow.h"

class WindowsRenderCommand {
public:
	virtual ~WindowsRenderCommand() = default;
	virtual void execute() const = 0;
	virtual void undo() const = 0;
};

class ToggleWindowsRenderCommand final : public WindowsRenderCommand {
private:
	Windows::AbstractWindow *window;
	bool status;

public:
	ToggleWindowsRenderCommand(Windows::AbstractWindow *window, const bool status)
		: window{window}, status{status} { }

	void execute() const override {
		if (status)
			window->open();
		else
			window->close();
	}

	void undo() const override {
		if (status)
			window->close();
		else
			window->open();
	}
};

class ReplaceWindowsRenderCommand final : public WindowsRenderCommand {
private:
	Windows::AbstractWindow *window;
	Windows::AbstractWindow *replace;

public:
	ReplaceWindowsRenderCommand(Windows::AbstractWindow *window, Windows::AbstractWindow *replace)
		: window{window}, replace{replace} { }

	void execute() const override {
		window->close();
		replace->open();
	}

	void undo() const override {
		window->open();
		replace->close();
	}
};

class CustomWindowsRenderCommand final : public WindowsRenderCommand {
private:
	std::function<void()> executeFunc;
	std::function<void()> undoFunc;

public:
	CustomWindowsRenderCommand(const std::function<void()> &executeFunc, std::function<void()> undoFunc)
		: executeFunc{executeFunc}, undoFunc{std::move(undoFunc)} { }

	void execute() const override {
		executeFunc();
	}

	void undo() const override {
		undoFunc();
	}
};
