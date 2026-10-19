#include "plastikstylehelper.h"
#include "qstylehelper.h"

#include <QLineEdit>
#include <QPixmapCache>

QColor PlastikStyleHelper::mergeColors(const QColor &colorA,
                                       const QColor &colorB, int factor) {
  // making sure we actually get something useful
  if (factor >= 100) {
    factor = 100;
  } else if (factor <= 0) {
    factor = 0;
  }
  const int maxFactor = 100;

  QColor mergedColor = colorA;
  mergedColor.setRed((mergedColor.red() * factor) / maxFactor +
                     (colorB.red() * (maxFactor - factor)) / maxFactor);
  mergedColor.setGreen((mergedColor.green() * factor) / maxFactor +
                       (colorB.green() * (maxFactor - factor)) / maxFactor);
  mergedColor.setBlue((mergedColor.blue() * factor) / maxFactor +
                      (colorB.blue() * (maxFactor - factor)) / maxFactor);
  return mergedColor;
}

QLinearGradient
PlastikStyleHelper::mapGradientToRect(const QLinearGradient &toMap,
                                      const QRectF &rect) {
  QLinearGradient mappedGradient(rect.center().x(), rect.top(),
                                 rect.center().x(), rect.bottom());
  mappedGradient.setStops(toMap.stops());
  return mappedGradient;
}

QBrush PlastikStyleHelper::mapBrushToRectangle(const QBrush &toMap,
                                               const QRectF &rect) {
  if (!toMap.gradient())
    return toMap;

  if (toMap.gradient()->type() != QGradient::LinearGradient) {
    // for now just return the Brush as it was before my use-case is just linear
    // gradients anyway but hey better save than sorry
    return toMap;
  }
  QBrush tmp(mapGradientToRect(
      *static_cast<const QLinearGradient *>(toMap.gradient()), rect));
  return tmp;
}

QBrush PlastikStyleHelper::darkenBrush(QBrush brush, int dark) {
  if (const QGradient *gradient = brush.gradient()) {
    // Use the gradient. Call QColor::darker() on all color stops.
    QGradientStops stops = gradient->stops();
    QMutableListIterator<QGradientStop> it(stops);
    while (it.hasNext()) {
      it.next();
      it.setValue(QPair<qreal, QColor>(it.value().first,
                                       it.value().second.darker(dark)));
    }

    switch (gradient->type()) {
    case QGradient::RadialGradient: {
      QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
      grad.setStops(stops);
      brush = QBrush(grad);
      break;
    }
    case QGradient::ConicalGradient: {
      QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
      grad.setStops(stops);
      brush = QBrush(grad);
      break;
    }
    default:
      qWarning("PlastikStyle::qBrushDark() - unknown gradient type"
               " - falling back to QLinearGradient");
    case QGradient::LinearGradient: {
      QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
      grad.setStops(stops);
      brush = QBrush(grad);
      break;
    }
    }
  } else if (!brush.texture().isNull()) {
    // Modify the texture - ridiculously expensive.
    QPixmap texture = brush.texture();
    QPixmap pixmap;
    QString name = QLatin1String("qbrushtexture-dark") % HexString<int>(dark) %
                   HexString<qint64>(texture.cacheKey());

    if (!QPixmapCache::find(name, &pixmap)) {
      QImage image = texture.toImage();
      QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
      int pixels = image.width() * image.height();
      QColor tmpColor;
      while (pixels--) {
        tmpColor.setRgb(*rgb);
        *rgb++ = tmpColor.darker(dark).rgba();
      }
      pixmap = QPixmap::fromImage(image);
      QPixmapCache::insert(name, pixmap);
    }
    brush.setTexture(pixmap);
  } else {
    // Use the color
    brush.setColor(brush.color().darker(dark));
  }
  return brush;
}

