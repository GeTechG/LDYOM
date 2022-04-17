#pragma once

namespace Windows
{
	class IWindow
	{
	private:
		bool show_;
	public:

		virtual void draw() = 0;

		bool& isShow() {
			return show_;
		}

		virtual void open()
		{
			show_ = true;
		}

		virtual void close()
		{
			show_ = false;
		}

		virtual ~IWindow() = default;
	};
}

