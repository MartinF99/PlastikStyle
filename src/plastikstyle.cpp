#include "plastikstyle.h"
#include "ui_plastikstyle.h"

PlastikStyle::PlastikStyle(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::PlastikStyle)
{
    m_ui->setupUi(this);
}

PlastikStyle::~PlastikStyle() = default;
