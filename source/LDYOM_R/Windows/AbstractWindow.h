#pragma once

namespace Windows
{
	class AbstractWindow
	{
	private:
		bool show_ = false;
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

		virtual ~AbstractWindow() = default;
	};
}

