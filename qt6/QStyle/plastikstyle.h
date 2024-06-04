#ifndef PLASTIKSTYLE_H_INCLUDED
#define PLASTIKSTYLE_H_INCLUDED
#include <QProxyStyle>
#include <QCommonStyle>
#include <QElapsedTimer>

//class QStyleOptionSlider;
class QProgressBar;




class PlastikStyle : public QCommonStyle
{
    Q_OBJECT

public:
    PlastikStyle();
    ~PlastikStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const override;

    int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = 0) const override;
    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     const QPoint &pos, const QWidget *widget = nullptr) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = nullptr) const override;

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = nullptr) const override;

    void polish(QWidget *widget) override;
    void polish(QApplication *app) override;
    void polish(QPalette &pal) override;
    void unpolish(QWidget *widget) override;
    void unpolish(QApplication *app) override;

    QPalette standardPalette() const override;

    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *opt = 0,
                       const QWidget *widget = nullptr) const override;
    int layoutSpacing(QSizePolicy::ControlType control1,
                      QSizePolicy::ControlType control2,
                      Qt::Orientation orientation,
                      const QStyleOption *option = 0,
                      const QWidget *widget = nullptr) const override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *event) override;
    void startProgressAnimation(QProgressBar *bar);
    void stopProgressAnimation(QProgressBar *bar);
    //void drawSliderGroove(const QStyleOptionSlider *slider, QPainter *painter, QWidget *widget=nullptr);
    qreal dpiScaled(qreal toScale, qreal dpi) const;
    qreal dpiScaled(qreal toScale, const QStyleOption* option) const;
    qreal dpiScaled(qreal toScale, const QPaintDevice* device) const;
    qreal dpi(const QStyleOption* option) const;
    qreal defaultDPI() const; // return 96 or primary Screen dpi
private:
    const qreal baseDPI=96; // doesn't work on mac'
    const int menuItemHMargin = 3;
    int animateStep;
    QList<QProgressBar *> bars;
    int progressBarAnimateTimer;
    QElapsedTimer timer;

    QList<const QWidget *> seenAlt; // from windowsstyle
    bool alt_down=false;
    bool hasSeenAlt(const QWidget *widget) const;

    QColor inactiveCaptionText;
        QColor activeCaptionColor;
        QColor activeGradientCaptionColor;
        QColor inactiveCaptionColor;
        QColor inactiveGradientCaptionColor;

    bool two_buttons_bottom = true;

    void draw_addline_two_buttons(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void draw_addline_one_button(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void draw_subline_two_buttons(const QStyleOption *scrollBar, QPainter *painter, const QWidget *widget=nullptr) const;
    void draw_subline_one_button(const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const;
};

#endif // PLASTIKSTYLE_H_INCLUDED
