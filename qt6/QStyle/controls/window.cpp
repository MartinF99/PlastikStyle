#include "plastikstyle.h"
#include "plastikstylehelper.h"

#include <QPainter>

void PlastikStyle::drawWindowFrame(const QStyleOption *option,
                                   QPainter *painter,
                                   const QWidget *widget) const {
  painter->save();
  bool active = (option->state & State_Active);
  int titleBarStop = option->rect.top() +
                     proxy()->pixelMetric(PM_TitleBarHeight, option, widget);

  QPalette palette = option->palette;
  if (!active) {
    palette.setCurrentColorGroup(QPalette::Disabled);
  }
  // Frame and rounded corners
  painter->setPen(PlastikStyleHelper::mergeColors(palette.highlight().color(),
                                                  Qt::black, 50));

  QLine lines[3];
  QPoint points[4];

  // bottom border line
  lines[0] = QLine(option->rect.left() + 1, option->rect.bottom(),
                   option->rect.right() - 1, option->rect.bottom());

  // bottom left and right side border lines
  lines[1] = QLine(option->rect.left(), titleBarStop, option->rect.left(),
                   option->rect.bottom() - 1);
  lines[2] = QLine(option->rect.right(), titleBarStop, option->rect.right(),
                   option->rect.bottom() - 1);
  painter->drawLines(lines, 3);
  points[0] = QPoint(option->rect.left() + 1, option->rect.bottom() - 1);
  points[1] = QPoint(option->rect.right() - 1, option->rect.bottom() - 1);
  painter->drawPoints(points, 2);

  // alpha corners
  painter->setPen(PlastikStyleHelper::mergeColors(
      palette.highlight().color(), palette.window().color(), 55));
  points[0] = QPoint(option->rect.left() + 2, option->rect.bottom() - 1);
  points[1] = QPoint(option->rect.left() + 1, option->rect.bottom() - 2);
  points[2] = QPoint(option->rect.right() - 2, option->rect.bottom() - 1);
  points[3] = QPoint(option->rect.right() - 1, option->rect.bottom() - 2);
  painter->drawPoints(points, 4);

  // upper and lower left inner
  painter->setPen(
      active ? PlastikStyleHelper::mergeColors(palette.highlight().color(),
                                               palette.window().color())
             : palette.window().color().darker(120));
  painter->drawLine(option->rect.left() + 1, titleBarStop,
                    option->rect.left() + 1, option->rect.bottom() - 2);

  painter->setPen(
      active ? PlastikStyleHelper::mergeColors(palette.highlight().color(),
                                               palette.window().color(), 57)
             : palette.window().color().darker(130));
  lines[0] = QLine(option->rect.right() - 1, titleBarStop,
                   option->rect.right() - 1, option->rect.bottom() - 2);
  lines[1] = QLine(option->rect.left() + 1, option->rect.bottom() - 1,
                   option->rect.right() - 1, option->rect.bottom() - 1);
  painter->drawLines(lines, 2);

  painter->restore();
}
