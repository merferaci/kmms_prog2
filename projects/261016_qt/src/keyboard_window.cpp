#include "keyboard_window.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

using py::KeyBoardWindow;

KeyBoardWindow::KeyBoardWindow(QWidget* parent) : QWidget(parent) {
	const int keyboard_width = 1160;
	resize(keyboard_width, 710);
	setWindowTitle("Грустная Клавиатура");

	// QPixmap pixmap("img/grustnii-smail.png");
	QPixmap pixmap("img/veselii-smail.png");
	QLabel* image = new QLabel(this);
	image->setFixedSize(200, 200);
	image->setPixmap(pixmap);
	image->setScaledContents(true);

	QHBoxLayout* smail_layout = new QHBoxLayout();
	smail_layout->addWidget(image);

	display = new QLineEdit();
	display->setMinimumHeight(80);
	display->setFont(QFont("Roboto", 40));
	display->setReadOnly(true);
	display->setText("Помоги мне заработать лучше...");
	display->setFocusPolicy(Qt::NoFocus); 

	keyboard = new KeyBoard(keyboard_width);
	connect(keyboard, &KeyBoard::keyClicked, this, &KeyBoardWindow::handleKeyPress);

	QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->addLayout(smail_layout);
	main_layout->addWidget(display);
	main_layout->addWidget(keyboard);
}

void KeyBoardWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Backspace) {
        display->backspace();
        return;
    }
    if (event->key() == Qt::Key_Return) {
        display->clear();
        return;
    }


    QString text = event->text();
    if (!text.isEmpty()) {
        display->insert(text); 
        keyboard->animate_by_text(text); 
    }
}

void KeyBoardWindow::handleKeyPress(int code) {

	if (code == Qt::Key_Backspace) {
		display->backspace();
	}
	else if (code == Qt::Key_Space) {
		display->insert(" ");
	}
	else if (code == Qt::Key_Tab) {
		display->insert("\t");
	}
	else if (code == Qt::Key_Return) {
		display->clear(); 
	}
	else if (keyboard->is_key_allowed(code)) {
		QString txt = keyboard->get_key_text(code);
		if (!txt.isEmpty()) {
			display->insert(txt);
		}
		keyboard->animate_button(code);
	}
}