QBrush PlastikStyleHelper::lightenBrush(QBrush brush, int light) {
  if (const QGradient *gradient = brush.gradient()) {
    // Use the gradient. Call QColor::lighter() on all color stops.
    QGradientStops stops = gradient->stops();
    QMutableListIterator<QGradientStop> it(stops);
    while (it.hasNext()) {
      it.next();
      it.setValue(QPair<qreal, QColor>(it.value().first,
                                       it.value().second.lighter(light)));
    }

    switch (gradient->type()) {
    case QGradient::RadialGradient: {
      QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
      grad.setStops(stops);
      brush = QBrush(grad);
      break;
    }
    case QGradient::ConicalGradient: {
      QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
      grad.setStops(stops);
      brush = QBrush(grad);
      break;
    }
    default:
      qWarning("PlastikStyle::qBrushLight() - unknown gradient type"
               " - falling back to QLinearGradient");
    case QGradient::LinearGradient: {
      QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
      grad.setStops(stops);
      brush = QBrush(grad);
      break;
    }
    }
  } else if (!brush.texture().isNull()) {
    // Modify the texture - ridiculously expensive.
    QPixmap texture = brush.texture();
    QPixmap pixmap;
    QString name = QLatin1String("qbrushtexture-light") %
                   HexString<int>(light) %
                   HexString<qint64>(texture.cacheKey());

    if (!QPixmapCache::find(name, &pixmap)) {
      QImage image = texture.toImage();
      QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
      int pixels = image.width() * image.height();
      QColor tmpColor;
      while (pixels--) {
        tmpColor.setRgb(*rgb);
        *rgb++ = tmpColor.lighter(light).rgba();
      }
      pixmap = QPixmap::fromImage(image);
      QPixmapCache::insert(name, pixmap);
    }
    brush.setTexture(pixmap);
  } else {
    // Use the color
    brush.setColor(brush.color().lighter(light));
  }
  return brush;
}

void PlastikStyleHelper::setBrushAlphaF(QBrush *brush, qreal alpha) {
  if (const QGradient *gradient = brush->gradient()) {
    // Use the gradient. Call QColor::setAlphaF() on all color stops.
    QGradientStops stops = gradient->stops();
    QMutableListIterator<QGradientStop> it(stops);
    QColor tmpColor;
    while (it.hasNext()) {
      it.next();
      tmpColor = it.value().second;
      tmpColor.setAlphaF(alpha * tmpColor.alphaF());
      it.setValue(QPair<qreal, QColor>(it.value().first, tmpColor));
    }

    switch (gradient->type()) {
    case QGradient::RadialGradient: {
      QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
      grad.setStops(stops);
      *brush = QBrush(grad);
      break;
    }
    case QGradient::ConicalGradient: {
      QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
      grad.setStops(stops);
      *brush = QBrush(grad);
      break;
    }
    default:
      qWarning("PlastikStyle::qBrushLight() - unknown gradient type"
               " - falling back to QLinearGradient");
    case QGradient::LinearGradient: {
      QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
      grad.setStops(stops);
      *brush = QBrush(grad);
      break;
    }
    }
  } else if (!brush->texture().isNull()) {
    // Modify the texture - ridiculously expensive.
    QPixmap texture = brush->texture();
    QPixmap pixmap;
    QString name = QLatin1String("qbrushtexture-alpha") %
                   HexString<qreal>(alpha) %
                   HexString<qint64>(texture.cacheKey());
    if (!QPixmapCache::find(name, &pixmap)) {
      QImage image = texture.toImage();
      QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
      int pixels = image.width() * image.height();
      QColor tmpColor;
      while (pixels--) {
        tmpColor.setRgb(*rgb);
        tmpColor.setAlphaF(alpha * tmpColor.alphaF());
        *rgb++ = tmpColor.rgba();
      }
      pixmap = QPixmap::fromImage(image);
      QPixmapCache::insert(name, pixmap);
    }
    brush->setTexture(pixmap);
  } else {
    // Use the color
    QColor tmpColor = brush->color();
    tmpColor.setAlphaF(alpha * tmpColor.alphaF());
    brush->setColor(tmpColor);
  }
}

