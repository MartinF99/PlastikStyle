#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylehelper.h"

#include <QPainter>
#include <QProgressBar>
#include <QStyleOptionProgressBar>
#include <QStyleOptionSlider>
#include <QPixmapCache>


void PlastikStyle::drawIndicatorProgressChunk(const QStyleOption *option,
                                              QPainter *painter,
                                              const QWidget *widget) const {
  bool vertical = false, inverted = false;
  if (const QStyleOptionProgressBar *progressBar =
          qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
    vertical = !(progressBar->state & State_Horizontal);
    inverted = progressBar->invertedAppearance;
  }
  int space = 2;

  int chunkSize =
      proxy()->pixelMetric(PM_ProgressBarChunkWidth, option, widget) - space;

  if (!vertical) {
    if (option->rect.width() <= chunkSize) {
      space = 0;
    }
    if (inverted) {
      painter->fillRect(option->rect.x() + space, option->rect.y(),
                        option->rect.width() - space, option->rect.height(),
                        option->palette.brush(QPalette::Highlight));

    } else {
      painter->fillRect(option->rect.x(), option->rect.y(),
                        option->rect.width() - space, option->rect.height(),
                        option->palette.brush(QPalette::Highlight));
    }
  } else {
    if (option->rect.width() <= chunkSize) {
      space = 0;
    }
    if (inverted) {
      painter->fillRect(option->rect.x(), option->rect.y(),
                        option->rect.width(), option->rect.height() - space,
                        option->palette.brush(QPalette::Highlight));
    } else {
      painter->fillRect(option->rect.x(), option->rect.y() + space,
                        option->rect.width(), option->rect.height() - space,
                        option->palette.brush(QPalette::Highlight));
    }
  }
}

void PlastikStyle::drawProgressBarGroove(const QStyleOption *option,
                                         QPainter *painter, QColor borderColor,
                                         QColor gradientStartColor,
                                         const QWidget *widget) const {
  QColor alphaCornerColor = calcAlphaCornerColor(option, widget);
  if (const QStyleOptionProgressBar *bar =
          qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
    QRect rect = bar->rect;
    QPen oldPen = painter->pen();

    QLine lines[4];

    // outline
    painter->setPen(borderColor);
    lines[0] = QLine(rect.left() + 2, rect.top(), rect.right() - 2, rect.top());
    lines[1] =
        QLine(rect.left() + 2, rect.bottom(), rect.right() - 2, rect.bottom());
    lines[2] =
        QLine(rect.left(), rect.top() + 2, rect.left(), rect.bottom() - 2);
    lines[3] =
        QLine(rect.right(), rect.top() + 2, rect.right(), rect.bottom() - 2);
    painter->drawLines(lines, 4);

    QPoint points[8];
    points[0] = QPoint(rect.left() + 1, rect.top() + 1);
    points[1] = QPoint(rect.right() - 1, rect.top() + 1);
    points[2] = QPoint(rect.left() + 1, rect.bottom() - 1);
    points[3] = QPoint(rect.right() - 1, rect.bottom() - 1);
    painter->drawPoints(points, 4);

    // alpha corners
    painter->setPen(alphaCornerColor);
    points[0] = QPoint(rect.left(), rect.top() + 1);
    points[1] = QPoint(rect.left() + 1, rect.top());
    points[2] = QPoint(rect.right(), rect.top() + 1);
    points[3] = QPoint(rect.right() - 1, rect.top());
    points[4] = QPoint(rect.left(), rect.bottom() - 1);
    points[5] = QPoint(rect.left() + 1, rect.bottom());
    points[6] = QPoint(rect.right(), rect.bottom() - 1);
    points[7] = QPoint(rect.right() - 1, rect.bottom());
    painter->drawPoints(points, 8);

    // inner outline, north-west
    painter->setPen(gradientStartColor.darker(105));
    lines[0] = QLine(rect.left() + 2, rect.top() + 1, rect.right() - 2,
                     rect.top() + 1);
    lines[1] = QLine(rect.left() + 1, rect.top() + 2, rect.left() + 1,
                     rect.bottom() - 2);
    painter->drawLines(lines, 2);

    // base of the groove
    painter->setPen(QPen());
    painter->fillRect(rect.adjusted(2, 2, -2, -1),
                      QBrush(bar->palette.base().color()));
    painter->setPen(bar->palette.base().color());
    painter->drawLine(rect.right() - 1, rect.top() + 2, rect.right() - 1,
                      rect.bottom() - 2);

    painter->setPen(oldPen);
  }
}

