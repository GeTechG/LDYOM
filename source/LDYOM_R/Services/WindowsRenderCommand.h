#pragma once
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
