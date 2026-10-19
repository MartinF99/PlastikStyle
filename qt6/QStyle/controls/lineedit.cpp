#include <QPainter>
#include <QSpinBox>
#include <QStyleOptionSpinBox>

#include "plastikstyle.h"
#include "plastikstylehelper.h"

void PlastikStyle::drawLineEditFrame(const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const {
  if (const QStyleOptionFrame *lineEdit =
          qstyleoption_cast<const QStyleOptionFrame *>(option)) {
    // Panel of a line edit inside combo box or spin box is drawn in
    // CC_ComboBox and CC_SpinBox
    if (widget) {
#ifndef QT_NO_SPINBOX
      // Spinbox doesn't need a separate palette for the lineedit
      if (qobject_cast<const QAbstractSpinBox *>(widget->parentWidget()))
        return;
#endif
    }

    painter->save();

    // Fill the line edit insides
    QRect filledRect = lineEdit->rect.adjusted(1, 1, -1, -1);
    QBrush baseBrush = PlastikStyleHelper::mapBrushToRectangle(
        lineEdit->palette.base(), filledRect);
    painter->setBrushOrigin(filledRect.topLeft());
    painter->fillRect(filledRect.adjusted(1, 1, -1, -1), baseBrush);

    painter->setPen(QPen(baseBrush, 0));
    const QLine lines[4] = {QLine(filledRect.left(), filledRect.top() + 1,
                                  filledRect.left(), filledRect.bottom() - 1),
                            QLine(filledRect.right(), filledRect.top() + 1,
                                  filledRect.right(), filledRect.bottom() - 1),
                            QLine(filledRect.left() + 1, filledRect.top(),
                                  filledRect.right() - 1, filledRect.top()),
                            QLine(filledRect.left() + 1, filledRect.bottom(),
                                  filledRect.right() - 1, filledRect.bottom())};
    painter->drawLines(lines, 4);

    if (lineEdit->lineWidth != 0)
      PlastikStyleHelper::drawFrame(painter, option->rect, option,
                                    QFrame::Sunken);

    painter->restore();
  }
}
