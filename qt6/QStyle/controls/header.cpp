#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylehelper.h"

#include <QPainter>
#include <QPixmapCache>

void PlastikStyle::drawHeaderSection(const QStyleOption *option, QPainter *painter, QColor gradientStartColor, QColor gradientStopColor, QColor borderColor, QColor alphaCornerColor) const{
    // Draws the header in tables.
    if (const QStyleOptionHeader *header =
        qstyleoption_cast<const QStyleOptionHeader *>(option)) {
        QPixmap cache;
    QString pixmapName = QStyleHelper::uniqueName(
        QLatin1String("headersection"), option, option->rect.size());
    pixmapName += QString::number(-int(header->position));
    pixmapName += QString::number(-int(header->orientation));

    if (!QPixmapCache::find(pixmapName, &cache)) {
        cache = QPixmap(option->rect.size());
        cache.fill(Qt::white);
        QRect pixmapRect(0, 0, option->rect.width(), option->rect.height());
        QPainter cachePainter(&cache);

        bool sunken =
        (header->state & State_Enabled) && (header->state & State_Sunken);

        QColor headerGradientStart =
        sunken ? option->palette.window().color().darker(114)
        : gradientStartColor;
        QColor headerGradientStop =
        sunken ? option->palette.window().color().darker(106)
        : gradientStopColor;

        QColor lightLine = sunken ? option->palette.window().color().darker(118)
        : gradientStartColor;
        QColor darkLine = sunken ? option->palette.window().color().darker(110)
        : gradientStopColor.darker(105);

        PlastikStyleHelper::drawGradient(
            &cachePainter, pixmapRect, headerGradientStart, headerGradientStop);

        cachePainter.setPen(borderColor);
        cachePainter.drawRect(pixmapRect.adjusted(0, 0, -1, -1));
        cachePainter.setPen(alphaCornerColor);

        const QPoint points[4] = {pixmapRect.topLeft(), pixmapRect.topRight(),
            pixmapRect.bottomLeft(),
            pixmapRect.bottomRight()};
            cachePainter.drawPoints(points, 4);

            QLine lines[2];

            // inner lines
            cachePainter.setPen(lightLine);
            lines[0] = QLine(pixmapRect.left() + 2, pixmapRect.top() + 1,
                             pixmapRect.right() - 2, pixmapRect.top() + 1);
            lines[1] = QLine(pixmapRect.left() + 1, pixmapRect.top() + 2,
                             pixmapRect.left() + 1, pixmapRect.bottom() - 2);
            cachePainter.drawLines(lines, 2);

            cachePainter.setPen(darkLine);
            lines[0] = QLine(pixmapRect.left() + 2, pixmapRect.bottom() - 1,
                             pixmapRect.right() - 2, pixmapRect.bottom() - 1);
            lines[1] = QLine(pixmapRect.right() - 1, pixmapRect.bottom() - 2,
                             pixmapRect.right() - 1, pixmapRect.top() + 2);
            cachePainter.drawLines(lines, 2);

            cachePainter.end();
            QPixmapCache::insert(pixmapName, cache);
    }
    painter->drawPixmap(option->rect.topLeft(), cache);
        }
}
