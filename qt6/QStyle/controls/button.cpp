#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylecache.h"
#include "qstylehelper.h"
#include <QBrush>
#include <QPainter>
#include <QStyleOptionButton>
#include <qdrawutil.h>

// This is all button frames, so put it out of frame.cpp
void PlastikStyle::drawButtonBevel(const QStyleOption *option,
                                   QPainter *painter, bool isPanel) const {
  QBrush fill;
  painter->setBrushOrigin(option->rect.topLeft());

  if (!(option->state & State_Sunken) && (option->state & State_On))
    fill = QBrush(option->palette.light().color(), Qt::Dense4Pattern);
  else
    fill = option->palette.brush(QPalette::Button);

  if (option->state & (State_Raised | State_On | State_Sunken)) {
    qDrawWinButton(painter, option->rect, option->palette,
                   option->state & (State_Sunken | State_On),
                   isPanel ? &fill : nullptr);
  } else {
    if (isPanel)
      painter->fillRect(option->rect, fill);
    else
      painter->drawRect(option->rect);
  }
}

void PlastikStyle::drawToolButtonFrame(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const {
  QPen oldPen = painter->pen();
  // If widget is Dockwidget and the parent is a window, Draw the Frame as a
  // Window button then return
  if (widget && widget->inherits("QDockWidgetTitleButton")) {
    if (const QWidget *parent = widget->parentWidget()) {
      if (parent->isWindow()) {
        (qDrawWinButton(painter, option->rect.adjusted(1, 1, 0, 0),
                        option->palette,
                        option->state & (State_Sunken | State_On),
                        &option->palette.button()));
      }
    }
    return;
  }
  QBrush fill;
  bool stippled;
  bool panel = (element == PE_PanelButtonTool);
  if ((!(option->state & State_Sunken)) &&
      (!(option->state & State_Enabled) ||
       !(option->state & State_MouseOver && option->state & State_AutoRaise)) &&
      (option->state & State_On)) {
    fill = QBrush(option->palette.light().color(), Qt::Dense4Pattern);
    stippled = true;
  } else {
    fill = option->palette.brush(QPalette::Button);
    stippled = false;
  }

  if (option->state & (State_Raised | State_Sunken | State_On)) {
    if (option->state & State_AutoRaise) {
      if (option->state & (State_Enabled | State_Sunken | State_On)) {
        if (panel)
          qDrawShadePanel(painter, option->rect, option->palette,
                          option->state & (State_Sunken | State_On), 1, &fill);
        else
          qDrawShadeRect(painter, option->rect, option->palette,
                         option->state & (State_Sunken | State_On), 1);
      }
      if (stippled) {
        painter->setPen(option->palette.button().color());
        painter->drawRect(option->rect.adjusted(1, 1, -2, -2));
      }
    } else {
      qDrawWinButton(painter, option->rect, option->palette,
                     option->state & (State_Sunken | State_On),
                     panel ? &fill : nullptr);
    }
  } else {
    painter->fillRect(option->rect, fill);
  }
  painter->setPen(oldPen);
}

void PlastikStyle::drawDefaultButtonFrame(const QStyleOption *option,
                                          QPainter *painter) const {
  // don't draw Frame if the button is not enabled
  if (!(option->state & QStyle::State_Enabled))
    return;
  painter->setPen(QPen(QColor(0, 0, 0, 127), 0));
  const QLine lines[4] = {
      QLine(option->rect.left() + 2, option->rect.top(),
            option->rect.right() - 2, option->rect.top()),
      QLine(option->rect.left() + 2, option->rect.bottom(),
            option->rect.right() - 2, option->rect.bottom()),
      QLine(option->rect.left(), option->rect.top() + 2, option->rect.left(),
            option->rect.bottom() - 2),
      QLine(option->rect.right(), option->rect.top() + 2, option->rect.right(),
            option->rect.bottom() - 2)};
  painter->drawLines(lines, 4);

  QPoint points[8];
  points[0] = QPoint(option->rect.left() + 1, option->rect.top() + 1);
  points[1] = QPoint(option->rect.right() - 1, option->rect.top() + 1);
  points[2] = QPoint(option->rect.left() + 1, option->rect.bottom() - 1);
  points[3] = QPoint(option->rect.right() - 1, option->rect.bottom() - 1);
  painter->drawPoints(points, 4);

  painter->setPen(QPen(QColor(0, 0, 0, 63), 0));
  points[0] = QPoint(option->rect.left() + 1, option->rect.top());
  points[1] = QPoint(option->rect.right() - 1, option->rect.top());
  points[2] = QPoint(option->rect.left(), option->rect.top() + 1);
  points[3] = QPoint(option->rect.right(), option->rect.top() + 1);
  points[4] = QPoint(option->rect.left() + 1, option->rect.bottom());
  points[5] = QPoint(option->rect.right() - 1, option->rect.bottom());
  points[6] = QPoint(option->rect.left(), option->rect.bottom() - 1);
  points[7] = QPoint(option->rect.right(), option->rect.bottom() - 1);
  painter->drawPoints(points, 8);
}

void PlastikStyle::drawCommandButtonPanel(const QStyleOption *option,
                                          QPainter *painter) const {
  if (const QStyleOptionButton *button =
          qstyleoption_cast<const QStyleOptionButton *>(option)) {
    bool sunken = (button->state & State_Sunken) || (button->state & State_On);
    if ((button->features & QStyleOptionButton::Flat) && !sunken)
      return;

    bool defaultButton =
        (button->features & (QStyleOptionButton::DefaultButton |
                             QStyleOptionButton::AutoDefaultButton));

    BEGIN_STYLE_PIXMAPCACHE(
        QString::fromLatin1("pushbutton-%1").arg(defaultButton))

    QPen oldPen = p->pen();
    bool hover =
        (button->state & State_Enabled) && (button->state & State_MouseOver);

    // Give the painter a different brush origin for sunken buttons
    if (sunken) {
      // ### No such function
      // p->setPenOrigin(rect.left() + 1, rect.top() + 1);
      p->setBrushOrigin(rect.left() + 1, rect.top() + 1);
    }

    // Draw border
    PlastikStyleHelper::drawFrame(p, rect, option);

    // Fill the panel
    QRectF fillRect = rect.adjusted(2, 2, -2, -2);

    // Button colors
    QBrush alphaCornerBrush = PlastikStyleHelper::mapBrushToRectangle(
        PlastikStyleHelper::darkenBrush(option->palette.button(), 165), rect);
    PlastikStyleHelper::setBrushAlphaF(&alphaCornerBrush, 0.5);
    QBrush buttonGradientBrush;
    QBrush leftLineGradientBrush;
    QBrush rightLineGradientBrush;
    QBrush sunkenButtonGradientBrush;
    QBrush sunkenLeftLineGradientBrush;
    QBrush sunkenRightLineGradientBrush;
    QBrush buttonBrush =
        PlastikStyleHelper::mapBrushToRectangle(option->palette.button(), rect);
    if (buttonBrush.gradient() || !buttonBrush.texture().isNull()) {
      buttonGradientBrush = buttonBrush;
      sunkenButtonGradientBrush =
          PlastikStyleHelper::darkenBrush(buttonBrush, 108);
      leftLineGradientBrush =
          PlastikStyleHelper::lightenBrush(buttonBrush, 105);
      rightLineGradientBrush =
          PlastikStyleHelper::darkenBrush(buttonBrush, 105);
      sunkenLeftLineGradientBrush =
          PlastikStyleHelper::darkenBrush(buttonBrush, 110);
      sunkenRightLineGradientBrush =
          PlastikStyleHelper::darkenBrush(buttonBrush, 106);
    } else {
      // Generate gradients
      QLinearGradient buttonGradient(rect.topLeft(), rect.bottomLeft());
      if (hover) {
        buttonGradient.setColorAt(0.0,
                                  PlastikStyleHelper::mergeColors(
                                      option->palette.highlight().color(),
                                      buttonBrush.color().lighter(104), 6));
        buttonGradient.setColorAt(1.0, PlastikStyleHelper::mergeColors(
                                           option->palette.highlight().color(),
                                           buttonBrush.color().darker(110), 6));
      } else {
        buttonGradient.setColorAt(0.0, buttonBrush.color().lighter(104));
        buttonGradient.setColorAt(1.0, buttonBrush.color().darker(110));
      }
      buttonGradientBrush = QBrush(buttonGradient);

      QLinearGradient buttonGradient2(rect.topLeft(), rect.bottomLeft());
      buttonGradient2.setColorAt(0.0, buttonBrush.color().darker(113));
      buttonGradient2.setColorAt(1.0, buttonBrush.color().darker(103));
      sunkenButtonGradientBrush = QBrush(buttonGradient2);

      QLinearGradient buttonGradient3(rect.topLeft(), rect.bottomLeft());
      buttonGradient3.setColorAt(0.0, buttonBrush.color().lighter(105));
      buttonGradient3.setColorAt(1.0, buttonBrush.color());
      leftLineGradientBrush = QBrush(buttonGradient3);

      QLinearGradient buttonGradient4(rect.topLeft(), rect.bottomLeft());
      buttonGradient4.setColorAt(0.0, buttonBrush.color());
      buttonGradient4.setColorAt(1.0, buttonBrush.color().darker(110));
      rightLineGradientBrush = QBrush(buttonGradient4);

      QLinearGradient buttonGradient5(rect.topLeft(), rect.bottomLeft());
      buttonGradient5.setColorAt(0.0, buttonBrush.color().darker(113));
      buttonGradient5.setColorAt(1.0, buttonBrush.color().darker(107));
      sunkenLeftLineGradientBrush = QBrush(buttonGradient5);

      QLinearGradient buttonGradient6(rect.topLeft(), rect.bottomLeft());
      buttonGradient6.setColorAt(0.0, buttonBrush.color().darker(108));
      buttonGradient6.setColorAt(1.0, buttonBrush.color().darker(103));
      sunkenRightLineGradientBrush = QBrush(buttonGradient6);
    }

    // Main fill
    p->fillRect(
        fillRect,
        PlastikStyleHelper::mapBrushToRectangle(
            sunken ? sunkenButtonGradientBrush : buttonGradientBrush, rect));

    // Top line
    p->setPen(QPen(
        PlastikStyleHelper::lightenBrush(
            PlastikStyleHelper::mapBrushToRectangle(
                sunken ? sunkenButtonGradientBrush : buttonGradientBrush, rect),
            105),
        0));
    p->drawLine(QPointF(rect.left() + 2, rect.top() + 1),
                QPointF(rect.right() - 2, rect.top() + 1));

    // Bottom line
    p->setPen(QPen(
        PlastikStyleHelper::darkenBrush(
            PlastikStyleHelper::mapBrushToRectangle(
                sunken ? sunkenButtonGradientBrush : buttonGradientBrush, rect),
            105),
        0));
    p->drawLine(QPointF(rect.left() + 2, rect.bottom() - 1),
                QPointF(rect.right() - 2, rect.bottom() - 1));

    // Left line
    p->setPen(QPen(
        PlastikStyleHelper::mapBrushToRectangle(
            sunken ? sunkenLeftLineGradientBrush : leftLineGradientBrush, rect),
        1));
    p->drawLine(QPointF(rect.left() + 1, rect.top() + 2),
                QPointF(rect.left() + 1, rect.bottom() - 2));

    // Right line
    p->setPen(
        QPen(PlastikStyleHelper::mapBrushToRectangle(
                 sunken ? sunkenRightLineGradientBrush : rightLineGradientBrush,
                 rect),
             1));
    p->drawLine(QPointF(rect.right() - 1, rect.top() + 2),
                QPointF(rect.right() - 1, rect.bottom() - 2));

    // Hovering
    if (hover && !sunken) {
      QBrush hover = PlastikStyleHelper::mapBrushToRectangle(
          option->palette.highlight(), rect);
      QBrush hoverOuter = hover;
      PlastikStyleHelper::setBrushAlphaF(&hoverOuter, qreal(0.7));

      QLine lines[2];

      p->setPen(QPen(hoverOuter, 0));
      lines[0] = QLine(rect.left() + 1, rect.top() + 1, rect.right() - 1,
                       rect.top() + 1);
      lines[1] = QLine(rect.left() + 1, rect.bottom() - 1, rect.right() - 1,
                       rect.bottom() - 1);
      p->drawLines(lines, 2);

      QBrush hoverInner = hover;
      PlastikStyleHelper::setBrushAlphaF(&hoverInner, qreal(0.45));
      p->setPen(QPen(hoverInner, 0));
      lines[0] = QLine(rect.left() + 1, rect.top() + 2, rect.right() - 1,
                       rect.top() + 2);
      lines[1] = QLine(rect.left() + 1, rect.bottom() - 2, rect.right() - 1,
                       rect.bottom() - 2);
      p->drawLines(lines, 2);

      QBrush hoverSide = hover;
      PlastikStyleHelper::setBrushAlphaF(&hoverSide, qreal(0.075));
      p->setPen(QPen(hoverSide, 0));
      lines[0] = QLine(rect.left() + 1, rect.top() + 2, rect.left() + 1,
                       rect.bottom() - 2);
      lines[1] = QLine(rect.right() - 1, rect.top() + 2, rect.right() - 1,
                       rect.bottom() - 2);
      p->drawLines(lines, 2);
    }

    p->setPen(oldPen);

    END_STYLE_PIXMAPCACHE
  }
}
