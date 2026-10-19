#include "plastikstyle.h"
#include "plastikstylehelper.h"

#include <QPainter>
#include <QStyleOptionRubberBand>

void PlastikStyle::drawRubberBand(const QStyleOption *option, QPainter *painter,
                                  const QWidget *widget) const {
  if (qstyleoption_cast<const QStyleOptionRubberBand *>(option)) {
    // ### workaround for slow general painter path
    // should I save in stylecache?
    QPixmap tiledPixmap(16, 16);
    QPainter pixmapPainter(&tiledPixmap);
    pixmapPainter.setPen(Qt::NoPen);
    pixmapPainter.setBrush(Qt::Dense4Pattern);
    pixmapPainter.setBackground(Qt::white);
    pixmapPainter.setBackgroundMode(Qt::OpaqueMode);
    pixmapPainter.drawRect(0, 0, tiledPixmap.width(), tiledPixmap.height());
    pixmapPainter.end();
    tiledPixmap = QPixmap::fromImage(tiledPixmap.toImage());
    painter->save();
    QRect r = option->rect;
    QStyleHintReturnMask mask;
    if (proxy()->styleHint(QStyle::SH_RubberBand_Mask, option, widget, &mask))
      painter->setClipRegion(mask.region);
    painter->drawTiledPixmap(r.x(), r.y(), r.width(), r.height(), tiledPixmap);
    painter->restore();
  }
}
