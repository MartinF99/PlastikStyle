#include <QPainter>
#include <QTabBar>
// QStyleoptions
#include <QStyleOptionTabBarBase>
#include <QStyleOptionTabWidgetFrame>

#include "plastikstyle.h"
#include "plastikstylehelper.h"

void PlastikStyle::drawTabWidgetFrame(PrimitiveElement element,
                                      const QStyleOption *option,
                                      QPainter *painter,
                                      const QWidget *widget) const {
  if (const QStyleOptionTabWidgetFrame *twf =
          qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option)) {
    if (twf->shape != QTabBar::RoundedNorth &&
        twf->shape != QTabBar::RoundedWest &&
        twf->shape != QTabBar::RoundedSouth &&
        twf->shape != QTabBar::RoundedEast) {
      QCommonStyle::drawPrimitive(element, option, painter, widget);
      return;
    }

    int borderThickness =
        proxy()->pixelMetric(PM_TabBarBaseOverlap, twf, widget);
    bool reverse = (twf->direction == Qt::RightToLeft);

    painter->save();

    // Start by filling the contents of the tab widget frame (which is
    // actually a panel).
    painter->fillRect(option->rect.adjusted(1, 1, -1, -1),
                      option->palette.window());

    QRect tabBarRect;
    switch (twf->shape) {
    case QTabBar::RoundedNorth:
      if (reverse)
        tabBarRect =
            QRect(twf->rect.right() - twf->leftCornerWidgetSize.width() -
                      twf->tabBarSize.width() + 1,
                  twf->rect.top(), twf->tabBarSize.width(), borderThickness);
      else
        tabBarRect =
            QRect(twf->rect.left() + twf->leftCornerWidgetSize.width(),
                  twf->rect.top(), twf->tabBarSize.width(), borderThickness);
      break;
    case QTabBar::RoundedWest:
      tabBarRect = QRect(twf->rect.left(),
                         twf->rect.top() + twf->leftCornerWidgetSize.height(),
                         borderThickness, twf->tabBarSize.height());
      break;
    case QTabBar::RoundedEast:
      tabBarRect = QRect(twf->rect.right() - borderThickness + 1,
                         twf->rect.top() + twf->leftCornerWidgetSize.height(),
                         borderThickness, twf->tabBarSize.height());
      break;
    case QTabBar::RoundedSouth:
      if (reverse)
        tabBarRect =
            QRect(twf->rect.right() - twf->leftCornerWidgetSize.width() -
                      twf->tabBarSize.width() + 1,
                  twf->rect.bottom() - borderThickness + 1,
                  twf->tabBarSize.width(), borderThickness);
      else
        tabBarRect = QRect(twf->rect.left() + twf->leftCornerWidgetSize.width(),
                           twf->rect.bottom() - borderThickness + 1,
                           twf->tabBarSize.width(), borderThickness);
      break;
    default:
      break;
    }

    QRegion region(twf->rect);
    region -= tabBarRect;
    painter->setClipRegion(region);

    // Outer border
    QLine leftLine = QLine(twf->rect.topLeft() + QPoint(0, 2),
                           twf->rect.bottomLeft() - QPoint(0, 2));
    QLine rightLine = QLine(twf->rect.topRight() + QPoint(0, 2),
                            twf->rect.bottomRight() - QPoint(0, 2));
    QLine bottomLine = QLine(twf->rect.bottomLeft() + QPoint(2, 0),
                             twf->rect.bottomRight() - QPoint(2, 0));
    QLine topLine = QLine(twf->rect.topLeft() + QPoint(2, 0),
                          twf->rect.topRight() - QPoint(2, 0));

    QBrush border = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.4));
    painter->setPen(QPen(border, 0));

    QVarLengthArray<QLine, 4> lines;
    QVarLengthArray<QPoint, 8> points;

    lines.append(topLine);

    // Inner border
    QLine innerLeftLine =
        QLine(leftLine.p1() + QPoint(1, 0), leftLine.p2() + QPoint(1, 0));
    QLine innerRightLine =
        QLine(rightLine.p1() - QPoint(1, 0), rightLine.p2() - QPoint(1, 0));
    QLine innerBottomLine =
        QLine(bottomLine.p1() - QPoint(0, 1), bottomLine.p2() - QPoint(0, 1));
    QLine innerTopLine =
        QLine(topLine.p1() + QPoint(0, 1), topLine.p2() + QPoint(0, 1));

    // Rounded Corner
    QPoint leftBottomOuterCorner = QPoint(innerLeftLine.p2() + QPoint(0, 1));
    QPoint leftBottomInnerCorner1 = QPoint(leftLine.p2() + QPoint(0, 1));
    QPoint leftBottomInnerCorner2 = QPoint(bottomLine.p1() - QPoint(1, 0));
    QPoint rightBottomOuterCorner = QPoint(innerRightLine.p2() + QPoint(0, 1));
    QPoint rightBottomInnerCorner1 = QPoint(rightLine.p2() + QPoint(0, 1));
    QPoint rightBottomInnerCorner2 = QPoint(bottomLine.p2() + QPoint(1, 0));
    QPoint rightTopOuterCorner = QPoint(innerRightLine.p1() - QPoint(0, 1));
    QPoint rightTopInnerCorner1 = QPoint(rightLine.p1() - QPoint(0, 1));
    QPoint rightTopInnerCorner2 = QPoint(topLine.p2() + QPoint(1, 0));
    QPoint leftTopOuterCorner = QPoint(innerLeftLine.p1() - QPoint(0, 1));
    QPoint leftTopInnerCorner1 = QPoint(leftLine.p1() - QPoint(0, 1));
    QPoint leftTopInnerCorner2 = QPoint(topLine.p1() - QPoint(1, 0));

    lines.append(leftLine);
    lines.append(rightLine);
    lines.append(bottomLine);

    painter->drawLines(lines.constData(), lines.size());
    lines.clear();

    points.append(leftBottomOuterCorner);
    points.append(rightBottomOuterCorner);
    points.append(rightTopOuterCorner);
    points.append(leftTopOuterCorner);

    painter->drawPoints(points.constData(), points.size());
    points.clear();

    QBrush innerTopLeft = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&innerTopLeft, qreal(0.075));
    painter->setPen(QPen(innerTopLeft, 0));

    lines.append(innerLeftLine);
    lines.append(innerTopLine);
    painter->drawLines(lines.constData(), lines.size());
    lines.clear();

    QBrush innerBottomRight = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&innerBottomRight, qreal(0.23));
    painter->setPen(QPen(innerBottomRight, 0));
    lines.append(innerRightLine);
    lines.append(innerBottomLine);
    painter->drawLines(lines.constData(), lines.size());
    lines.clear();

    QBrush corner = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&corner, 0.25);
    painter->setPen(QPen(corner, 0));
    points.append(leftBottomInnerCorner1);
    points.append(leftBottomInnerCorner2);
    points.append(rightBottomInnerCorner1);
    points.append(rightBottomInnerCorner2);
    points.append(rightTopInnerCorner1);
    points.append(rightTopInnerCorner2);
    points.append(leftTopInnerCorner1);
    points.append(leftTopInnerCorner2);
    painter->drawPoints(points.constData(), points.size());
    points.clear();

    painter->restore();
  }
}

