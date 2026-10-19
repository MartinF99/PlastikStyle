#include "plastikstyle.h"
#include "plastikstylehelper.h"

#include <QPainter>

void PlastikStyle::drawItemViewItemCheck(const QStyleOption *option,
                                         QPainter *painter,
                                         const QWidget *widget) const {
  QStyleOptionButton button;
  button.QStyleOption::operator=(*option);
  button.state &= ~State_MouseOver;
  proxy()->drawPrimitive(PE_IndicatorCheckBox, &button, painter, widget);
}