void PlastikStyleHelper::drawGradient(QPainter *painter, const QRect &rect,
                                      const QColor &gradientStart,
                                      const QColor &gradientStop) {
  QString gradientName =
      QLatin1String("qplastique-g") % HexString<int>(rect.width()) %
      HexString<int>(rect.height()) % HexString<QRgb>(gradientStart.rgba()) %
      HexString<QRgb>(gradientStop.rgba());

  QPixmap cache;
  QPainter *p = painter;
  QRect r = rect;

  bool doPixmapCache = painter->deviceTransform().isIdentity() &&
                       painter->worldTransform().isIdentity();
  if (doPixmapCache && QPixmapCache::find(gradientName, &cache)) {
    painter->drawPixmap(rect, cache);
  } else {
    if (doPixmapCache) {
      cache = QPixmap(rect.size());
      cache.fill(Qt::transparent);
      p = new QPainter(&cache);
      r = QRect(0, 0, rect.width(), rect.height());
    }

    int x = r.center().x();
    QLinearGradient gradient(x, r.top(), x, r.bottom());
    gradient.setColorAt(0, gradientStart);
    gradient.setColorAt(1, gradientStop);
    p->fillRect(r, gradient);

    if (doPixmapCache) {
      p->end();
      delete p;
      painter->drawPixmap(rect, cache);
      QPixmapCache::insert(gradientName, cache);
    }
  }
}

void PlastikStyleHelper::drawFrame(QPainter *painter, const QRect &rect,
                                   const QStyleOption *option,
                                   QFrame::Shadow shadow) {
  QPen oldPen = painter->pen();
  QBrush border;
  QBrush corner;
  QBrush innerTopLeft;
  QBrush innerBottomRight;

  if (shadow != QFrame::Plain && (option->state & QStyle::State_HasFocus)) {
    border = option->palette.highlight();
    setBrushAlphaF(&border, qreal(0.8));
    corner = option->palette.highlight();
    setBrushAlphaF(&corner, 0.5);
    innerTopLeft = darkenBrush(option->palette.highlight(), 125);
    innerBottomRight = option->palette.highlight();
    setBrushAlphaF(&innerBottomRight, qreal(0.65));
  } else {
    border = option->palette.shadow();
    setBrushAlphaF(&border, qreal(0.4));
    corner = option->palette.shadow();
    setBrushAlphaF(&corner, 0.25);
    innerTopLeft = option->palette.shadow();
    innerBottomRight = option->palette.shadow();
    if (shadow == QFrame::Sunken) {
      setBrushAlphaF(&innerTopLeft, qreal(0.23));
      setBrushAlphaF(&innerBottomRight, qreal(0.075));
    } else {
      setBrushAlphaF(&innerTopLeft, qreal(0.075));
      setBrushAlphaF(&innerBottomRight, qreal(0.23));
    }
  }

  QLine lines[4];
  QPoint points[8];

  // Opaque corner lines
  painter->setPen(QPen(border, 0));
  lines[0] = QLine(rect.left() + 2, rect.top(), rect.right() - 2, rect.top());
  lines[1] =
      QLine(rect.left() + 2, rect.bottom(), rect.right() - 2, rect.bottom());
  lines[2] = QLine(rect.left(), rect.top() + 2, rect.left(), rect.bottom() - 2);
  lines[3] =
      QLine(rect.right(), rect.top() + 2, rect.right(), rect.bottom() - 2);
  painter->drawLines(lines, 4);

  // Opaque corner dots
  points[0] = QPoint(rect.left() + 1, rect.top() + 1);
  points[1] = QPoint(rect.left() + 1, rect.bottom() - 1);
  points[2] = QPoint(rect.right() - 1, rect.top() + 1);
  points[3] = QPoint(rect.right() - 1, rect.bottom() - 1);
  painter->drawPoints(points, 4);

  // Shaded corner dots
  painter->setPen(QPen(corner, 0));
  points[0] = QPoint(rect.left(), rect.top() + 1);
  points[1] = QPoint(rect.left(), rect.bottom() - 1);
  points[2] = QPoint(rect.left() + 1, rect.top());
  points[3] = QPoint(rect.left() + 1, rect.bottom());
  points[4] = QPoint(rect.right(), rect.top() + 1);
  points[5] = QPoint(rect.right(), rect.bottom() - 1);
  points[6] = QPoint(rect.right() - 1, rect.top());
  points[7] = QPoint(rect.right() - 1, rect.bottom());
  painter->drawPoints(points, 8);

  // Shadows
  if (shadow != QFrame::Plain) {
    painter->setPen(QPen(innerTopLeft, 0));
    lines[0] = QLine(rect.left() + 2, rect.top() + 1, rect.right() - 2,
                     rect.top() + 1);
    lines[1] = QLine(rect.left() + 1, rect.top() + 2, rect.left() + 1,
                     rect.bottom() - 2);
    painter->drawLines(lines, 2);
    painter->setPen(QPen(innerBottomRight, 0));
    lines[0] = QLine(rect.left() + 2, rect.bottom() - 1, rect.right() - 2,
                     rect.bottom() - 1);
    lines[1] = QLine(rect.right() - 1, rect.top() + 2, rect.right() - 1,
                     rect.bottom() - 2);
    painter->drawLines(lines, 2);
  }

  painter->setPen(oldPen);
}

