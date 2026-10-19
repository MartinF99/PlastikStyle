#pragma once

#include <QFrame>
#include <QPainter>
#include <QRect>
#include <QStyleOption>

// HElper class, allows splitting implementation for these and calling them
// Where I want, technically not necessary, could just add it wherever, but
// feels right
class PlastikStyleHelper {
public:


// Methinks this should be in PlastikStyle itself..., or at least in something that can be instanced
//maybe this thing should be its own namespace instead of class, like QStyleHelper..., for now let's do it this way until we run into trouble
static const int blueFrameWidth = 2; // with of line edit focus frame
// some static variables from WindowsStyle


// various charmaps for pixmap generation, could probably done differentlyt (also should this be here? Probably could just be Put into the various files later)

constexpr static const char *const sliderVerticalHandle[] = {
    "15 11 6 1",
    "   c None",
    "+  c #979797",
    "@  c #C9C9C9",
    "$  c #C1C1C1",
    "b  c None",
    "d  c None",
    " $++++++++$    ",
    "$+bbbbbbbb+$   ",
    "+b $$      +$  ",
    "+b $@       +$ ",
    "+b           +$",
    "+b           d+",
    "+b          d+$",
    "+b $$      d+$ ",
    "+b $@     d+$  ",
    "$+dddddddd+$   ",
    " $++++++++$    "};

constexpr static const char *const sliderVerticalHandleLeft[] = {
    "15 11 6 1",
    "   c None",
    "+  c #979797",
    "@  c #C9C9C9",
    "$  c #C1C1C1",
    "b  c None",
    "d  c None",
    "    $++++++++$ ",
    "   $+bbbbbbbb+$",
    "  $+b     $$ d+",
    " $+b      $@ d+",
    "$+b          d+",
    "+b           d+",
    "$+           d+",
    " $+       $$ d+",
    "  $+      $@ d+",
    "   $+dddddddd+$",
    "    $++++++++$ "};


constexpr static const char *const sliderHorizontalHandle[] = {
    "11 15 6 1",
    "   c None",
    "+  c #979797",
    "@  c #C9C9C9",
    "$  c #C1C1C1",
    "b  c None",
    "d  c None",
    " $+++++++$ ",
    "$+bbbbbbb+$",
    "+b       d+",
    "+b$$   $$d+",
    "+b$@   $@d+",
    "+b       d+",
    "+b       d+",
    "+b       d+",
    "+b       d+",
    "+b       d+",
    "$+      d+$",
    " $+    d+$ ",
    "  $+  d+$  ",
    "   $+d+$   ",
    "    $+$    "};

constexpr static const char *const sliderHorizontalHandleUp[] = {
    "11 15 6 1",
    "   c None",
    "+  c #979797",
    "@  c #C9C9C9",
    "$  c #C1C1C1",
    "b  c None",
    "d  c None",
    "    $+$    ",
    "   $+b+$   ",
    "  $+b  +$  ",
    " $+b    +$ ",
    "$+b      +$",
    "+b       d+",
    "+b       d+",
    "+b       d+",
    "+b       d+",
    "+b       d+",
    "+b$$   $$d+",
    "+b$@   $@d+",
    "+b       d+",
    "$+ddddddd+$",
    " $+++++++$ "};

constexpr static const char *const scrollBarButtonArrowLeft[] = {
    "4 7 2 1",
    "   c None",
    "*  c #BFBFBF",
    "   *",
    "  **",
    " ***",
    "****",
    " ***",
    "  **",
    "   *"};

constexpr static const char *const scrollBarButtonArrowRight[] = {
    "4 7 2 1",
    "   c None",
    "*  c #BFBFBF",
    "*   ",
    "**  ",
    "*** ",
    "****",
    "*** ",
    "**  ",
    "*   "};

constexpr static const char *const scrollBarButtonArrowUp[] = {
    "7 4 2 1",
    "   c None",
    "*  c #BFBFBF",
    "   *   ",
    "  ***  ",
    " ***** ",
    "*******"};

constexpr static const char *const scrollBarButtonArrowDown[] = {
    "7 4 2 1",
    "   c None",
    "*  c #BFBFBF",
    "*******",
    " ***** ",
    "  ***  ",
    "   *   "};

constexpr static const char *const scrollBarButtonLeft[] = {
    "16 16 6 1",
    "   c None",
    ".  c #BFBFBF",
    "+  c #979797",
    "#  c #FAFAFA",
    "<  c #FAFAFA",
    "*  c #FAFAFA",
    " .+++++++++++++.",
    ".+#############+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    ".+<<<<<<<<<<<<<+",
    " .+++++++++++++."};


constexpr static const char *const scrollBarButtonRight[] = {
    "16 16 6 1",
    "   c None",
    ".  c #BFBFBF",
    "+  c #979797",
    "#  c #FAFAFA",
    "<  c #FAFAFA",
    "*  c #FAFAFA",
    ".+++++++++++++. ",
    "+#############+.",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+<<<<<<<<<<<<<+.",
    ".+++++++++++++. "};

constexpr static const char *const scrollBarButtonUp[] = {
    "16 16 6 1",
    "   c None",
    ".  c #BFBFBF",
    "+  c #979797",
    "#  c #FAFAFA",
    "<  c #FAFAFA",
    "*  c #FAFAFA",
    " .++++++++++++. ",
    ".+############+.",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+<<<<<<<<<<<<<<+",
    ".++++++++++++++."};

constexpr static const char *const scrollBarButtonDown[] = {
    "16 16 6 1",
    "   c None",
    ".  c #BFBFBF",
    "+  c #979797",
    "#  c #FAFAFA",
    "<  c #FAFAFA",
    "*  c #FAFAFA",
    "++++++++++++++++",
    "+##############+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    "+#            <+",
    ".+<<<<<<<<<<<<+.",
    " .++++++++++++. "};

constexpr static const char *const sliderPatternVertical[] = {
    "10 18 3 1",
    "   c None",
    ".  c #BFBFBF",
    "+  c #979797",
    "..  ..  ..",
    ".+  .+  .+",
    "          ",
    "          ",
    "..  ..  ..",
    ".+  .+  .+",
    "          ",
    "          ",
    "..  ..  ..",
    ".+  .+  .+",
    "          ",
    "          ",
    "..  ..  ..",
    ".+  .+  .+",
    "          ",
    "          ",
    "..  ..  ..",
    ".+  .+  .+"};

constexpr static const char *const sliderPatternHorizontal[] = {
    "18 10 3 1",
    "   c None",
    ".  c #BFBFBF",
    "+  c #979797",
    "..  ..  ..  ..  ..",
    ".+  .+  .+  .+  .+",
    "                  ",
    "                  ",
    "..  ..  ..  ..  ..",
    ".+  .+  .+  .+  .+",
    "                  ",
    "                  ",
    "..  ..  ..  ..  ..",
    ".+  .+  .+  .+  .+"};

constexpr static const char *const toolBarHandle[] = {
    "6 6 4 1",
    "       c None",
    ".      c #C5C5C5",
    "+      c #EEEEEE",
    "@      c #FAFAFA",
    "..    ",
    ".+@   ",
    " @@   ",
    "   .. ",
    "   .+@",
    "    @@"};

constexpr static const char *const simpleToolBarHandle[] = {
    "3 3 4 1",
    "       c None",
    ".      c #C5C5C5",
    "+      c #EEEEEE",
    "@      c #FAFAFA",
    ".. ",
    ".+@",
    " @@"};

constexpr static const char *const titleBarContextHelp[] = {
    "27 27 5 1",
    "  c None",
    ". c #0A0C12",
    "+ c #1B202D",
    "@ c #293144",
    "# c #3C435D",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "           +@##@+          ",
    "         .@@@.+@@..        ",
    "         .##+  +@@+.       ",
    "         .##@  @#@+.       ",
    "         ....  +@+..       ",
    "            .@+@@..        ",
    "            +#@@+          ",
    "            .##.           ",
    "            .++.           ",
    "            .++.           ",
    "            +##+           ",
    "            .@@.           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           ",
    "                           "};

// map to Rect
static QLinearGradient mapGradientToRect(const QLinearGradient &toMap,
                                        const QRectF &rect);
static QBrush mapBrushToRectangle(const QBrush &toMap, const QRectF &rect);
// Brush Colors
static QColor mergeColors(const QColor &colorA, const QColor &colorB,
                        int factor = 50);
static QBrush darkenBrush(QBrush brush, int dark);
static QBrush lightenBrush(QBrush brush, int light);
static void setBrushAlphaF(QBrush *brush, qreal alpha);

// drawing certain Elements

static void drawGradient(QPainter *painter, const QRect &rect,
                        const QColor &gradientStart,
                        const QColor &gradientStop);

static void drawFrame(QPainter *painter, const QRect &rect,
                        const QStyleOption *option,
                        QFrame::Shadow shadow = QFrame::Plain);
static void drawFrame(QPainter *painter,
                        const QStyleOption *option,
                        const QWidget *widget);
static void drawMdiButton(QPainter *painter,
                            const QStyleOptionTitleBar *option,
                            const QRect &tmp, bool hover, bool sunken);
static void drawShadePanel(QPainter *painter, const QStyleOption *option,
                            bool base, const QWidget *widget);
static QString elliditide(const QString &text, const QFontMetrics &fontMetrics,
                            const QRect &rect, int *textWidth = 0);
static void drawHandle(QPainter *painter, const QStyleOption *option, const QRect &rect, Qt::Orientation orientation, const QWidget *widget);

static void drawPartialFrame(QPainter *painter, const QStyleOptionComplex *option, const QRect &rect, const QWidget *widget, const QStyle *style);
};