void PlastikStyle::drawTabBarBaseFrame(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const {
  if (const QStyleOptionTabBarBase *tbb =
          qstyleoption_cast<const QStyleOptionTabBarBase *>(option)) {

    // if not rounded on top, bottom, left or right, draw QCommonStyle's
    // TabBarFrame
    if (tbb->shape != QTabBar::RoundedNorth &&
        tbb->shape != QTabBar::RoundedWest &&
        tbb->shape != QTabBar::RoundedSouth &&
        tbb->shape != QTabBar::RoundedEast) {
      QCommonStyle::drawPrimitive(element, option, painter, widget);
      return;
    }

    // draw the tabbar rounded at the correct spot...
    painter->save();

    QRegion region(tbb->rect);
    region -= tbb->tabBarRect;
    painter->setClipRegion(region);

    QLine topLine = QLine(tbb->rect.bottomLeft() - QPoint(0, 1),
                          tbb->rect.bottomRight() - QPoint(0, 1));
    QLine bottomLine = QLine(tbb->rect.bottomLeft(), tbb->rect.bottomRight());

    QBrush border = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.4));
    QBrush innerTopLeft = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&innerTopLeft, qreal(0.075));
    QBrush innerBottomRight = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&innerBottomRight, qreal(0.23));
    QBrush corner = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&corner, 0.25);
    if (tbb->shape == QTabBar::RoundedSouth)
      painter->setPen(QPen(corner, 0));
    else
      painter->setPen(QPen(border, 0));
    painter->drawLine(topLine);

    if (tbb->shape != QTabBar::RoundedSouth)
      painter->setPen(QPen(innerTopLeft, 0));
    else
      painter->setPen(QPen(border, 0));
    painter->drawLine(bottomLine);

    painter->restore();
  }
}

