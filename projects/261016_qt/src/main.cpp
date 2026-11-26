#include <QApplication>
#include "welcome_window.hpp"
#include "keyboard_window.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
	
	py::WelcomeWindow welcome_window;
    py::KeyBoardWindow keyboard_window;
    	
	QObject::connect(&welcome_window, &py::WelcomeWindow::goToKeyboardClicked, [&]() {
        welcome_window.hide();   
        keyboard_window.show();  
    });
	
	welcome_window.show();
	
    return app.exec();
}