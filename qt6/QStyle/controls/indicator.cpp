#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylecache.h"
#include "qstylehelper.h"

#include <QPainter>

// checkboxes, radiobuttons ...

void PlastikStyle::drawCheckBoxIndicator(const QStyleOption *option,
                                         QPainter *painter) const {
  if (const QStyleOptionButton *button =
          qstyleoption_cast<const QStyleOptionButton *>(option)) {
    BEGIN_STYLE_PIXMAPCACHE(QLatin1String("checkbox"))

    p->save();

    // Outline
    QBrush border = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.4));
    p->setPen(QPen(border, 0));
    const QLine lines[4] = {
        QLine(rect.left() + 1, rect.top(), rect.right() - 1, rect.top()),
        QLine(rect.left() + 1, rect.bottom(), rect.right() - 1, rect.bottom()),
        QLine(rect.left(), rect.top() + 1, rect.left(), rect.bottom() - 1),
        QLine(rect.right(), rect.top() + 1, rect.right(), rect.bottom() - 1)};
    p->drawLines(lines, 4);

    QBrush corner = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&corner, qreal(0.2));
    p->setPen(QPen(corner, 0));
    const QPoint points[4] = {rect.topLeft(), rect.topRight(),
                              rect.bottomLeft(), rect.bottomRight()};
    p->drawPoints(points, 4);

    // Fill
    QBrush baseBrush =
        PlastikStyleHelper::mapBrushToRectangle(button->palette.base(), rect);
    if (!baseBrush.gradient() && baseBrush.texture().isNull()) {
      QLinearGradient gradient(rect.center().x(), rect.top(), rect.center().x(),
                               rect.bottom());
      gradient.setColorAt(0, baseBrush.color());
      gradient.setColorAt(1, baseBrush.color().darker(105));
      baseBrush = gradient;
    }
    p->fillRect(rect.adjusted(1, 1, -1, -1), baseBrush);

    // Hover
    if ((button->state & State_Enabled) && (button->state & State_MouseOver)) {
      QBrush pen = PlastikStyleHelper::mapBrushToRectangle(
          button->palette.highlight(), rect);
      PlastikStyleHelper::setBrushAlphaF(&pen, qreal(0.8));
      p->setPen(QPen(pen, 0));
      p->drawRect(rect.adjusted(1, 1, -2, -2));
      PlastikStyleHelper::setBrushAlphaF(&pen, 0.5);
      p->setPen(QPen(pen, 0));
      p->drawRect(rect.adjusted(2, 2, -3, -3));

      PlastikStyleHelper::setBrushAlphaF(&pen, qreal(0.2));
      p->setBrush(pen);
      p->drawRect(rect.adjusted(2, 2, -3, -3));
    }
    // Indicator
    bool on = button->state & State_On;
    bool sunken = button->state & State_Sunken;
    bool unchanged = button->state & State_NoChange;
    bool enabled = button->state & State_Enabled;
    if (on || (enabled && sunken) || unchanged) {
      p->setRenderHint(QPainter::Antialiasing);
      QBrush pointBrush =
          PlastikStyleHelper::mapBrushToRectangle(button->palette.text(), rect);
      if (sunken)
        PlastikStyleHelper::setBrushAlphaF(&pointBrush, qreal(0.5));
      else if (unchanged)
        PlastikStyleHelper::setBrushAlphaF(&pointBrush, qreal(0.3));
      p->setPen(QPen(pointBrush, 3));
      const QLine lines[2] = {QLine(rect.left() + 4, rect.top() + 4,
                                    rect.right() - 3, rect.bottom() - 3),
                              QLine(rect.right() - 3, rect.top() + 4,
                                    rect.left() + 4, rect.bottom() - 3)};
      p->drawLines(lines, 2);
    }

    p->restore();
    END_STYLE_PIXMAPCACHE
  }
}

void PlastikStyle::drawRadioButtonIndicator(const QStyleOption *option,
                                            QPainter *painter) const {
  if (const QStyleOptionButton *button =
          qstyleoption_cast<const QStyleOptionButton *>(option)) {
    BEGIN_STYLE_PIXMAPCACHE(QLatin1String("radiobutton"))

    p->save();
    p->setRenderHint(QPainter::Antialiasing);

    // The the filled ellipse
    QBrush border =
        PlastikStyleHelper::mapBrushToRectangle(option->palette.shadow(), rect);
    PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.51));
    p->setPen(QPen(border, 0));

    QBrush baseBrush =
        PlastikStyleHelper::mapBrushToRectangle(button->palette.base(), rect);
    if (!baseBrush.gradient() && baseBrush.texture().isNull()) {
      QLinearGradient gradient(rect.center().x(), rect.top(), rect.center().x(),
                               rect.bottom());
      gradient.setColorAt(0, baseBrush.color());
      gradient.setColorAt(1, baseBrush.color().darker(105));
      baseBrush = gradient;
    }
    p->setBrush(baseBrush);
    p->drawEllipse(QRectF(rect).adjusted(1, 1, -1, -1));

    // Hover
    if ((button->state & State_Enabled) && (button->state & State_MouseOver)) {
      QBrush pen = PlastikStyleHelper::mapBrushToRectangle(
          button->palette.highlight(), rect);
      PlastikStyleHelper::setBrushAlphaF(&pen, qreal(0.8));
      p->setPen(QPen(pen, 0));
      PlastikStyleHelper::setBrushAlphaF(&pen, qreal(0.2));
      p->setBrush(pen);
      p->drawEllipse(QRectF(rect).adjusted(2, 2, -2, -2));
    }

    // Indicator
    bool on = button->state & State_On;
    bool sunken = button->state & State_Sunken;
    bool enabled = button->state & State_Enabled;
    if (on || (enabled && sunken)) {
      p->setPen(Qt::NoPen);
      QBrush pointBrush =
          PlastikStyleHelper::mapBrushToRectangle(button->palette.text(), rect);
      if (sunken)
        PlastikStyleHelper::setBrushAlphaF(&pointBrush, 0.5);
      p->setBrush(pointBrush);
      p->drawEllipse(QRectF(rect).adjusted(3, 3, -3, -3));
    }
    QT_END_NAMESPACE
    p->restore();
    END_STYLE_PIXMAPCACHE
  }
}

