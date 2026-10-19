// this file includes all functions for drawing frames
#include "plastikstyle.h"

#include <QFrame>
#include <QPainter>
#include <QStyleOptionFrame>
#include <qdrawutil.h>

void PlastikStyle::drawFocusRectFrame(const QStyleOption *option,
                                      QPainter *painter,
                                      const QWidget *widget) const {
  if (const QStyleOptionFocusRect *focusRect =
          qstyleoption_cast<const QStyleOptionFocusRect *>(option)) {
    if (!(focusRect->state & State_KeyboardFocusChange) &&
        !(proxy()->styleHint(SH_UnderlineShortcut, option)))
      return;
    QRect rect = option->rect;
    painter->save();
    painter->setBackgroundMode(Qt::TransparentMode);

    QColor backgroundColor = focusRect->backgroundColor;
    if (!backgroundColor.isValid()) {
      backgroundColor =
          painter->background().color(); // use background color if color from
                                         // styleoption is invalid
    }

    QColor patternColor((backgroundColor.red() ^ 0xff) & 0xff,
                        (backgroundColor.green() ^ 0xff) & 0xff,
                        (backgroundColor.blue() ^ 0xff) & 0xff);

    painter->setBrush(QBrush(patternColor, Qt::Dense4Pattern));
    painter->setBrushOrigin(rect.topLeft());
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect.left(), rect.top(), rect.width(), 1);    // Top
    painter->drawRect(rect.left(), rect.bottom(), rect.width(), 1); // Bottom
    painter->drawRect(rect.left(), rect.top(), 1, rect.height());   // Left
    painter->drawRect(rect.right(), rect.top(), 1, rect.height());  // Right
    painter->restore();
  }
}
