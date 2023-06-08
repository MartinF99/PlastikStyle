#ifndef PLASTIKSTYLE_H
#define PLASTIKSTYLE_H
#include <QProxyStyle>
#include <QElapsedTimer>

class QProgressBar;

class PlastikStyle : public QProxyStyle
{
    Q_OBJECT

public:
    PlastikStyle();
    ~PlastikStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = 0) const override;
    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const override;

    int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const override;
    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     const QPoint &pos, const QWidget *widget = 0) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const override;

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = 0) const override;

    void polish(QWidget *widget) override;
    void polish(QApplication *app) override;
    void polish(QPalette &pal) override;
    void unpolish(QWidget *widget) override;
    void unpolish(QApplication *app) override;

    QPalette standardPalette() const override;

    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *opt = 0,
                       const QWidget *widget = 0) const override;
    int layoutSpacing(QSizePolicy::ControlType control1,
                      QSizePolicy::ControlType control2,
                      Qt::Orientation orientation,
                      const QStyleOption *option = 0,
                      const QWidget *widget = 0) const override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *event) override;
    void startProgressAnimation(QProgressBar *bar);
    void stopProgressAnimation(QProgressBar *bar);

private:
    int animateStep;
    QList<QProgressBar *> bars;
    int progressBarAnimateTimer;
    QElapsedTimer timer;
};

#endif // PLASTIKSTYLE_H