void PlastikStyleHelper::drawFrame(QPainter *painter,
                                   const QStyleOption *option,
                                   const QWidget *widget) {
  QRect rect = option->rect;
  QPen oldPen = painter->pen();

  QColor borderColor = option->palette.window().color().darker(178);
  QColor gradientStartColor = option->palette.button().color().lighter(104);
  QColor gradientStopColor = option->palette.button().color().darker(105);
  QColor alphaCornerColor;
  if (widget) {
    // ### backgroundrole/foregroundrole should be part of the style option
    alphaCornerColor = mergeColors(
        option->palette.color(widget->backgroundRole()), borderColor);
  } else {
    alphaCornerColor =
        mergeColors(option->palette.window().color(), borderColor);
  }

  QLine lines[4];
  QPoint points[8];

  // outline / border
  painter->setPen(borderColor);
  lines[0] = QLine(rect.left() + 2, rect.top(), rect.right() - 2, rect.top());
  lines[1] =
      QLine(rect.left() + 2, rect.bottom(), rect.right() - 2, rect.bottom());
  lines[2] = QLine(rect.left(), rect.top() + 2, rect.left(), rect.bottom() - 2);
  lines[3] =
      QLine(rect.right(), rect.top() + 2, rect.right(), rect.bottom() - 2);
  painter->drawLines(lines, 4);

  points[0] = QPoint(rect.left() + 1, rect.top() + 1);
  points[1] = QPoint(rect.right() - 1, rect.top() + 1);
  points[2] = QPoint(rect.left() + 1, rect.bottom() - 1);
  points[3] = QPoint(rect.right() - 1, rect.bottom() - 1);
  painter->drawPoints(points, 4);

  painter->setPen(alphaCornerColor);

  points[0] = QPoint(rect.left() + 1, rect.top());
  points[1] = QPoint(rect.right() - 1, rect.top());
  points[2] = QPoint(rect.left() + 1, rect.bottom());
  points[3] = QPoint(rect.right() - 1, rect.bottom());
  points[4] = QPoint(rect.left(), rect.top() + 1);
  points[5] = QPoint(rect.right(), rect.top() + 1);
  points[6] = QPoint(rect.left(), rect.bottom() - 1);
  points[7] = QPoint(rect.right(), rect.bottom() - 1);
  painter->drawPoints(points, 8);

  // inner border
  if ((option->state & QStyle::State_Sunken) ||
      (option->state & QStyle::State_On))
    painter->setPen(option->palette.button().color().darker(118));
  else
    painter->setPen(gradientStartColor);

  lines[0] = QLine(rect.left() + 2, rect.top() + 1, rect.right() - 2,
                   option->rect.top() + 1);
  lines[1] = QLine(rect.left() + 1, rect.top() + 2, rect.left() + 1,
                   option->rect.bottom() - 2);
  painter->drawLines(lines, 2);

  if ((option->state & QStyle::State_Sunken) ||
      (option->state & QStyle::State_On))
    painter->setPen(option->palette.button().color().darker(110));
  else
    painter->setPen(gradientStopColor.darker(102));

  lines[0] = QLine(rect.left() + 2, rect.bottom() - 1, rect.right() - 2,
                   rect.bottom() - 1);
  lines[1] = QLine(rect.right() - 1, rect.top() + 2, rect.right() - 1,
                   rect.bottom() - 2);
  painter->drawLines(lines, 2);

  painter->setPen(oldPen);
}

