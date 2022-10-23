#pragma once
#include "ListWindow.h"

namespace Windows {
	class SaveConfirmPopup : public AbstractWindow {
	private:
		boost::signals2::signal<void()> onSave{};
		boost::signals2::signal<void()> onCancel{};
	public:
		void draw() override;
		boost::signals2::signal<void()>& getOnSave();
		boost::signals2::signal<void()>& getOnCancel();
	};
}
