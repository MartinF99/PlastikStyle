#include "plastikstyle.h"
#include "plastikstylehelper.h"
#include "qstylehelper.h"

#include <QPainter>

void PlastikStyle::drawMenuItem(const QStyleOption *option, QPainter *painter,
                                QColor alphaCornerColor,
                                const QWidget *widget) const {
  // Draws one item in a popup menu.
  if (const QStyleOptionMenuItem *menuItem =
          qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
    painter->save();
    QBrush textBrush;
    // if (option->palette.resolve() & (1 << QPalette::ButtonText)) // bitwise
    // AND with new QPalette and binary 1 bitshifted by 8 to the left (but no
    // palette to resolve with)
    //     textBrush = option->palette.buttonText();
    // else
    //  what exactly does that condition try to do? it is now nonfunctional,
    //  so windowText is now the default

    textBrush = option->palette.windowText(); // KDE uses windowText rather
    // than buttonText for menus

    if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
      painter->fillRect(menuItem->rect,
                        option->palette.window().color().lighter(103));
      int margin = dpiScaled(5, option);
      int w = 0;
      if (!menuItem->text.isEmpty()) {
        painter->setFont(menuItem->font);
        proxy()->drawItemText(
            painter, menuItem->rect.adjusted(5, 0, -5, 0),
            Qt::AlignLeft | Qt::AlignVCenter, menuItem->palette,
            menuItem->state & State_Enabled, menuItem->text, QPalette::Text);
        w = menuItem->fontMetrics.horizontalAdvance(menuItem->text) + margin;
      }

      painter->setPen(alphaCornerColor);
      bool reverse = menuItem->direction == Qt::RightToLeft;
      painter->drawLine(menuItem->rect.left() + margin + (reverse ? 0 : w),
                        menuItem->rect.center().y(),
                        menuItem->rect.right() - margin - (reverse ? w : 0),
                        menuItem->rect.center().y());

      painter->restore();
      return;
    }

    bool selected = menuItem->state & State_Selected;
    bool checkable = menuItem->checkType != QStyleOptionMenuItem::NotCheckable;
    bool checked = menuItem->checked;

    if (selected) {
      PlastikStyleHelper::drawGradient(
          painter, menuItem->rect,
          option->palette.highlight().color().lighter(105),
          option->palette.highlight().color().darker(110));

      painter->setPen(option->palette.highlight().color().lighter(110));
      painter->drawLine(option->rect.topLeft(), option->rect.topRight());
      painter->setPen(option->palette.highlight().color().darker(115));
      painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
    } else {
      painter->fillRect(option->rect,
                        option->palette.window().color().lighter(103));
    }

    // Check
    QRect checkRect(option->rect.left() + 7, option->rect.center().y() - 6, 13,
                    13);
    checkRect = visualRect(menuItem->direction, menuItem->rect, checkRect);
    if (checkable) {
      if ((menuItem->checkType & QStyleOptionMenuItem::Exclusive) &&
          menuItem->icon.isNull()) {
        QStyleOptionButton button;
        button.rect = checkRect;
        button.state = menuItem->state;
        if (checked)
          button.state |= State_On;
        button.palette = menuItem->palette;
        proxy()->drawPrimitive(PE_IndicatorRadioButton, &button, painter,
                               widget);
      } else {
        if (menuItem->icon.isNull()) {
          QStyleOptionButton button;
          button.rect = checkRect;
          button.state = menuItem->state;
          if (checked)
            button.state |= State_On;
          button.palette = menuItem->palette;
          proxy()->drawPrimitive(PE_IndicatorCheckBox, &button, painter,
                                 widget);
        } else if (checked) {
          qreal iconMinSize = dpiScaled(20, option);
          int iconSize = qMax(menuItem->maxIconWidth, (int)iconMinSize);
          QRect sunkenRect(
              option->rect.left() + menuItemHMargin + itemFrameWidth - 2,
              option->rect.top() + (option->rect.height() - iconSize) / 2 + 1,
              iconSize, iconSize);
          sunkenRect =
              visualRect(menuItem->direction, menuItem->rect, sunkenRect);
          // sunkenRect.moveCenter(vSunkenRect.center());
          QStyleOption opt = *option;
          opt.state |= State_Sunken;
          opt.rect = sunkenRect;
          PlastikStyleHelper::drawShadePanel(painter, &opt, false, widget);
        }
      }
    }

    // Text and icon, ripped from windows style
    bool dis = !(menuItem->state & State_Enabled);
    bool act = menuItem->state & State_Selected;
    const QStyleOption *opt = option;
    const QStyleOptionMenuItem *menuitem = menuItem;
    int checkcol = qMax<int>(menuitem->maxIconWidth, dpiScaled(20, option));
    QPainter *p = painter;
    QRect vCheckRect = visualRect(opt->direction, menuitem->rect,
                                  QRect(menuitem->rect.x() + itemFrameWidth,
                                        menuitem->rect.y(), checkcol,
                                        menuitem->rect.height()));
    if (!menuItem->icon.isNull()) {
      QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;
      if (act && !dis)
        mode = QIcon::Active;
      QPixmap pixmap;
      QSize iconSize(pixelMetric(PM_SmallIconSize, option, widget),
                     pixelMetric(PM_SmallIconSize, option, widget));
      if (checked)
        pixmap = menuItem->icon.pixmap(
            iconSize, painter->device()->devicePixelRatio(), mode, QIcon::On);
      else
        pixmap = menuItem->icon.pixmap(
            iconSize, painter->device()->devicePixelRatio(), mode);
      QRect pmr(QPoint(0, 0), pixmap.deviceIndependentSize().toSize());
      pmr.moveCenter(vCheckRect.center());
      painter->setPen(textBrush.color());
      if (checkable && checked)
        painter->drawPixmap(QPoint(pmr.left() + 1, pmr.top() + 1), pixmap);
      else
        painter->drawPixmap(pmr.topLeft(), pixmap);
    }

    if (selected) {
      painter->setPen(menuItem->palette.highlightedText().color());
    } else {
      painter->setPen(textBrush.color());
    }
    int x, y, w, h;
    menuitem->rect.getRect(&x, &y, &w, &h);
    int tab = menuitem->reservedShortcutWidth;
    QColor discol;
    if (dis) {
      discol = textBrush.color();
      p->setPen(discol);
    }
    int xm = itemFrameWidth + checkcol + itemHMargin;
    int xpos = menuitem->rect.x() + xm;
    QRect textRect(xpos, y + itemVMargin, w - xm - rightBorder - tab + 1,
                   h - 2 * itemVMargin);
    QRect vTextRect = visualRect(opt->direction, menuitem->rect, textRect);
    QString s = menuitem->text;
    if (!s.isEmpty()) { // draw text
      p->save();
      int t = s.indexOf(QLatin1Char('\t'));
      int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic |
                       Qt::TextDontClip | Qt::TextSingleLine;
      if (!styleHint(SH_UnderlineShortcut, menuitem, widget))
        text_flags |= Qt::TextHideMnemonic;
      text_flags |= Qt::AlignLeft;
      if (t >= 0) {
        QRect vShortcutRect =
            visualRect(opt->direction, menuitem->rect,
                       QRect(textRect.topRight(), QPoint(menuitem->rect.right(),
                                                         textRect.bottom())));
        if (dis && !act && styleHint(SH_EtchDisabledText, option, widget)) {
          p->setPen(menuitem->palette.light().color());
          p->drawText(vShortcutRect.adjusted(1, 1, 1, 1), text_flags,
                      s.mid(t + 1));
          p->setPen(discol);
        }
        p->drawText(vShortcutRect, text_flags, s.mid(t + 1));
        s = s.left(t);
      }
      QFont font = menuitem->font;
      if (menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem)
        font.setBold(true);
      p->setFont(font);
      if (dis && !act && styleHint(SH_EtchDisabledText, option, widget)) {
        p->setPen(menuitem->palette.light().color());
        p->drawText(vTextRect.adjusted(1, 1, 1, 1), text_flags, s.left(t));
        p->setPen(discol);
      }
      p->drawText(vTextRect, text_flags, s.left(t));
      p->restore();
    }

    // Arrow
    if (menuItem->menuItemType ==
        QStyleOptionMenuItem::SubMenu) { // draw sub menu arrow
      int dim = (menuItem->rect.height() - 4) / 2;
      PrimitiveElement arrow;
      arrow = (opt->direction == Qt::RightToLeft) ? PE_IndicatorArrowLeft
                                                  : PE_IndicatorArrowRight;
      int xpos = menuItem->rect.left() + menuItem->rect.width() - 6 - 2 - dim;
      QRect vSubMenuRect = visualRect(
          option->direction, menuItem->rect,
          QRect(xpos,
                menuItem->rect.top() + menuItem->rect.height() / 2 - dim / 2,
                dim, dim));
      QStyleOptionMenuItem newMI = *menuItem;
      newMI.rect = vSubMenuRect;
      newMI.state = option->state & State_Enabled;
      if (selected)
        newMI.palette.setColor(QPalette::ButtonText,
                               newMI.palette.highlightedText().color());
      else
        newMI.palette.setColor(QPalette::ButtonText, textBrush.color());
      proxy()->drawPrimitive(arrow, &newMI, painter, widget);
    }

    painter->restore();
  }
}
