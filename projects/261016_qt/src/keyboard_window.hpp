#pragma once

#include <cstddef>

#include <QKeyEvent>
#include <QLineEdit>
#include <QWidget>

#include "keyboard.hpp"

namespace py {
	class KeyBoardWindow : public QWidget {
		Q_OBJECT
		
		private:
			QLineEdit* display;
			KeyBoard* keyboard;

		public:
			KeyBoardWindow(QWidget* parent = nullptr);

		protected:
			void keyPressEvent(QKeyEvent* event) override;
			
		private slots: 
			void handleKeyPress(int code);
	};
}