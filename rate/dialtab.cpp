#include "dialtab.h"
#include "Dial.h"
#include <qlayout.h>

DialTab::DialTab(QWidget *parent): QWidget(parent) {
    QGridLayout *layout = new QGridLayout(this);
    DialBox *dialBox = new DialBox(this, 0);
    layout->addWidget(dialBox, 0, 0);
}

