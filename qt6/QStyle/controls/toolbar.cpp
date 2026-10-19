#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylehelper.h"

#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QStyleOptionToolBar>
#include <QStyleOptionToolButton>
#include <QToolBar>
#include <QToolButton>

void PlastikStyle::drawToolBarHandleIndicator(const QStyleOption *option,
                                              QPainter *painter,
                                              const QWidget *widget) const {
  QColor alphaCornerColor = calcAlphaCornerColor(option, widget);
  QPixmap cache;
  QRect rect = option->rect;
  QString pixmapName = QStyleHelper::uniqueName(QLatin1String("toolbarhandle"),
                                                option, rect.size());
  if (!QPixmapCache::find(pixmapName, &cache)) {
    cache = QPixmap(rect.size());
    cache.fill(Qt::transparent);
    QPainter cachePainter(&cache);
    QRect cacheRect(QPoint(0, 0), rect.size());
    if (widget)
      cachePainter.fillRect(cacheRect,
                            option->palette.brush(widget->backgroundRole()));
    else
      cachePainter.fillRect(cacheRect, option->palette.window());

    QImage handle(PlastikStyleHelper::toolBarHandle);
    alphaCornerColor.setAlpha(170);
    handle.setColor(1, alphaCornerColor.rgba());
    handle.setColor(2, PlastikStyleHelper::mergeColors(
                           alphaCornerColor, option->palette.light().color())
                           .rgba());
    handle.setColor(3, option->palette.light().color().rgba());

    if (option->state & State_Horizontal) {
      int nchunks = cacheRect.height() / handle.height();
      int indent = (cacheRect.height() - (nchunks * handle.height())) / 2;
      for (int i = 0; i < nchunks; ++i)
        cachePainter.drawImage(
            QPoint(cacheRect.left() + 3,
                   cacheRect.top() + indent + i * handle.height()),
            handle);
    } else {
      int nchunks = cacheRect.width() / handle.width();
      int indent = (cacheRect.width() - (nchunks * handle.width())) / 2;
      for (int i = 0; i < nchunks; ++i)
        cachePainter.drawImage(
            QPoint(cacheRect.left() + indent + i * handle.width(),
                   cacheRect.top() + 3),
            handle);
    }
    cachePainter.end();
    QPixmapCache::insert(pixmapName, cache);
  }
  painter->drawPixmap(rect.topLeft(), cache);
}

void PlastikStyle::drawToolBarSeperatorIndicator(const QStyleOption *option,
                                                 QPainter *painter,
                                                 const QWidget *widget) const {
  QColor alphaCornerColor = calcAlphaCornerColor(option, widget);
  QPen oldPen = painter->pen();
  painter->setPen(alphaCornerColor);
  if (option->state & State_Horizontal) {
    painter->drawLine(option->rect.left(), option->rect.top() + 1,
                      option->rect.left(), option->rect.bottom() - 2);
    painter->setPen(option->palette.base().color());
    painter->drawLine(option->rect.right(), option->rect.top() + 1,
                      option->rect.right(), option->rect.bottom() - 2);
  } else {
    painter->drawLine(option->rect.left() + 1, option->rect.top(),
                      option->rect.right() - 2, option->rect.top());
    painter->setPen(option->palette.base().color());
    painter->drawLine(option->rect.left() + 1, option->rect.bottom(),
                      option->rect.right() - 2, option->rect.bottom());
  }
  painter->setPen(oldPen);
}

void PlastikStyle::drawToolButtonPanel(const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const {
  // Draws a tool button (f.ex., in QToolBar and QTabBar), not seperated,
  // because tiny
  if ((option->state & State_Enabled || option->state & State_On) ||
      !(option->state & State_AutoRaise)) {
    PlastikStyleHelper::drawShadePanel(painter, option, true, widget);
  }
}
