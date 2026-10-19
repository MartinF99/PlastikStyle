#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include <QDockWidget>
#include <QPainter>
#include <QStyleOptionDockWidget>

void PlastikStyle::drawDockWidgetResizeHandleIndicator(
    const QStyleOption *option, QPainter *painter,
    const QWidget *widget) const {
  if ((option->state & State_Enabled) && (option->state & State_MouseOver))
    painter->fillRect(option->rect, QColor(255, 255, 255, 128));
  if (option->state & State_Horizontal) {
    int width = option->rect.width() / 3;
    QRect rect(option->rect.center().x() - width / 2,
               option->rect.top() + (option->rect.height() / 2) - 1, width, 3);
    PlastikStyleHelper::drawHandle(painter, option, rect, Qt::Vertical, widget);
  } else {
    int height = option->rect.height() / 3;
    QRect rect(option->rect.left() + (option->rect.width() / 2 - 1),
               option->rect.center().y() - height / 2, 3, height);
    PlastikStyleHelper::drawHandle(painter, option, rect, Qt::Horizontal,
                                   widget);
  }
}
