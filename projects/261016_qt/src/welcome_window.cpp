#include "welcome_window.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

using py::WelcomeWindow;

WelcomeWindow::WelcomeWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Грустная Клавиатура");
    resize(700, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPixmap pixmap("img/grustnii-smail.png"); 
    QLabel* image = new QLabel(this);
    image->setFixedSize(200, 200);
    image->setPixmap(pixmap);
    image->setScaledContents(true);
    layout->addWidget(image, 0, Qt::AlignCenter);

    QPushButton *btn = new QPushButton("Перейти к клавиатуре", this);
    layout->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, &WelcomeWindow::goToKeyboardClicked);
}