void PlastikStyle::drawBranchIndicator(const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const {
  int mid_h = option->rect.x() + option->rect.width() / 2;
  int mid_v = option->rect.y() + option->rect.height() / 2;
  int bef_h = mid_h;
  int bef_v = mid_v;
  int aft_h = mid_h;
  int aft_v = mid_v;
  QColor borderColor = option->palette.window().color().darker(178);
  QColor alphaCornerColor = this->calcAlphaCornerColor(option, widget);
  QColor alphaTextColor = this->calcAlphaTextColor(option);
  QBrush brush(option->palette.dark().color(), Qt::Dense4Pattern);

  if (option->state & State_Item) {
    if (option->direction == Qt::RightToLeft) {
      painter->fillRect(option->rect.left(), mid_v, bef_h - option->rect.left(),
                        1, brush);
    } else {
      painter->fillRect(aft_h, mid_v, option->rect.right() - aft_h + 1, 1,
                        brush);
    }
  }

  if (option->state & State_Sibling) {
    painter->fillRect(mid_h, aft_v, 1, option->rect.bottom() - aft_v + 1,
                      brush);
  }
  if (option->state &
      (State_Open | State_Children | State_Item | State_Sibling)) {
    painter->fillRect(mid_h, option->rect.y(), 1, bef_v - option->rect.y(),
                      brush);
  }
  if (option->state & State_Children) {
    painter->save();
    QPoint center = option->rect.center();
    // border
    QRect fullRect(center.x() - 4, center.y() - 4, 9, 9);
    painter->setPen(borderColor);

    const QLine lines[4] = {QLine(fullRect.left() + 1, fullRect.top(),
                                  fullRect.right() - 1, fullRect.top()),
                            QLine(fullRect.left() + 1, fullRect.bottom(),
                                  fullRect.right() - 1, fullRect.bottom()),
                            QLine(fullRect.left(), fullRect.top() + 1,
                                  fullRect.left(), fullRect.bottom() - 1),
                            QLine(fullRect.right(), fullRect.top() + 1,
                                  fullRect.right(), fullRect.bottom() - 1)};
    painter->drawLines(lines, 4);

    // "antialiased" corners
    painter->setPen(alphaCornerColor);
    const QPoint points[4] = {fullRect.topLeft(), fullRect.topRight(),
                              fullRect.bottomLeft(), fullRect.bottomRight()};
    painter->drawPoints(points, 4);

    // fill
    QRect adjustedRect = fullRect;
    QRect gradientRect(adjustedRect.left() + 1, adjustedRect.top() + 1,
                       adjustedRect.right() - adjustedRect.left() - 1,
                       adjustedRect.bottom() - adjustedRect.top() - 1);
    if (option->palette.base().style() == Qt::SolidPattern) {
      QColor baseGradientStartColor =
          option->palette.base().color().darker(101);
      QColor baseGradientStopColor = option->palette.base().color().darker(106);
      PlastikStyleHelper::drawGradient(
          painter, gradientRect, baseGradientStartColor, baseGradientStopColor);
    } else {
      painter->fillRect(gradientRect, option->palette.base());
    }
    // draw "+" or "-"
    painter->setPen(alphaTextColor);
    painter->drawLine(center.x() - 2, center.y(), center.x() + 2, center.y());
    if (!(option->state & State_Open)) {
      painter->drawLine(center.x(), center.y() - 2, center.x(), center.y() + 2);
    }
    painter->restore();
  }
}

void PlastikStyle::drawHeaderArrowIndicator(PrimitiveElement element,
                                            const QStyleOption *option,
                                            QPainter *painter,
                                            const QWidget *widget) const {
  bool usedAntialiasing = painter->renderHints() & QPainter::Antialiasing;
  if (!usedAntialiasing) {
    painter->setRenderHint(QPainter::Antialiasing);
  }
  QCommonStyle::drawPrimitive(element, option, painter, widget);
  if (!usedAntialiasing) {
    painter->setRenderHint(QPainter::Antialiasing, false);
  }
}