void PlastikStyle::drawTabBarTabShape(
    ControlElement element, const QStyleOption *option, QPainter *painter,
    QColor lightInnerHighlightBorder, QColor darkInnerHighlightBorder,
    QColor lightShadowGradientStart, QColor lightShadowGradientStop,
    const QWidget *widget) const {
  if (const QStyleOptionTab *tab =
          qstyleoption_cast<const QStyleOptionTab *>(option)) {

    if (tab->shape != QTabBar::RoundedNorth &&
        tab->shape != QTabBar::RoundedWest &&
        tab->shape != QTabBar::RoundedSouth &&
        tab->shape != QTabBar::RoundedEast) {
      QCommonStyle::drawControl(element, option, painter, widget);
      return;
    }

    painter->save();

    // Set up some convenience variables
    bool disabled = !(tab->state & State_Enabled);
    bool onlyTab = tab->position == QStyleOptionTab::OnlyOneTab;
    bool selected = tab->state & State_Selected;
    bool mouseOver = (tab->state & State_MouseOver) && !selected && !disabled;
    bool previousSelected =
        tab->selectedPosition == QStyleOptionTab::PreviousIsSelected;
    bool nextSelected =
        tab->selectedPosition == QStyleOptionTab::NextIsSelected;
    bool leftCornerWidget =
        (tab->cornerWidgets & QStyleOptionTab::LeftCornerWidget);
    bool reverse = (tab->direction == Qt::RightToLeft);

    int lowerTop =
        selected ? 0 : 3; // to make the selected tab bigger than the rest
    bool atEnd = (tab->position == QStyleOptionTab::End) || onlyTab;
    bool atBeginning =
        ((tab->position == QStyleOptionTab::Beginning) || onlyTab) &&
        !leftCornerWidget;
    bool reverseShadow = false;

    int borderThickness =
        proxy()->pixelMetric(PM_TabBarBaseOverlap, tab, widget);
    int marginLeft = 0;
    if ((atBeginning && !selected) ||
        (selected && leftCornerWidget &&
         ((tab->position == QStyleOptionTab::Beginning) || onlyTab))) {
      marginLeft = 1;
    }

    // I've set the names based on the natural coordinate system. Vectors are
    // used to rotate everything if the orientation of the tab bare is
    // different than north.
    // Coordinates of corners of rectangle for transformation
    QPoint topLeft;
    QPoint topRight;
    QPoint bottomLeft;
    QPoint bottomRight;

    // Fill with normalized vectors in the direction of the coordinate
    // system (down and right should be complement of up and left, or it
    // will look odd)
    QPoint vectorUp;
    QPoint vectorDown;
    QPoint vectorLeft;
    QPoint vectorRight;

    QBrush border = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.4));
    QBrush innerTopLeft = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&innerTopLeft, qreal(0.075));
    QBrush innerBottomRight = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&innerBottomRight, qreal(0.23));
    QBrush corner = option->palette.shadow();
    PlastikStyleHelper::setBrushAlphaF(&corner, qreal(0.25));

    QBrush baseColor1;
    QBrush baseColor2;

    switch (tab->shape) {
    case QTabBar::RoundedNorth:
      vectorUp = QPoint(0, -1);
      vectorDown = QPoint(0, 1);

      if (reverse) {
        vectorLeft = QPoint(1, 0);
        vectorRight = QPoint(-1, 0);
        reverseShadow = true;
      } else {
        vectorLeft = QPoint(-1, 0);
        vectorRight = QPoint(1, 0);
      }

      if (reverse) {
        topLeft = tab->rect.topRight();
        topRight = tab->rect.topLeft();
        bottomLeft = tab->rect.bottomRight();
        bottomRight = tab->rect.bottomLeft();
      } else {
        topLeft = tab->rect.topLeft();
        topRight = tab->rect.topRight();
        bottomLeft = tab->rect.bottomLeft();
        bottomRight = tab->rect.bottomRight();
      }

      baseColor1 = border;
      baseColor2 = innerTopLeft;
      break;
    case QTabBar::RoundedWest:
      vectorUp = QPoint(-1, 0);
      vectorDown = QPoint(1, 0);
      vectorLeft = QPoint(0, -1);
      vectorRight = QPoint(0, 1);

      topLeft = tab->rect.topLeft();
      topRight = tab->rect.bottomLeft();
      bottomLeft = tab->rect.topRight();
      bottomRight = tab->rect.bottomRight();

      baseColor1 = border;
      baseColor2 = innerTopLeft;
      break;
    case QTabBar::RoundedEast:
      vectorUp = QPoint(1, 0);
      vectorDown = QPoint(-1, 0);
      vectorLeft = QPoint(0, -1);
      vectorRight = QPoint(0, 1);

      topLeft = tab->rect.topRight();
      topRight = tab->rect.bottomRight();
      bottomLeft = tab->rect.topLeft();
      bottomRight = tab->rect.bottomLeft();

      baseColor1 = border;
      baseColor2 = innerBottomRight;
      break;
    case QTabBar::RoundedSouth:
      vectorUp = QPoint(0, 1);
      vectorDown = QPoint(0, -1);

      if (reverse) {
        vectorLeft = QPoint(1, 0);
        vectorRight = QPoint(-1, 0);
        reverseShadow = true;

        topLeft = tab->rect.bottomRight();
        topRight = tab->rect.bottomLeft();
        bottomLeft = tab->rect.topRight();
        bottomRight = tab->rect.topLeft();
      } else {
        vectorLeft = QPoint(-1, 0);
        vectorRight = QPoint(1, 0);

        topLeft = tab->rect.bottomLeft();
        topRight = tab->rect.bottomRight();
        bottomLeft = tab->rect.topLeft();
        bottomRight = tab->rect.topRight();
      }

      baseColor1 = border;
      baseColor2 = innerBottomRight;
      break;
    default:
      break;
    }

    // Make the tab smaller when it's at the end, so that we are able to
    // draw the corner
    if (atEnd) {
      topRight += vectorLeft;
      bottomRight += vectorLeft;
    }

    // Outer border
    QLine topLine;
    QPoint adjustTopLineLeft =
        (vectorRight * (marginLeft + (previousSelected ? 0 : 1))) +
        (vectorDown * lowerTop);
    QPoint adjustTopLineRight = (vectorDown * lowerTop);
    if (atBeginning || selected)
      adjustTopLineLeft += vectorRight;
    if (atEnd || selected)
      adjustTopLineRight += 2 * vectorLeft;

    topLine = QLine(topLeft + adjustTopLineLeft, topRight + adjustTopLineRight);

    QLine leftLine;
    QPoint adjustLeftLineTop =
        (vectorRight * marginLeft) + (vectorDown * (lowerTop + 1));
    QPoint adjustLeftLineBottom =
        (vectorRight * marginLeft) + (vectorUp * borderThickness);
    if (atBeginning || selected) {
      adjustLeftLineTop += vectorDown;
    } // Make place for rounded corner
    if (atBeginning && selected) {
      adjustLeftLineBottom += borderThickness * vectorDown;
    } else if (selected) {
      adjustLeftLineBottom += vectorUp;
    }

    leftLine =
        QLine(topLeft + adjustLeftLineTop, bottomLeft + adjustLeftLineBottom);

    QLine rightLine;
    QPoint adjustRightLineTop = vectorDown * (2 + lowerTop);
    QPoint adjustRightLineBottom = vectorUp * borderThickness;
    if (selected) {
      adjustRightLineBottom += vectorUp;
    }

    rightLine = QLine(topRight + adjustRightLineTop,
                      bottomRight + adjustRightLineBottom);

    // Background
    QPoint startPoint = topLine.p1() + vectorDown + vectorLeft;
    if (mouseOver) {
      startPoint += vectorDown;
    }
    QPoint endPoint = rightLine.p2();

    if (tab->state & State_Enabled) {
      QRect fillRect = QRect(startPoint, endPoint).normalized();
      if (fillRect.isValid()) {
        if (selected) {
          fillRect = QRect(startPoint, endPoint + vectorLeft + vectorDown * 3)
                         .normalized();
          painter->fillRect(fillRect, option->palette.window());

          // Connect to the base
          painter->setPen(QPen(option->palette.window(), 0));
          QVarLengthArray<QPoint, 6> points;
          points.append(rightLine.p2() + vectorDown);
          points.append(rightLine.p2() + vectorDown + vectorDown);
          points.append(rightLine.p2() + vectorDown + vectorDown + vectorRight);
          if (tab->position != QStyleOptionTab::Beginning) {
            points.append(leftLine.p2() + vectorDown);
            points.append(leftLine.p2() + vectorDown + vectorDown);
            points.append(leftLine.p2() + vectorDown + vectorDown + vectorLeft);
          }
          painter->drawPoints(points.constData(), points.size());
        } else {
          QBrush buttonGradientBrush;
          QBrush buttonBrush = PlastikStyleHelper::mapBrushToRectangle(
              option->palette.button(), fillRect);
          if (buttonBrush.gradient() || !buttonBrush.texture().isNull()) {
            buttonGradientBrush = buttonBrush;
          } else {
            // Generate gradients
            QLinearGradient buttonGradient(fillRect.topLeft(),
                                           fillRect.bottomLeft());
            buttonGradient.setColorAt(0.0, buttonBrush.color().lighter(104));
            buttonGradient.setColorAt(1.0, buttonBrush.color().darker(110));
            buttonGradientBrush = QBrush(buttonGradient);
          }

          painter->fillRect(fillRect, buttonGradientBrush);
        }
      }
    }

    QPoint rightCornerDot = topRight + vectorLeft + (lowerTop + 1) * vectorDown;
    QPoint leftCornerDot =
        topLeft + (marginLeft + 1) * vectorRight + (lowerTop + 1) * vectorDown;
    QPoint bottomRightConnectToBase = rightLine.p2() + vectorRight + vectorDown;
    QPoint bottomLeftConnectToBase = leftLine.p2() + vectorLeft + vectorDown;

    painter->setPen(QPen(border, 0));

    QVarLengthArray<QLine, 3> lines;
    QVarLengthArray<QPoint, 7> points;

    lines.append(topLine);

    if (mouseOver) {
      painter->drawLines(lines.constData(), lines.count());
      lines.clear();

      QLine secondHoverLine =
          QLine(topLine.p1() + vectorDown * 2 + vectorLeft,
                topLine.p2() + vectorDown * 2 + vectorRight);
      painter->setPen(lightInnerHighlightBorder);
      painter->drawLine(secondHoverLine);
    }

    if (mouseOver)
      painter->setPen(QPen(border, 0));

    if (!previousSelected) {
      lines.append(leftLine);
    }
    if (atEnd || selected) {
      lines.append(rightLine);
      points.append(rightCornerDot);
    }
    if (atBeginning || selected) {
      points.append(leftCornerDot);
    }
    if (selected) {
      points.append(bottomRightConnectToBase);
      points.append(bottomLeftConnectToBase);
    }
    if (lines.size() > 0) {
      painter->drawLines(lines.constData(), lines.size());
      lines.clear();
    }
    if (points.size() > 0) {
      painter->drawPoints(points.constData(), points.size());
      points.clear();
    }

    // Antialiasing
    painter->setPen(QPen(corner, 0));
    if (atBeginning || selected) {
      points.append(topLine.p1() + vectorLeft);
    }
    if (!previousSelected) {
      points.append(leftLine.p1() + vectorUp);
    }
    if (atEnd || selected) {
      points.append(topLine.p2() + vectorRight);
      points.append(rightLine.p1() + vectorUp);
    }

    if (selected) {
      points.append(bottomRightConnectToBase + vectorLeft);
      if (!atBeginning) {
        points.append(bottomLeftConnectToBase + vectorRight);

        if (((tab->position == QStyleOptionTab::Beginning) || onlyTab) &&
            leftCornerWidget) {
          // A special case: When the first tab is selected and
          // has a left corner widget, it needs to do more work
          // to connect to the base
          QPoint p1 = bottomLeftConnectToBase + vectorDown;

          points.append(p1);
        }
      }
    }
    if (points.size() > 0) {
      painter->drawPoints(points.constData(), points.size());
      points.clear();
    }

    // Inner border
    QLine innerTopLine =
        QLine(topLine.p1() + vectorDown, topLine.p2() + vectorDown);
    if (!selected) {
      QLinearGradient topLineGradient(innerTopLine.p1(), innerTopLine.p2());
      topLineGradient.setColorAt(0, lightShadowGradientStart);
      topLineGradient.setColorAt(1, lightShadowGradientStop);
      painter->setPen(QPen(mouseOver ? QBrush(darkInnerHighlightBorder)
                                     : QBrush(topLineGradient),
                           1));
    } else {
      painter->setPen(QPen(innerTopLeft, 0));
    }
    painter->drawLine(innerTopLine);

    QLine innerLeftLine = QLine(leftLine.p1() + vectorRight + vectorDown,
                                leftLine.p2() + vectorRight);
    QLine innerRightLine = QLine(rightLine.p1() + vectorLeft + vectorDown,
                                 rightLine.p2() + vectorLeft);

    if (selected) {
      innerRightLine =
          QLine(innerRightLine.p1() + vectorUp, innerRightLine.p2());
      innerLeftLine = QLine(innerLeftLine.p1() + vectorUp, innerLeftLine.p2());
    }

    if (selected || atBeginning) {
      QBrush leftLineGradientBrush;
      QRect rect = QRect(innerLeftLine.p1(), innerLeftLine.p2()).normalized();
      QBrush buttonBrush = PlastikStyleHelper::mapBrushToRectangle(
          option->palette.button(), rect);
      if (buttonBrush.gradient() || !buttonBrush.texture().isNull()) {
        leftLineGradientBrush =
            PlastikStyleHelper::lightenBrush(buttonBrush, 105);
      } else {
        QLinearGradient buttonGradient3(rect.topLeft(), rect.bottomLeft());
        buttonGradient3.setColorAt(0.0, buttonBrush.color().lighter(105));
        buttonGradient3.setColorAt(1.0, buttonBrush.color());
        leftLineGradientBrush = QBrush(buttonGradient3);
      }

      if (!selected)
        painter->setPen(QPen(leftLineGradientBrush, 0));

      // Assume the sun is on the same side in Right-To-Left layouts and
      // draw the light shadow on the left side always (the right line is
      // on the left side in reverse layouts for north and south)
      if (reverseShadow)
        painter->drawLine(innerRightLine);
      else
        painter->drawLine(innerLeftLine);
    }

    if (atEnd || selected) {
      if (!selected) {
        QBrush rightLineGradientBrush;
        QRect rect =
            QRect(innerRightLine.p1(), innerRightLine.p2()).normalized();
        QBrush buttonBrush = PlastikStyleHelper::mapBrushToRectangle(
            option->palette.button(), rect);
        if (buttonBrush.gradient() || !buttonBrush.texture().isNull()) {
          rightLineGradientBrush =
              PlastikStyleHelper::darkenBrush(buttonBrush, 105);
        } else {
          QLinearGradient buttonGradient4(rect.topLeft(), rect.bottomLeft());
          buttonGradient4.setColorAt(0.0, buttonBrush.color());
          buttonGradient4.setColorAt(1.0, buttonBrush.color().darker(110));
          rightLineGradientBrush = QBrush(buttonGradient4);
        }

        painter->setPen(QPen(rightLineGradientBrush, 0));
      } else {
        painter->setPen(QPen(innerBottomRight, 0));
      }

      if (reverseShadow) {
        painter->drawLine(innerLeftLine);
      } else {
        painter->drawLine(innerRightLine);
      }
    }

    // Base
    QLine baseLine =
        QLine(bottomLeft + marginLeft * 2 * vectorRight, bottomRight);
    {

      QPoint adjustedLeft;
      QPoint adjustedRight;

      if (atEnd && !selected) {
        baseLine = QLine(baseLine.p1(), baseLine.p2() + vectorRight);
      }

      if (nextSelected) {
        adjustedRight += vectorLeft;
        baseLine = QLine(baseLine.p1(), baseLine.p2() + vectorLeft);
      }
      if (previousSelected) {
        adjustedLeft += vectorRight;
        baseLine = QLine(baseLine.p1() + vectorRight, baseLine.p2());
      }
      if (atBeginning) {
        adjustedLeft += vectorRight;
      }

      painter->setPen(QPen(baseColor2, 0));
      if (!selected) {
        painter->drawLine(baseLine);
      }

      if (atEnd && !selected) {
        painter->drawPoint(baseLine.p2() + vectorRight);
      }

      if (atBeginning && !selected) {
        adjustedLeft = vectorRight;
      } else {
        adjustedLeft = QPoint(0, 0);
      }
      painter->setPen(QPen(baseColor1, 0));
      if (!selected) {
        painter->drawLine(bottomLeft + vectorUp + adjustedLeft,
                          baseLine.p2() + vectorUp);
      }

      QPoint endPoint = bottomRight + vectorUp;
      if (atEnd && !selected) {
        painter->drawPoint(endPoint);
      }

      // For drawing a lower left "fake" corner on the base when the first
      // tab is unselected
      if (atBeginning && !selected) {
        painter->drawPoint(baseLine.p1() + vectorLeft);
      }

      painter->setPen(QPen(corner, 0));
      if (nextSelected) {
        painter->drawPoint(endPoint);
      } else if (selected) {
        painter->drawPoint(endPoint + vectorRight);
      }

      // For drawing a lower left "fake" corner on the base when the first
      // tab is unselected
      if (atBeginning && !selected) {
        painter->drawPoint(baseLine.p1() + 2 * vectorLeft);
      }
    }
  }
  // Yay we're done
  painter->restore();
}