void PlastikStyleHelper::drawShadePanel(QPainter *painter,
                                        const QStyleOption *option, bool base,
                                        const QWidget *widget) {
  QRect rect = option->rect;
  QPen oldPen = painter->pen();

  QColor gradientStartColor = option->palette.button().color().lighter(104);
  QColor gradientStopColor = option->palette.button().color().darker(105);

  // gradient fill
  if ((option->state & QStyle::State_Enabled) ||
      !(option->state & QStyle::State_AutoRaise)) {
    if ((option->state & QStyle::State_Sunken) ||
        (option->state & QStyle::State_On)) {
      drawGradient(painter, rect.adjusted(1, 1, -1, -1),
                   option->palette.button().color().darker(114),
                   option->palette.button().color().darker(106));
    } else if ((option->state &
                QStyle::State_MouseOver)) { // paint a graident on hover
      drawGradient(painter, rect.adjusted(1, 1, -1, -1),
                   option->palette.button().color().darker(106),
                   option->palette.button().color().darker(98));
    } else {
      drawGradient(painter, rect.adjusted(1, 1, -1, -1),
                   base ? option->palette.window().color().lighter(105)
                        : gradientStartColor,
                   base ? option->palette.window().color().darker(102)
                        : gradientStopColor);
    }
  }

  drawFrame(painter, option, widget);

  painter->setPen(oldPen);
}

void PlastikStyleHelper::drawMdiButton(QPainter *painter,
                                       const QStyleOptionTitleBar *option,
                                       const QRect &tmp, bool hover,
                                       bool sunken) {
  if (tmp.isNull())
    return;
  bool active = (option->titleBarState & QStyle::State_Active);

  // ### use palette colors instead
  QColor mdiButtonGradientStartColor;
  QColor mdiButtonGradientStopColor;
  if (active) {
    mdiButtonGradientStartColor =
        QColor((hover || sunken) ? 0x7d8bb1 : 0x55689a);
    mdiButtonGradientStopColor =
        QColor((hover || sunken) ? 0x939ebe : 0x7381ab);
  } else {
    mdiButtonGradientStartColor =
        QColor((hover || sunken) ? 0x9e9e9e : 0x818181);
    mdiButtonGradientStopColor =
        QColor((hover || sunken) ? 0xababab : 0x929292);
  }

  drawGradient(painter, tmp.adjusted(1, 1, -1, -1), mdiButtonGradientStartColor,
               mdiButtonGradientStopColor);

  QColor mdiButtonBorderColor;
  if (active) {
    mdiButtonBorderColor =
        (hover || sunken) ? QColor(0x627097) : QColor(0x324577);
  } else {
    mdiButtonBorderColor =
        (hover || sunken) ? QColor(0x838383) : QColor(0x5e5e5e);
  }
  painter->setPen(QPen(mdiButtonBorderColor, 1));

  const QLine lines[4] = {
      QLine(tmp.left() + 2, tmp.top(), tmp.right() - 2, tmp.top()),
      QLine(tmp.left() + 2, tmp.bottom(), tmp.right() - 2, tmp.bottom()),
      QLine(tmp.left(), tmp.top() + 2, tmp.left(), tmp.bottom() - 2),
      QLine(tmp.right(), tmp.top() + 2, tmp.right(), tmp.bottom() - 2)};
  painter->drawLines(lines, 4);

  const QPoint points[4] = {QPoint(tmp.left() + 1, tmp.top() + 1),
                            QPoint(tmp.right() - 1, tmp.top() + 1),
                            QPoint(tmp.left() + 1, tmp.bottom() - 1),
                            QPoint(tmp.right() - 1, tmp.bottom() - 1)};
  painter->drawPoints(points, 4);
}

QString PlastikStyleHelper::elliditide(const QString &text,
                                       const QFontMetrics &fontMetrics,
                                       const QRect &rect, int *textWidth) {
  // Chop and insert ellide into title if text is too wide
  QString title = text;
  int width = textWidth ? *textWidth : fontMetrics.horizontalAdvance(text);
  QString ellipsis = QLatin1String("...");
  if (width > rect.width()) {
    QString leftHalf = title.left(title.size() / 2);
    QString rightHalf = title.mid(leftHalf.size() + 1);
    while (!leftHalf.isEmpty() && !rightHalf.isEmpty()) {
      leftHalf.chop(1);
      int width =
          fontMetrics.horizontalAdvance(leftHalf + ellipsis + rightHalf);
      if (width < rect.width()) {
        title = leftHalf + ellipsis + rightHalf;
        break;
      }
      rightHalf.remove(0, 1);
      width = fontMetrics.horizontalAdvance(leftHalf + ellipsis + rightHalf);
      if (width < rect.width()) {
        title = leftHalf + ellipsis + rightHalf;
        break;
      }
    }
  }
  if (textWidth)
    *textWidth = width;
  return title;
}

