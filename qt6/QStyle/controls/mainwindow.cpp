#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylehelper.h"


#include <QMainWindow>
#include <QMenuBar>
#include <QPainter>
#include <QPixmapCache>

void PlastikStyle::drawMenuBarPanel(PrimitiveElement element,
                                    const QStyleOption *option,
                                    QPainter *painter,
                                    const QWidget *widget) const {
  if ((widget && qobject_cast<const QMainWindow *>(widget->parentWidget()))) {
    // Draws the light line above and the dark line below menu bars and
    // tool bars.
    QPen oldPen = painter->pen();

    QColor alphaCornerColor = this->calcAlphaCornerColor(option, widget);
    if (element == PE_PanelMenuBar || (option->state & State_Horizontal)) {
      painter->setPen(alphaCornerColor);
      painter->drawLine(option->rect.left(), option->rect.bottom(),
                        option->rect.right(), option->rect.bottom());
      painter->setPen(option->palette.window().color().lighter(104));
      painter->drawLine(option->rect.left(), option->rect.top(),
                        option->rect.right(), option->rect.top());
    } else {
      painter->setPen(option->palette.window().color().lighter(104));
      painter->drawLine(option->rect.left(), option->rect.top(),
                        option->rect.left(), option->rect.bottom());
      painter->setPen(alphaCornerColor);
      painter->drawLine(option->rect.right(), option->rect.top(),
                        option->rect.right(), option->rect.bottom());
    }
    painter->setPen(oldPen);
  }
}

void PlastikStyle::drawMenuBarItem(ControlElement element, const QStyleOption *option,
                                   QPainter *painter, QColor alphaCornerColor,
                                   QColor borderColor,
                                   QColor gradientStartColor,
                                   QColor gradientStopColor,
                                   const QWidget *widget) const {
  // Draws a menu bar item; File, Edit, Help etc..
  if ((option->state & State_Selected) || (option->state & State_MouseOver)) {
    QPixmap cache;
    QString pixmapName = QStyleHelper::uniqueName(QLatin1String("menubaritem"),
                                                  option, option->rect.size());
    if (!QPixmapCache::find(pixmapName, &cache)) {
      cache = QPixmap(option->rect.size());
      cache.fill(Qt::white);
      QRect pixmapRect(0, 0, option->rect.width(), option->rect.height());
      QPainter cachePainter(&cache);

      QRect rect = pixmapRect;

      // gradient fill
      if ((option->state & QStyle::State_Sunken) ||
          (option->state & QStyle::State_On)) {
        PlastikStyleHelper::drawGradient(
            &cachePainter, rect.adjusted(1, 1, -1, -1),
            option->palette.button().color().darker(114),
            option->palette.button().color().darker(106));
      } else if ((option->state & QStyle::State_HasFocus)) {
        PlastikStyleHelper::drawGradient(
            &cachePainter, rect.adjusted(1, 1, -1, -1),
            option->palette.button().color().darker(104),
            option->palette.button().color().darker(98));
      } else {
        PlastikStyleHelper::drawGradient(
            &cachePainter, rect.adjusted(1, 1, -1, -1),
            option->palette.window().color().lighter(105),
            option->palette.window().color().darker(102));
      }

      // outer border and corners
      cachePainter.setPen(borderColor);
      cachePainter.drawRect(rect.adjusted(0, 0, -1, -1));
      cachePainter.setPen(alphaCornerColor);

      const QPoint points[4] = {rect.topLeft(), rect.topRight(),
                                rect.bottomLeft(), rect.bottomRight()};
      cachePainter.drawPoints(points, 4);

      // inner border
      if ((option->state & QStyle::State_Sunken) ||
          (option->state & QStyle::State_On))
        cachePainter.setPen(option->palette.button().color().darker(118));
      else
        cachePainter.setPen(gradientStartColor);

      QLine lines[2];
      lines[0] = QLine(rect.left() + 1, rect.top() + 1, rect.right() - 1,
                       rect.top() + 1);
      lines[1] = QLine(rect.left() + 1, rect.top() + 2, rect.left() + 1,
                       rect.bottom() - 2);
      cachePainter.drawLines(lines, 2);

      if ((option->state & QStyle::State_Sunken) ||
          (option->state & QStyle::State_On))
        cachePainter.setPen(option->palette.button().color().darker(114));
      else
        cachePainter.setPen(gradientStopColor.darker(102));
      lines[0] = QLine(rect.left() + 1, rect.bottom() - 1, rect.right() - 1,
                       rect.bottom() - 1);
      lines[1] = QLine(rect.right() - 1, rect.top() + 1, rect.right() - 1,
                       rect.bottom() - 2);
      cachePainter.drawLines(lines, 2);
      cachePainter.end();
      QPixmapCache::insert(pixmapName, cache);
    }
    painter->drawPixmap(option->rect.topLeft(), cache);
  } else {
    painter->fillRect(option->rect, option->palette.window());
  }

  if (const QStyleOptionMenuItem *mbi =
          qstyleoption_cast<const QStyleOptionMenuItem *>(
              option)) { // from WindowsStyle
    QStyleOptionMenuItem newMI = *mbi;
    QCommonStyle::drawControl(element, &newMI, painter, widget);
  }
}

void PlastikStyle::drawMenuBarEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
  if (widget && qobject_cast<const QMainWindow *>(widget->parentWidget())) {
    painter->fillRect(option->rect, option->palette.window());
    QPen oldPen = painter->pen();
    painter->setPen(QPen(option->palette.dark().color()));
    painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
    painter->setPen(oldPen);
  }
}
