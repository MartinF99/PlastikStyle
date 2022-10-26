#ifndef PLASTIKSTYLE_H
#define PLASTIKSTYLE_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class PlastikStyle;
}

class PlastikStyle : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlastikStyle(QWidget *parent = nullptr);
    ~PlastikStyle() override;

private:
    QScopedPointer<Ui::PlastikStyle> m_ui;
};

#endif // PLASTIKSTYLE_H