void PlastikStyleHelper::drawHandle(QPainter *painter,
                                    const QStyleOption *option,
                                    const QRect &rect,
                                    Qt::Orientation orientation,
                                    const QWidget *widget) {
  QColor borderColor = option->palette.window().color().darker(178);
  QColor alphaCornerColor;
  if (widget) {
    // ### backgroundrole/foregroundrole should be part of the style option
    alphaCornerColor = mergeColors(
        option->palette.color(widget->backgroundRole()), borderColor);
  } else {
    alphaCornerColor =
        mergeColors(option->palette.window().color(), borderColor);
  }
  QImage handle(simpleToolBarHandle);
  alphaCornerColor.setAlpha(170);
  handle.setColor(1, alphaCornerColor.rgba());
  handle.setColor(
      2, mergeColors(alphaCornerColor, option->palette.light().color()).rgba());
  handle.setColor(3, option->palette.light().color().rgba());

  const int spacing = 2;

  if (orientation == Qt::Vertical) {
    int nchunks = rect.width() / (handle.width() + spacing);
    for (int i = 0; i < nchunks; ++i)
      painter->drawImage(
          QPoint(rect.left() + i * (handle.width() + spacing), rect.top()),
          handle);
  } else {
    int nchunks = rect.height() / (handle.height() + spacing);
    for (int i = 0; i < nchunks; ++i)
      painter->drawImage(
          QPoint(rect.left(), rect.top() + i * (handle.height() + spacing)),
          handle);
  }
}

/*
 *    Used by spin- and combo box.
 *    Draws a rounded frame around rect but omits the right hand edge
 */

void PlastikStyleHelper::drawPartialFrame(QPainter *painter,
                                          const QStyleOptionComplex *option,
                                          const QRect &rect,
                                          const QWidget *widget,
                                          const QStyle *style) {
  // need to check whether widget == nullptr
  bool reverse = option->direction == Qt::RightToLeft;
  QStyleOptionFrame frameOpt;
#ifndef QT_NO_LINEEDIT
  if (widget) // do we even have a widget available to do a search with?
  {
    QLineEdit *lineedit =
        widget->findChild<QLineEdit *>(); // this seems to crash kcm_kscreen
    if (lineedit) {
      frameOpt.initFrom(lineedit);
    }
  }
#else
  Q_UNUSED(widget)
#endif // QT_NO_LINEEDIT

  frameOpt.rect = rect;
  painter->save();
  frameOpt.rect.adjust(-blueFrameWidth + (reverse ? 1 : 0), -blueFrameWidth,
                       blueFrameWidth + (reverse ? 0 : -1), blueFrameWidth);
  painter->setClipRect(frameOpt.rect);
  frameOpt.rect.adjust(reverse ? -2 : 0, 0, reverse ? 0 : 2, 0);
  frameOpt.lineWidth = style->pixelMetric(QStyle::PM_DefaultFrameWidth);
  frameOpt.midLineWidth = 0;
  frameOpt.state = option->state | QStyle::State_Sunken;
  frameOpt.palette = option->palette;
  style->drawPrimitive(QStyle::PE_PanelLineEdit, &frameOpt, painter, widget);
  painter->restore();

  // Draw a two pixel highlight on the flat edge
  if (option->state & QStyle::State_HasFocus) {
    painter->setPen(QPen(option->palette.highlight(), 0));
    QBrush focusBorder = option->palette.highlight();
    setBrushAlphaF(&focusBorder, qreal(0.65));
    if (!reverse) {
      painter->drawLine(rect.topRight() + QPoint(1, -1),
                        rect.bottomRight() + QPoint(1, 1));
      painter->setPen(QPen(focusBorder, 0));
      painter->drawLine(rect.topRight(), rect.bottomRight());
    } else {
      painter->drawLine(rect.topLeft() + QPoint(-1, -1),
                        rect.bottomLeft() + QPoint(-1, 1));
      painter->setPen(QPen(focusBorder, 0));
      painter->drawLine(rect.topLeft(), rect.bottomLeft());
    }
  }
}
