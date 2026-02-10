#pragma once

#include "window.h"
#include <ImNodeFlow.h>
#include <memory>
#include <string>
#include <vector>

// Example node types
class NumberNode : public ImFlow::BaseNode {
  public:
	NumberNode();
	void draw() override;

  private:
	float m_value = 0.0f;
};

class AddNode : public ImFlow::BaseNode {
  public:
	AddNode();
	void draw() override;
};

class MultiplyNode : public ImFlow::BaseNode {
  public:
	MultiplyNode();
	void draw() override;
};

class OutputNode : public ImFlow::BaseNode {
  public:
	OutputNode();
	void draw() override;
};

class TextNode : public ImFlow::BaseNode {
  public:
	TextNode();
	void draw() override;

  private:
	char m_buffer[256] = "Hello";
};

class NodeEditorWindow : public Window {
  private:
	std::unique_ptr<ImFlow::ImNodeFlow> m_nodeFlow;

	void renderContextMenu();

	static void renderContent(NodeEditorWindow* window);

  public:
	NodeEditorWindow();
	~NodeEditorWindow() override = default;

	void open() override;
	void close() override;

	ImFlow::ImNodeFlow* getNodeFlow() { return m_nodeFlow.get(); }
	void refreshTheme();
};