// No Widget, not needed in this function
void PlastikStyle::drawProgressBarLabel(const QStyleOption *option,
                                        QPainter *painter) const {

  if (const QStyleOptionProgressBar *bar =
          qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
    // The busy indicator doesn't draw a label
    if (bar->minimum == 0 && bar->maximum == 0)
      return;

    painter->save();

    QRect rect = bar->rect;
    QRect leftRect;

    QFont font;
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(bar->palette.text().color());

    bool vertical = !(bar->state & State_Horizontal);
    bool inverted = bar->invertedAppearance;
    bool bottomToTop = bar->bottomToTop;

    if (vertical) {
      rect = QRect(rect.left(), rect.top(), rect.height(),
                   rect.width()); // flip width and height
      QTransform m;
      if (bottomToTop) {
        m.translate(0.0, rect.width());
        m.rotate(-90);
      } else {
        m.translate(rect.height(), 0.0);
        m.rotate(90);
      }
      painter->setTransform(m, true);
    }

    int progressIndicatorPos =
        (bar->progress - qreal(bar->minimum)) /
        qMax(qreal(1.0), qreal(bar->maximum) - bar->minimum) * rect.width();

    bool flip =
        (!vertical && (((bar->direction == Qt::RightToLeft) && !inverted) ||
                       ((bar->direction == Qt::LeftToRight) && inverted))) ||
        (vertical &&
         ((!inverted && !bottomToTop) || (inverted && bottomToTop)));
    if (flip) {
      int indicatorPos = rect.width() - progressIndicatorPos;
      if (indicatorPos >= 0 && indicatorPos <= rect.width()) {
        painter->setPen(bar->palette.highlightedText().color());
        leftRect = QRect(rect.left(), rect.top(), indicatorPos, rect.height());
      } else if (indicatorPos > rect.width()) {
        painter->setPen(bar->palette.text().color());
      } else {
        painter->setPen(bar->palette.highlightedText().color());
      }
    } else {
      if (progressIndicatorPos >= 0 && progressIndicatorPos <= rect.width()) {
        leftRect =
            QRect(rect.left(), rect.top(), progressIndicatorPos, rect.height());
      } else if (progressIndicatorPos > rect.width()) {
        painter->setPen(bar->palette.highlightedText().color());
      } else {
        painter->setPen(bar->palette.text().color());
      }
    }

    QRegion rightRect = rect;
    rightRect = rightRect.subtracted(leftRect);
    painter->setClipRegion(rightRect);
    painter->drawText(
        rect, bar->text,
        QTextOption(Qt::AlignAbsolute | Qt::AlignHCenter | Qt::AlignVCenter));
    if (!leftRect.isNull()) {
      painter->setPen(flip ? bar->palette.text().color()
                           : bar->palette.highlightedText().color());
      painter->setClipRect(leftRect);
      painter->drawText(
          rect, bar->text,
          QTextOption(Qt::AlignAbsolute | Qt::AlignHCenter | Qt::AlignVCenter));
    }

    painter->restore();
  }
}

