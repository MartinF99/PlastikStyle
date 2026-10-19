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
    QColor calcAlphaCornerColor(const QStyleOption *option, const QWidget *widget = nullptr) const;
    QColor calcAlphaTextColor(const QStyleOption *option) const;

    void drawToolButtonFrame(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
    void drawButtonBevel(const QStyleOption *option, QPainter *painter, bool isPanel) const;
    void drawDefaultButtonFrame(const QStyleOption *option, QPainter *painter) const;
    void drawTabWidgetFrame(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget* widget=nullptr) const;
    void drawTabBarBaseFrame(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget* widget=nullptr) const;
    void drawLineEditFrame(const QStyleOption *option, QPainter *painter, const QWidget* widget=nullptr) const;
    void drawMenuBarPanel(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawIndicatorProgressChunk(const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
    void drawFrame(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawToolBarHandleIndicator(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawToolBarSeperatorIndicator(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawCommandButtonPanel(const QStyleOption *option, QPainter *painter) const;
    void drawCheckBoxIndicator(const QStyleOption *option, QPainter *painter) const;
    void drawRadioButtonIndicator(const QStyleOption *option, QPainter *painter) const;
    void drawDockWidgetResizeHandleIndicator(const QStyleOption *option, QPainter *painter,  const QWidget *widget=nullptr) const;
    void drawItemViewItemCheck(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawWindowFrame(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawFocusRectFrame(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawBranchIndicator(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawHeaderArrowIndicator(PrimitiveElement element,  const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawToolButtonPanel(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;

    // ControlElements
    void drawRubberBand(const QStyleOption *option, QPainter *painter, const QWidget *widget=nullptr) const;
    void drawTabBarTabShape(ControlElement element, const QStyleOption* option, QPainter* painter, QColor lightInnerHighlightBorder, QColor darkInnerHighlightBorder, QColor lightShadowGradientStart, QColor lightShadowGradientStop, const QWidget* widget = nullptr) const;
    // ProgressBar
    void drawProgressBarGroove(const QStyleOption *option, QPainter *painter, QColor borderColor, QColor gradientStartColor, const QWidget *widget=nullptr) const;
    void drawProgressBarLabel(const QStyleOption *option, QPainter *painter) const;
    void drawProgressBarContents(const QStyleOption *option, QPainter *painter, QColor highlightedDarkInnerBorderColor, QColor alphaInnerColor) const;
    void drawHeaderSection(const QStyleOption *option, QPainter *painter, QColor gradientStartColor, QColor gradientStopColor, QColor borderColor, QColor alphaCornerColor) const;
    void drawMenuItem(const QStyleOption *option, QPainter *painter, QColor alphaCornerColor, const QWidget *widget=nullptr) const;
    void drawMenuBarItem(ControlElement element, const QStyleOption *option, QPainter *painter, QColor alphaCornerColor, QColor borderColor, QColor gradientStartColor, QColor gradientStopColor, const QWidget *widget=nullptr) const;
    void drawMenuBarEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

private:
    const int itemFrameWidth = 2;       // menu item frame width
    const int seperatorHeight = 2;       // separator item height
    const int itemHMargin = 3;     // menu item hor text margin
    const int itemVMargin = 2;     // menu item ver text margin
    const int arrowHMargin = 6;    // arrow horizontal margin
    const int tabSpacing = 12;     // space between text and tab
    const int rightBorder = 15;    // right border
    const int checkMarkWidth = 12; // checkmarks width on windows
    //functions for drawing Elements
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
    // probably should be here, sure it's not necessary for anything but implementation, but it makes this stuff a lot easier later on...', maybe we can even make this not const and instead have this via settings, but that is future thingy
    const bool AnimateBusyProgressBar = true;
    const bool AnimateProgressBar = true;
    const int ProgressBarFps = 30;
};

#endif // PLASTIKSTYLE_H_INCLUDED
