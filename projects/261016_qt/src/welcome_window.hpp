#pragma once

#include <QWidget>
#include <QPushButton>

namespace py {
    class WelcomeWindow : public QWidget {
        Q_OBJECT 

        public:
            WelcomeWindow(QWidget *parent = nullptr);

        signals:
            void goToKeyboardClicked();
    };
}