void PlastikStyle::drawProgressBarContents(const QStyleOption *option,
                                           QPainter *painter, QColor highlightedDarkInnerBorderColor, QColor alphaInnerColor ) const {
  if (const QStyleOptionProgressBar *bar =
          qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
    QRect rect = bar->rect;
    bool vertical = !(bar->state & State_Horizontal);
    bool inverted = bar->invertedAppearance;
    bool indeterminate = (bar->minimum == 0 && bar->maximum == 0);
    if (!indeterminate && bar->progress == -1)
     {return;}

    painter->save();

    // If the orientation is vertical, we use a transform to rotate
    // the progress bar 90 degrees clockwise.  This way we can use the
    // same rendering code for both orientations.
    if (vertical) {
      rect = QRect(rect.left(), rect.top(), rect.height(),
                   rect.width()); // flip width and height
      QTransform m = QTransform::fromTranslate(rect.height() - 1, 0);
      m.rotate(90.0);
      painter->setTransform(m, true);
    }

    int maxWidth = rect.width() - 4;
    int minWidth = 4;
    qint64 progress = qMax<qint64>(
        bar->progress, bar->minimum); // workaround for bug in QProgressBar
    double vc6_workaround = ((progress - qint64(bar->minimum)) /
                             qMax(double(1.0), double(qint64(bar->maximum) -
                                                      qint64(bar->minimum))) *
                             maxWidth);
    int width = indeterminate ? maxWidth : qMax(int(vc6_workaround), minWidth);
    bool reverse =
        (!vertical && (bar->direction == Qt::RightToLeft)) || vertical;
    if (inverted)
      reverse = !reverse;

    QRect progressBar;
    if (!indeterminate) {
      if (!reverse) {
        progressBar.setRect(rect.left() + 2, rect.top() + 2, width,
                            rect.height() - 4);
      } else {
        progressBar.setRect(rect.right() - 1 - width, rect.top() + 2, width,
                            rect.height() - 4);
      }
    } else {
      int slideWidth = ((rect.width() - 4) * 2) / 3;
      int step = ((animateStep * slideWidth) / ProgressBarFps) % slideWidth;
      if ((((animateStep * slideWidth) / ProgressBarFps) % (2 * slideWidth)) >=
          slideWidth)
        step = slideWidth - step;
      progressBar.setRect(rect.left() + 2 + step, rect.top() + 2,
                          slideWidth / 2, rect.height() - 4);
    }

    // outline
    painter->setPen(highlightedDarkInnerBorderColor);

    QVarLengthArray<QLine, 4> lines;
    QVarLengthArray<QPoint, 8> points;
    if (!reverse) {
      if (width == minWidth) {
        points.append(QPoint(progressBar.left() + 1, progressBar.top()));
        points.append(QPoint(progressBar.left() + 1, progressBar.bottom()));
      } else {
        if (indeterminate) {
          lines.append(QLine(progressBar.left() + 2, progressBar.top(),
                             progressBar.right() - 2, progressBar.top()));
          lines.append(QLine(progressBar.left() + 2, progressBar.bottom(),
                             progressBar.right() - 2, progressBar.bottom()));
        } else {
          lines.append(QLine(progressBar.left() + 1, progressBar.top(),
                             progressBar.right() - 2, progressBar.top()));
          lines.append(QLine(progressBar.left() + 1, progressBar.bottom(),
                             progressBar.right() - 2, progressBar.bottom()));
        }
      }

      if (indeterminate) {
        lines.append(QLine(progressBar.left(), progressBar.top() + 2,
                           progressBar.left(), progressBar.bottom() - 2));
      } else {
        lines.append(QLine(progressBar.left(), progressBar.top() + 1,
                           progressBar.left(), progressBar.bottom() - 1));
      }
      lines.append(QLine(progressBar.right(), progressBar.top() + 2,
                         progressBar.right(), progressBar.bottom() - 2));
    } else {
      if (width == minWidth) {
        points.append(QPoint(progressBar.right() - 1, progressBar.top()));
        points.append(QPoint(progressBar.right() - 1, progressBar.bottom()));
      } else {
        if (indeterminate) {
          lines.append(QLine(progressBar.right() - 2, progressBar.top(),
                             progressBar.left() + 2, progressBar.top()));
          lines.append(QLine(progressBar.right() - 2, progressBar.bottom(),
                             progressBar.left() + 2, progressBar.bottom()));
        } else {
          lines.append(QLine(progressBar.right() - 1, progressBar.top(),
                             progressBar.left() + 2, progressBar.top()));
          lines.append(QLine(progressBar.right() - 1, progressBar.bottom(),
                             progressBar.left() + 2, progressBar.bottom()));
        }
      }
      if (indeterminate) {
        lines.append(QLine(progressBar.right(), progressBar.top() + 2,
                           progressBar.right(), progressBar.bottom() - 2));
      } else {
        lines.append(QLine(progressBar.right(), progressBar.top() + 1,
                           progressBar.right(), progressBar.bottom() - 1));
      }
      lines.append(QLine(progressBar.left(), progressBar.top() + 2,
                         progressBar.left(), progressBar.bottom() - 2));
    }

    if (points.size() > 0) {
      painter->drawPoints(points.constData(), points.size());
      points.clear();
    }
    painter->drawLines(lines.constData(), lines.size());
    lines.clear();

    // alpha corners
    painter->setPen(alphaInnerColor);
    if (!reverse) {
      if (indeterminate) {
        points.append(QPoint(progressBar.left() + 1, progressBar.top()));
        points.append(QPoint(progressBar.left(), progressBar.top() + 1));
        points.append(QPoint(progressBar.left() + 1, progressBar.bottom()));
        points.append(QPoint(progressBar.left(), progressBar.bottom() - 1));
      } else {
        points.append(QPoint(progressBar.left(), progressBar.top()));
        points.append(QPoint(progressBar.left(), progressBar.bottom()));
      }
      points.append(QPoint(progressBar.right() - 1, progressBar.top()));
      points.append(QPoint(progressBar.right(), progressBar.top() + 1));
      points.append(QPoint(progressBar.right() - 1, progressBar.bottom()));
      points.append(QPoint(progressBar.right(), progressBar.bottom() - 1));
    } else {
      if (indeterminate) {
        points.append(QPoint(progressBar.right() - 1, progressBar.top()));
        points.append(QPoint(progressBar.right(), progressBar.top() + 1));
        points.append(QPoint(progressBar.right() - 1, progressBar.bottom()));
        points.append(QPoint(progressBar.right(), progressBar.bottom() - 1));
      } else {
        points.append(QPoint(progressBar.right(), progressBar.top()));
        points.append(QPoint(progressBar.right(), progressBar.bottom()));
      }
      points.append(QPoint(progressBar.left() + 1, progressBar.top()));
      points.append(QPoint(progressBar.left(), progressBar.top() + 1));
      points.append(QPoint(progressBar.left() + 1, progressBar.bottom()));
      points.append(QPoint(progressBar.left(), progressBar.bottom() - 1));
    }

    painter->drawPoints(points.constData(), points.size());
    points.clear();

    // contents
    painter->setPen(QPen());

    QString progressBarName = QStyleHelper::uniqueName(
        QLatin1String("progressBarContents"), option, rect.size());
    QPixmap cache;
    if (!QPixmapCache::find(progressBarName, &cache) && rect.height() > 7) {
      QSize size = rect.size();
      cache = QPixmap(QSize(size.width() - 6 + 30, size.height() - 6));
      cache.fill(Qt::white);
      QPainter cachePainter(&cache);
      QRect pixmapRect(0, 0, cache.width(), cache.height());

      int leftEdge = 0;
      bool flip = false;
      while (leftEdge < cache.width() + 1) {
        QColor rectColor = option->palette.highlight().color();
        QColor lineColor = option->palette.highlight().color();
        if (flip) {
          flip = false;
          rectColor = rectColor.lighter(105);
          lineColor = lineColor.lighter(105);
        } else {
          flip = true;
        }

        cachePainter.setPen(lineColor);
        const QLine cacheLines[2] = {
            QLine(pixmapRect.left() + leftEdge - 1, pixmapRect.top(),
                  pixmapRect.left() + leftEdge + 9, pixmapRect.top()),
            QLine(pixmapRect.left() + leftEdge - 1, pixmapRect.bottom(),
                  pixmapRect.left() + leftEdge + 9, pixmapRect.bottom())};
        cachePainter.drawLines(cacheLines, 2);
        cachePainter.fillRect(QRect(pixmapRect.left() + leftEdge,
                                    pixmapRect.top(), 10, pixmapRect.height()),
                              rectColor);

        leftEdge += 10;
      }

      QPixmapCache::insert(progressBarName, cache);
    }
    painter->setClipRect(progressBar.adjusted(1, 0, -1, -1));

    if (!vertical)
      progressBar.adjust(0, 1, 0, 1);
    if (!indeterminate) {
      int step =
          (AnimateProgressBar || (indeterminate && AnimateBusyProgressBar))
              ? (animateStep % 20)
              : 0;
      if (reverse)
        painter->drawPixmap(progressBar.left() - 25 + step, progressBar.top(),
                            cache);
      else
        painter->drawPixmap(progressBar.left() - 25 - step + width % 20,
                            progressBar.top(), cache);
    } else {
      painter->drawPixmap(progressBar.left(), progressBar.top(), cache);
    }

    painter->restore();
  }
}
