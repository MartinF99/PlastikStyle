/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "plastikstyle.h"
#define QPlastique_MaskButtons
static const int blueFrameWidth = 2; // with of line edit focus frame

#include "plastikstylehelper.h"
#include "qstylecache.h"
#include "qstylehelper.h"
#include <QAbstractItemView>
#include <QApplication>
#include <QBitmap>
#include <QCheckBox>
#include <QComboBox>
#include <QCommonStyle>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QImage>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMutableListIterator>
#include <QPaintEngine>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPen>
#include <QPixmap>
#include <QPixmapCache>
#include <QWizard>
#include <limits.h>
#include <qdrawutil.h>
#include <qelapsedtimer.h>
#include <qprocess.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qscrollbar.h>
#include <qspinbox.h>
#include <qsplitter.h>
#include <qstylefactory.h>
#include <qstyleoption.h>
#include <qtextedit.h>
#include <qtoolbar.h>
#include <qtoolbox.h>
#include <qtoolbutton.h>
#include <qvarlengtharray.h>

/*!
    \class PlastikStyle
    \brief The PlastikStyle class provides a widget style similar to the
    Plastik style available in KDE.

    The Plastique style provides a default look and feel for widgets on X11
    that closely resembles the Plastik style, introduced by Sandro Giessl in
    KDE 3.2.

    \image qplastiquestyle.png
    \sa QWindowsXPStyle, QMacStyle, QWindowsStyle
*/

/*!
    Constructs a PlastikStyle object.
*/
PlastikStyle::PlastikStyle() : QCommonStyle() {
  setObjectName(QLatin1String("Plastik_new"));
}

/*!
    Destructs the PlastikStyle object.
*/
PlastikStyle::~PlastikStyle() {}

/*!
  \reimp
*/
void PlastikStyle::drawPrimitive(PrimitiveElement element,
                                 const QStyleOption *option, QPainter *painter,
                                 const QWidget *widget) const {
  Q_ASSERT(option);

  switch (element) {
  case PE_FrameButtonTool:
    drawToolButtonFrame(element, option, painter, widget);
    break;
  case PE_IndicatorButtonDropDown:
    proxy()->drawPrimitive(PE_PanelButtonTool, option, painter, widget);
    break;
  case PE_FrameDefaultButton: {
    drawDefaultButtonFrame(option, painter);
    break;
  }
  case PE_FrameButtonBevel:
    drawButtonBevel(option, painter, false);
    break;
  case PE_PanelButtonBevel:
    drawButtonBevel(option, painter, true);
    break;
  case PE_FrameTabWidget:
    drawTabWidgetFrame(element, option, painter, widget);
    break;
  case PE_FrameTabBarBase:
    drawTabBarBaseFrame(element, option, painter, widget);
    break;
  case PE_FrameGroupBox:
    drawLineEditFrame(option, painter, widget);
    break;
  case PE_PanelMenuBar:
    drawMenuBarPanel(element, option, painter, widget);
    break;
  case PE_IndicatorProgressChunk:
    drawIndicatorProgressChunk(option, painter, widget);
    break;
  case PE_IndicatorHeaderArrow:
    drawHeaderArrowIndicator(element, option, painter, widget);
    break;
  case PE_PanelButtonTool:
    drawToolButtonPanel(option, painter, widget);
    break;
  case PE_IndicatorToolBarHandle:
    drawToolBarHandleIndicator(option, painter, widget);
    break;
  case PE_IndicatorToolBarSeparator: {
    drawToolBarSeperatorIndicator(option, painter, widget);
    break;
  }
  case PE_PanelButtonCommand:
    drawCommandButtonPanel(option, painter);
    break;
  case PE_IndicatorCheckBox:
    drawCheckBoxIndicator(option, painter);
    break;
  case PE_IndicatorRadioButton:
    drawRadioButtonIndicator(option, painter);
    break;
  case PE_IndicatorDockWidgetResizeHandle:
    drawDockWidgetResizeHandleIndicator(option, painter, widget);
    break;
  case PE_IndicatorItemViewItemCheck:
    drawItemViewItemCheck(option, painter, widget);
    break;
  case PE_FrameWindow:
    drawWindowFrame(option, painter, widget);
    break;
  case PE_FrameFocusRect:
    drawFocusRectFrame(option, painter, widget);
    break;
  case PE_IndicatorBranch:
    drawBranchIndicator(option, painter, widget);
    break;
  default:
    QCommonStyle::drawPrimitive(element, option, painter, widget);
    break;
  }
}

/*!
  \reimp
*/
void PlastikStyle::drawControl(ControlElement element,
                               const QStyleOption *option, QPainter *painter,
                               const QWidget *widget) const {
  QColor borderColor = option->palette.window().color().darker(178);
  QColor alphaCornerColor = calcAlphaCornerColor(option, widget);

  QColor gradientStartColor = option->palette.button().color().lighter(104);
  QColor gradientStopColor = option->palette.button().color().darker(105);

  QColor highlightedGradientStartColor =
      option->palette.button().color().lighter(101);
  QColor highlightedGradientStopColor =
      PlastikStyleHelper::mergeColors(option->palette.button().color(),
                                      option->palette.highlight().color(), 85);

  QColor lightShadowGradientStartColor =
      highlightedGradientStartColor.lighter(105);
  QColor lightShadowGradientStopColor =
      highlightedGradientStopColor.lighter(105);

  QColor highlightedDarkInnerBorderColor =
      PlastikStyleHelper::mergeColors(option->palette.button().color(),
                                      option->palette.highlight().color(), 35);
  QColor highlightedLightInnerBorderColor =
      PlastikStyleHelper::mergeColors(option->palette.button().color(),
                                      option->palette.highlight().color(), 58);

  QColor alphaInnerColor = PlastikStyleHelper::mergeColors(
      highlightedDarkInnerBorderColor, option->palette.base().color());

  switch (element) {
  case CE_RubberBand:
    drawRubberBand(option, painter, widget);
    break;
  case CE_TabBarTabShape:
    drawTabBarTabShape(element, option, painter, highlightedLightInnerBorderColor, highlightedDarkInnerBorderColor, lightShadowGradientStartColor, lightShadowGradientStopColor, widget);
    break;
#ifndef QT_NO_PROGRESSBAR
  case CE_ProgressBarGroove:
    drawProgressBarGroove(option, painter, borderColor, gradientStartColor);
    break;
  case CE_ProgressBarLabel:
    drawProgressBarLabel(option, painter);
    break;
  case CE_ProgressBarContents:
    drawProgressBarContents(option, painter, highlightedLightInnerBorderColor, alphaInnerColor);
    break;
#endif // QT_NO_PROGRESSBAR
  case CE_HeaderSection:
    drawHeaderSection(option, painter, gradientStartColor, gradientStopColor, borderColor, alphaCornerColor);
    break;
#ifndef QT_NO_MENU
  case CE_MenuItem:
    drawMenuItem(option, painter, alphaCornerColor, widget);
    break;
#endif // QT_NO_MENU
#ifndef QT_NO_MENUBAR
  case CE_MenuBarItem:
    drawMenuBarItem(element, option, painter, alphaCornerColor, borderColor, gradientStartColor, gradientStopColor, widget);
    break;

#ifndef QT_NO_MAINWINDOW
  case CE_MenuBarEmptyArea:
    drawMenuBarEmptyArea(option, painter, widget);
    break;
#endif // QT_NO_MAINWINDOW

#endif // QT_NO_MENUBAR

#ifndef QT_NO_TOOLBOX
  case CE_ToolBoxTabShape:
    if (const QStyleOptionToolBox *toolBox =
            qstyleoption_cast<const QStyleOptionToolBox *>(option)) {
      painter->save();

      int width = toolBox->rect.width();
      int diag = toolBox->rect.height() - 2;

      // The essential points
      QPoint rightMost;
      QPoint rightEdge;
      QPoint leftEdge;
      QPoint leftMost;
      QPoint leftOne;
      QPoint rightOne;
      QPoint upOne(0, -1); // unused in code, but stays
      QPoint downOne(0, 1);

      if (toolBox->direction != Qt::RightToLeft) {
        rightMost = QPoint(toolBox->rect.right(), toolBox->rect.bottom() - 2);
        rightEdge = QPoint(toolBox->rect.right() - width / 10,
                           toolBox->rect.bottom() - 2);
        leftEdge = QPoint(toolBox->rect.right() - width / 10 - diag,
                          toolBox->rect.top());
        leftMost = QPoint(toolBox->rect.left(), toolBox->rect.top());
        leftOne = QPoint(-1, 0);
        rightOne = QPoint(1, 0);
      } else {
        rightMost = QPoint(toolBox->rect.left(), toolBox->rect.bottom() - 2);
        rightEdge = QPoint(toolBox->rect.left() + width / 10,
                           toolBox->rect.bottom() - 2);
        leftEdge = QPoint(toolBox->rect.left() + width / 10 + diag,
                          toolBox->rect.top());
        leftMost = QPoint(toolBox->rect.right(), toolBox->rect.top());
        leftOne = QPoint(1, 0);
        rightOne = QPoint(-1, 0);
      }

      QLine lines[3];

      // Draw the outline
      painter->setPen(borderColor);
      lines[0] = QLine(rightMost, rightEdge);
      lines[1] = QLine(rightEdge + leftOne, leftEdge);
      lines[2] = QLine(leftEdge + leftOne, leftMost);
      painter->drawLines(lines, 3);
      painter->setPen(toolBox->palette.base().color());
      lines[0] = QLine(rightMost + downOne, rightEdge + downOne);
      lines[1] = QLine(rightEdge + leftOne + downOne, leftEdge + downOne);
      lines[2] = QLine(leftEdge + leftOne + downOne, leftMost + downOne);
      painter->drawLines(lines, 3);

      painter->restore();
    }
    break;
#endif // QT_NO_TOOLBOX
#ifndef QT_NO_SPLITTER
  case CE_Splitter:
    if ((option->state & State_Enabled) && (option->state & State_MouseOver))
      painter->fillRect(option->rect, QColor(255, 255, 255, 128));
    if (option->state & State_Horizontal) {
      int height = option->rect.height() / 3;
      QRect rect(option->rect.left() + (option->rect.width() / 2 - 1),
                 option->rect.center().y() - height / 2, 3, height);
      PlastikStyleHelper::drawHandle(painter, option, rect, Qt::Horizontal,
                                     widget);
    } else {
      int width = option->rect.width() / 3;
      QRect rect(option->rect.center().x() - width / 2,
                 option->rect.top() + (option->rect.height() / 2) - 1, width,
                 3);
      PlastikStyleHelper::drawHandle(painter, option, rect, Qt::Vertical,
                                     widget);
    }
    break;
#endif // QT_NO_SPLITTER
#ifndef QT_NO_DOCKWIDGET
  case CE_DockWidgetTitle:
    if (const QStyleOptionDockWidget *dockWidget =
            qstyleoption_cast<const QStyleOptionDockWidget *>(option)) {
      painter->save();

      bool verticalTitleBar = dockWidget->verticalTitleBar;

      // Find text width and title rect
      int textWidth = option->fontMetrics.horizontalAdvance(dockWidget->title);
      int margin = 4;
      QRect titleRect =
          subElementRect(SE_DockWidgetTitleBarText, option, widget);
      QRect rect = dockWidget->rect;

      if (verticalTitleBar) {
        QRect r = rect;
        QSize s = r.size();
        s.transpose();
        r.setSize(s);

        titleRect = QRect(r.left() + rect.bottom() - titleRect.bottom(),
                          r.top() + titleRect.left() - rect.left(),
                          titleRect.height(), titleRect.width());

        painter->translate(r.left(), r.top() + r.width());
        painter->rotate(-90);
        painter->translate(-r.left(), -r.top());

        rect = r;
      }

      // Chop and insert ellide into title if text is too wide
      QString title = PlastikStyleHelper::elliditide(
          dockWidget->title, dockWidget->fontMetrics, titleRect, &textWidth);

      // Draw the toolbar handle pattern to the left and right of the text
      QImage handle(PlastikStyleHelper::toolBarHandle);
      alphaCornerColor.setAlpha(170);
      handle.setColor(1, alphaCornerColor.rgba());
      handle.setColor(2, PlastikStyleHelper::mergeColors(
                             alphaCornerColor, option->palette.light().color())
                             .rgba());
      handle.setColor(3, option->palette.light().color().rgba());

      if (title.isEmpty()) {
        // Joint handle if there's no title
        QRect r;
        r.setRect(titleRect.left(), titleRect.top(), titleRect.width(),
                  titleRect.bottom());
        int nchunks = (r.width() / handle.width()) - 1;
        int indent = (r.width() - (nchunks * handle.width())) / 2;
        for (int i = 0; i < nchunks; ++i) {
          painter->drawImage(QPoint(r.left() + indent + i * handle.width(),
                                    r.center().y() - handle.height() / 2),
                             handle);
        }
      } else {
        // Handle pattern to the left of the title
        QRect leftSide(titleRect.left(), titleRect.top(),
                       titleRect.width() / 2 - textWidth / 2 - margin,
                       titleRect.bottom());
        int nchunks = leftSide.width() / handle.width();
        int indent = (leftSide.width() - (nchunks * handle.width())) / 2;
        for (int i = 0; i < nchunks; ++i) {
          painter->drawImage(
              QPoint(leftSide.left() + indent + i * handle.width(),
                     leftSide.center().y() - handle.height() / 2),
              handle);
        }

        // Handle pattern to the right of the title
        QRect rightSide = titleRect.adjusted(
            titleRect.width() / 2 + textWidth / 2 + margin, 0, 0, 0);
        nchunks = rightSide.width() / handle.width();
        indent = (rightSide.width() - (nchunks * handle.width())) / 2;
        for (int j = 0; j < nchunks; ++j) {
          painter->drawImage(
              QPoint(rightSide.left() + indent + j * handle.width(),
                     rightSide.center().y() - handle.height() / 2),
              handle);
        }
      }

      // Draw the text centered
      QFont font = painter->font();
      font.setPointSize(QFontInfo(font).pointSize() - 1);
      painter->setFont(font);
      painter->setPen(dockWidget->palette.windowText().color());
      painter->drawText(
          titleRect,
          int(Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextShowMnemonic),
          title);

      painter->restore();
    }

    break;
#endif // QT_NO_DOCKWIDGET
#ifndef QT_NO_TOOLBAR
  case CE_ToolBar:
    if (const QStyleOptionToolBar *toolBar =
            qstyleoption_cast<const QStyleOptionToolBar *>(option)) {
      // Draws the light line above and the dark line below menu bars and
      // tool bars.
      QPen oldPen = painter->pen();
      if (toolBar->toolBarArea == Qt::TopToolBarArea) {
        if (toolBar->positionOfLine == QStyleOptionToolBar::End ||
            toolBar->positionOfLine == QStyleOptionToolBar::OnlyOne) {
          // The end and onlyone top toolbar lines draw a double
          // line at the bottom to blend with the central
          // widget.
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.bottomLeft(),
                            option->rect.bottomRight());
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.left(), option->rect.bottom() - 1,
                            option->rect.right(), option->rect.bottom() - 1);
        } else {
          // All others draw a single dark line at the bottom.
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.bottomLeft(),
                            option->rect.bottomRight());
        }
        // All top toolbar lines draw a light line at the top.
        painter->setPen(option->palette.window().color().lighter(104));
        painter->drawLine(option->rect.topLeft(), option->rect.topRight());
      } else if (toolBar->toolBarArea == Qt::BottomToolBarArea) {
        if (toolBar->positionOfLine == QStyleOptionToolBar::End ||
            toolBar->positionOfLine == QStyleOptionToolBar::Middle) {
          // The end and middle bottom tool bar lines draw a dark
          // line at the bottom.
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.bottomLeft(),
                            option->rect.bottomRight());
        }
        if (toolBar->positionOfLine == QStyleOptionToolBar::Beginning ||
            toolBar->positionOfLine == QStyleOptionToolBar::OnlyOne) {
          // The beginning and only one tool bar lines draw a
          // double line at the bottom to blend with the
          // status bar.
          // ### The styleoption could contain whether the
          // main window has a menu bar and a status bar, and
          // possibly dock widgets.
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.left(), option->rect.bottom() - 1,
                            option->rect.right(), option->rect.bottom() - 1);
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.bottomLeft(),
                            option->rect.bottomRight());
        }
        if (toolBar->positionOfLine == QStyleOptionToolBar::End) {
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.topLeft(), option->rect.topRight());
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.left(), option->rect.top() + 1,
                            option->rect.right(), option->rect.top() + 1);

        } else {
          // All other bottom toolbars draw a light line at the top.
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.topLeft(), option->rect.topRight());
        }
      }
      if (toolBar->toolBarArea == Qt::LeftToolBarArea) {
        if (toolBar->positionOfLine == QStyleOptionToolBar::Middle ||
            toolBar->positionOfLine == QStyleOptionToolBar::End) {
          // The middle and left end toolbar lines draw a light
          // line to the left.
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.topLeft(), option->rect.bottomLeft());
        }
        if (toolBar->positionOfLine == QStyleOptionToolBar::End) {
          // All other left toolbar lines draw a dark line to the right
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.right() - 1, option->rect.top(),
                            option->rect.right() - 1, option->rect.bottom());
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.topRight(),
                            option->rect.bottomRight());
        } else {
          // All other left toolbar lines draw a dark line to the right
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.topRight(),
                            option->rect.bottomRight());
        }
      } else if (toolBar->toolBarArea == Qt::RightToolBarArea) {
        if (toolBar->positionOfLine == QStyleOptionToolBar::Middle ||
            toolBar->positionOfLine == QStyleOptionToolBar::End) {
          // Right middle and end toolbar lines draw the dark right line
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.topRight(),
                            option->rect.bottomRight());
        }
        if (toolBar->positionOfLine == QStyleOptionToolBar::End ||
            toolBar->positionOfLine == QStyleOptionToolBar::OnlyOne) {
          // The right end and single toolbar draws the dark
          // line on its left edge
          painter->setPen(alphaCornerColor);
          painter->drawLine(option->rect.topLeft(), option->rect.bottomLeft());
          // And a light line next to it
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.left() + 1, option->rect.top(),
                            option->rect.left() + 1, option->rect.bottom());
        } else {
          // Other right toolbars draw a light line on its left edge
          painter->setPen(option->palette.window().color().lighter(104));
          painter->drawLine(option->rect.topLeft(), option->rect.bottomLeft());
        }
      }
      painter->setPen(oldPen);
    }
    break;
#endif // QT_NO_TOOLBAR
#ifndef QT_NO_SCROLLBAR
  case CE_ScrollBarAddLine:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {

      bool horizontal = scrollBar->orientation == Qt::Horizontal;
      bool reverse = scrollBar->direction == Qt::RightToLeft;
      bool sunken = scrollBar->state & State_Sunken;

      QString addLinePixmapName = QStyleHelper::uniqueName(
          QLatin1String("scrollbar_addline"), option, option->rect.size());
      QPixmap cache;
      if (!QPixmapCache::find(addLinePixmapName, &cache)) {
        cache = QPixmap(option->rect.size());
        cache.fill(Qt::white);
        QRect pixmapRect(0, 0, cache.width(), cache.height());
        QPainter addLinePainter(&cache);
        addLinePainter.fillRect(pixmapRect, option->palette.window());

        if (option->state & State_Enabled) {
          // Gradient
          QLinearGradient gradient(
              pixmapRect.center().x(), pixmapRect.top() + 2,
              pixmapRect.center().x(), pixmapRect.bottom() - 2);
          if ((scrollBar->activeSubControls & SC_ScrollBarAddLine) && sunken) {
            gradient.setColorAt(0, gradientStopColor);
            gradient.setColorAt(1, gradientStopColor);
          } else {
            gradient.setColorAt(0, gradientStartColor.lighter(105));
            gradient.setColorAt(1, gradientStopColor);
          }
          addLinePainter.fillRect(pixmapRect.left() + 2, pixmapRect.top() + 2,
                                  pixmapRect.right() - 3,
                                  pixmapRect.bottom() - 3, gradient);
        }

        // Details
        QImage addButton;
        if (horizontal) {
          addButton =
              QImage(reverse ? PlastikStyleHelper::scrollBarButtonLeft
                             : PlastikStyleHelper::scrollBarButtonRight);
        } else {
          addButton = QImage(PlastikStyleHelper::scrollBarButtonDown);
        }
        addButton.setColor(1, alphaCornerColor.rgba());
        addButton.setColor(2, borderColor.rgba());
        if ((scrollBar->activeSubControls & SC_ScrollBarAddLine) && sunken) {
          addButton.setColor(3, gradientStopColor.rgba());
          addButton.setColor(4, gradientStopColor.rgba());
        } else {
          addButton.setColor(3, gradientStartColor.lighter(105).rgba());
          addButton.setColor(4, gradientStopColor.rgba());
        }
        addButton.setColor(5, scrollBar->palette.text().color().rgba());
        addLinePainter.drawImage(pixmapRect, addButton);

        // Arrow
        if (horizontal) {
          QImage arrow(reverse ? PlastikStyleHelper::scrollBarButtonArrowLeft
                               : PlastikStyleHelper::scrollBarButtonArrowLeft);
          arrow.setColor(1, scrollBar->palette.windowText().color().rgba());

          if ((scrollBar->activeSubControls & SC_ScrollBarAddLine) && sunken)
            addLinePainter.translate(1, 1);
          addLinePainter.drawImage(
              QPoint(pixmapRect.center().x() - 2, pixmapRect.center().y() - 3),
              arrow);
        } else {
          QImage arrow(PlastikStyleHelper::scrollBarButtonArrowDown);
          arrow.setColor(1, scrollBar->palette.windowText().color().rgba());

          if ((scrollBar->activeSubControls & SC_ScrollBarAddLine) && sunken)
            addLinePainter.translate(1, 1);
          addLinePainter.drawImage(
              QPoint(pixmapRect.center().x() - 3, pixmapRect.center().y() - 2),
              arrow);
        }
        addLinePainter.end();
        QPixmapCache::insert(addLinePixmapName, cache);
      }
      painter->drawPixmap(option->rect.topLeft(), cache);
    }
    break;
  case CE_ScrollBarSubPage:
  case CE_ScrollBarAddPage:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      bool sunken = scrollBar->state & State_Sunken;
      bool horizontal = scrollBar->orientation == Qt::Horizontal;

      QString groovePixmapName = QStyleHelper::uniqueName(
          QLatin1String("scrollbar_groove"), option, option->rect.size());
      if (sunken)
        groovePixmapName += QLatin1String("-sunken");
      if (element == CE_ScrollBarAddPage)
        groovePixmapName += QLatin1String("-addpage");

      QPixmap cache;
      if (!QPixmapCache::find(groovePixmapName, &cache)) {
        cache = QPixmap(option->rect.size());
        cache.fill(option->palette.window().color());
        QPainter groovePainter(&cache);
        QRect pixmapRect =
            QRect(0, 0, option->rect.width(), option->rect.height());
        QColor color =
            scrollBar->palette.base().color().darker(sunken ? 125 : 100);
        groovePainter.setBrushOrigin(
            (element == CE_ScrollBarAddPage) ? pixmapRect.width() : 0,
            (element == CE_ScrollBarAddPage) ? pixmapRect.height() : 0);
        groovePainter.fillRect(pixmapRect, QBrush(color, Qt::Dense4Pattern));

        QColor edgeColor = scrollBar->palette.base().color().darker(125);
        if (horizontal) {
          groovePainter.setBrushOrigin(
              (element == CE_ScrollBarAddPage) ? pixmapRect.width() : 1, 0);
          groovePainter.fillRect(
              QRect(pixmapRect.topLeft(), QSize(pixmapRect.width(), 1)),
              QBrush(edgeColor, Qt::Dense4Pattern));
          groovePainter.fillRect(
              QRect(pixmapRect.bottomLeft(), QSize(pixmapRect.width(), 1)),
              QBrush(edgeColor, Qt::Dense4Pattern));
        } else {
          groovePainter.setBrushOrigin(
              0, (element == CE_ScrollBarAddPage) ? pixmapRect.height() : 1);
          groovePainter.fillRect(
              QRect(pixmapRect.topLeft(), QSize(1, pixmapRect.height())),
              QBrush(edgeColor, Qt::Dense4Pattern));
          groovePainter.fillRect(
              QRect(pixmapRect.topRight(), QSize(1, pixmapRect.height())),
              QBrush(edgeColor, Qt::Dense4Pattern));
        }

        groovePainter.end();
        QPixmapCache::insert(groovePixmapName, cache);
      }
      painter->drawPixmap(option->rect.topLeft(), cache);
    }
    break;
  case CE_ScrollBarSubLine:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      QRect scrollBarSubLine = scrollBar->rect;

      bool horizontal = scrollBar->orientation == Qt::Horizontal;
      bool isEnabled = scrollBar->state & State_Enabled;
      bool reverse = scrollBar->direction == Qt::RightToLeft;
      bool sunken = scrollBar->state & State_Sunken;

      // The SubLine (up/left) buttons
      QRect button1;
      QRect button2;
      int scrollBarExtent =
          proxy()->pixelMetric(PM_ScrollBarExtent, option, widget);
      if (horizontal) {
        button1.setRect(scrollBarSubLine.left(), scrollBarSubLine.top(),
                        scrollBarExtent, scrollBarSubLine.height());
        button2.setRect(scrollBarSubLine.right() - (scrollBarExtent - 1),
                        scrollBarSubLine.top(), scrollBarExtent,
                        scrollBarSubLine.height());
      } else {
        button1.setRect(scrollBarSubLine.left(), scrollBarSubLine.top(),
                        scrollBarSubLine.width(), scrollBarExtent);
        button2.setRect(scrollBarSubLine.left(),
                        scrollBarSubLine.bottom() - (scrollBarExtent - 1),
                        scrollBarSubLine.width(), scrollBarExtent);
      }

      QString subLinePixmapName = QStyleHelper::uniqueName(
          QLatin1String("scrollbar_subline"), option, button1.size());
      QPixmap cache;
      if (!QPixmapCache::find(subLinePixmapName, &cache)) {
        cache = QPixmap(button1.size());
        cache.fill(Qt::white);
        QRect pixmapRect(0, 0, cache.width(), cache.height());
        QPainter subLinePainter(&cache);
        subLinePainter.fillRect(pixmapRect, option->palette.window());

        if (isEnabled) {
          // Gradients
          if ((scrollBar->activeSubControls & SC_ScrollBarSubLine) && sunken) {
            PlastikStyleHelper::drawGradient(
                &subLinePainter,
                QRect(pixmapRect.left() + 2, pixmapRect.top() + 2,
                      pixmapRect.right() - 3, pixmapRect.bottom() - 3),
                gradientStopColor, gradientStopColor);
          } else {
            PlastikStyleHelper::drawGradient(
                &subLinePainter,
                QRect(pixmapRect.left() + 2, pixmapRect.top() + 2,
                      pixmapRect.right() - 3, pixmapRect.bottom() - 3),
                gradientStartColor.lighter(105), gradientStopColor);
          }
        }

        // Details
        QImage subButton;
        if (horizontal) {
          subButton = QImage(reverse ? PlastikStyleHelper::scrollBarButtonRight
                                     : PlastikStyleHelper::scrollBarButtonLeft);
        } else {
          subButton = QImage(PlastikStyleHelper::scrollBarButtonUp);
        }
        subButton.setColor(1, alphaCornerColor.rgba());
        subButton.setColor(2, borderColor.rgba());
        if ((scrollBar->activeSubControls & SC_ScrollBarSubLine) && sunken) {
          subButton.setColor(3, gradientStopColor.rgba());
          subButton.setColor(4, gradientStopColor.rgba());
        } else {
          subButton.setColor(3, gradientStartColor.lighter(105).rgba());
          subButton.setColor(4, gradientStopColor.rgba());
        }
        subButton.setColor(5, scrollBar->palette.text().color().rgba());
        subLinePainter.drawImage(pixmapRect, subButton);

        // Arrows
        if (horizontal) {
          QImage arrow(reverse ? PlastikStyleHelper::scrollBarButtonArrowRight
                               : PlastikStyleHelper::scrollBarButtonArrowLeft);
          arrow.setColor(1, scrollBar->palette.windowText().color().rgba());

          if ((scrollBar->activeSubControls & SC_ScrollBarSubLine) && sunken)
            subLinePainter.translate(1, 1);
          subLinePainter.drawImage(
              QPoint(pixmapRect.center().x() - 2, pixmapRect.center().y() - 3),
              arrow);
        } else {
          QImage arrow(PlastikStyleHelper::scrollBarButtonArrowUp);
          arrow.setColor(1, scrollBar->palette.windowText().color().rgba());

          if ((scrollBar->activeSubControls & SC_ScrollBarSubLine) && sunken)
            subLinePainter.translate(1, 1);
          subLinePainter.drawImage(
              QPoint(pixmapRect.center().x() - 3, pixmapRect.center().y() - 2),
              arrow);
        }
        subLinePainter.end();
        QPixmapCache::insert(subLinePixmapName, cache);
      }
      painter->drawPixmap(button1.topLeft(), cache);
      painter->drawPixmap(button2.topLeft(), cache);
    }
    break;
  case CE_ScrollBarSlider:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      bool horizontal = scrollBar->orientation == Qt::Horizontal;
      bool isEnabled = scrollBar->state & State_Enabled;

      // The slider
      if (option->rect.isValid()) {
        QString sliderPixmapName = QStyleHelper::uniqueName(
            QLatin1String("scrollbar_slider"), option, option->rect.size());
        if (horizontal)
          sliderPixmapName += QLatin1String("-horizontal");

        QPixmap cache;
        if (!QPixmapCache::find(sliderPixmapName, &cache)) {
          cache = QPixmap(option->rect.size());
          cache.fill(Qt::white);
          QRect pixmapRect(0, 0, cache.width(), cache.height());
          QPainter sliderPainter(&cache);
          bool sunken = (scrollBar->state & State_Sunken);

          if (isEnabled) {
            QLinearGradient gradient(pixmapRect.left(), pixmapRect.center().y(),
                                     pixmapRect.right(),
                                     pixmapRect.center().y());
            if (horizontal)
              gradient =
                  QLinearGradient(pixmapRect.center().x(), pixmapRect.top(),
                                  pixmapRect.center().x(), pixmapRect.bottom());

            if (sunken) {
              gradient.setColorAt(0, gradientStartColor.lighter(110));
              gradient.setColorAt(1, gradientStopColor.lighter(105));
            } else {
              gradient.setColorAt(0, gradientStartColor.lighter(105));
              gradient.setColorAt(1, gradientStopColor);
            }
            sliderPainter.fillRect(pixmapRect.adjusted(2, 2, -2, -2), gradient);
          } else {
            sliderPainter.fillRect(pixmapRect.adjusted(2, 2, -2, -2),
                                   option->palette.window());
          }

          sliderPainter.setPen(borderColor);
          sliderPainter.drawRect(pixmapRect.adjusted(0, 0, -1, -1));
          sliderPainter.setPen(alphaCornerColor);
          QPoint points[4] = {QPoint(pixmapRect.left(), pixmapRect.top()),
                              QPoint(pixmapRect.left(), pixmapRect.bottom()),
                              QPoint(pixmapRect.right(), pixmapRect.top()),
                              QPoint(pixmapRect.right(), pixmapRect.bottom())};
          sliderPainter.drawPoints(points, 4);

          QLine lines[2];
          sliderPainter.setPen(sunken ? gradientStartColor.lighter(110)
                                      : gradientStartColor.lighter(105));
          lines[0] = QLine(pixmapRect.left() + 1, pixmapRect.top() + 1,
                           pixmapRect.right() - 1, pixmapRect.top() + 1);
          lines[1] = QLine(pixmapRect.left() + 1, pixmapRect.top() + 2,
                           pixmapRect.left() + 1, pixmapRect.bottom() - 2);
          sliderPainter.drawLines(lines, 2);

          sliderPainter.setPen(sunken ? gradientStopColor.lighter(105)
                                      : gradientStopColor);
          lines[0] = QLine(pixmapRect.left() + 1, pixmapRect.bottom() - 1,
                           pixmapRect.right() - 1, pixmapRect.bottom() - 1);
          lines[1] = QLine(pixmapRect.right() - 1, pixmapRect.top() + 2,
                           pixmapRect.right() - 1, pixmapRect.bottom() - 1);
          sliderPainter.drawLines(lines, 2);

          int sliderMinLength =
              proxy()->pixelMetric(PM_ScrollBarSliderMin, scrollBar, widget);
          if ((horizontal && scrollBar->rect.width() > sliderMinLength) ||
              (!horizontal && scrollBar->rect.height() > sliderMinLength)) {
            QImage pattern(horizontal
                               ? PlastikStyleHelper::sliderPatternHorizontal
                               : PlastikStyleHelper::sliderPatternVertical);
            pattern.setColor(1, alphaCornerColor.rgba());
            pattern.setColor(2, (sunken ? gradientStartColor.lighter(110)
                                        : gradientStartColor.lighter(105))
                                    .rgba());

            if (horizontal) {
              sliderPainter.drawImage(pixmapRect.center().x() -
                                          pattern.width() / 2 + 1,
                                      pixmapRect.center().y() - 4, pattern);
            } else {
              sliderPainter.drawImage(
                  pixmapRect.center().x() - 4,
                  pixmapRect.center().y() - pattern.height() / 2 + 1, pattern);
            }
          }
          sliderPainter.end();
          // insert the slider into the cache
          QPixmapCache::insert(sliderPixmapName, cache);
        }
        painter->drawPixmap(option->rect.topLeft(), cache);
      }
    }
    break;
#endif
#ifndef QT_NO_COMBOBOX
  case CE_ComboBoxLabel:
    if (const QStyleOptionComboBox *comboBox =
            qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
      painter->save();
      if (!comboBox->editable) {
        // Plastique's non-editable combo box is drawn as a button, so
        // we need the label to be drawn using ButtonText where it
        // would usually use Text.
        painter->setPen(QPen(comboBox->palette.buttonText(), 0));
        QCommonStyle::drawControl(element, option, painter, widget);
      } else if (!comboBox->currentIcon.isNull()) {
        {
          QRect editRect = proxy()->subControlRect(
              CC_ComboBox, comboBox, SC_ComboBoxEditField, widget);
          if (comboBox->direction == Qt::RightToLeft)
            editRect.adjust(0, 2, -2, -2);
          else
            editRect.adjust(2, 2, 0, -2);
          painter->save();
          painter->setClipRect(editRect);
          if (!comboBox->currentIcon.isNull()) {
            QIcon::Mode mode = comboBox->state & State_Enabled
                                   ? QIcon::Normal
                                   : QIcon::Disabled;
            QPixmap pixmap =
                comboBox->currentIcon.pixmap(comboBox->iconSize, mode);
            QRect iconRect(editRect);
            iconRect.setWidth(comboBox->iconSize.width() + 5);
            iconRect = alignedRect(comboBox->direction,
                                   Qt::AlignLeft | Qt::AlignVCenter,
                                   iconRect.size(), editRect);
            painter->fillRect(iconRect, option->palette.brush(QPalette::Base));
            proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);
          }
          painter->restore();
        }
      } else {
        QCommonStyle::drawControl(element, option, painter, widget);
      }

      painter->restore();
    }
    break;
#endif
  default:
    QCommonStyle::drawControl(element, option, painter, widget);
    break;
  }
}

/*!
  \reimp
*/
void PlastikStyle::drawComplexControl(ComplexControl control,
                                      const QStyleOptionComplex *option,
                                      QPainter *painter,
                                      const QWidget *widget) const {
  QColor borderColor = option->palette.window().color().darker(178);
  QColor alphaCornerColor;
  if (widget) {
    // ### backgroundrole/foregroundrole should be part of the style option
    alphaCornerColor = PlastikStyleHelper::mergeColors(
        option->palette.color(widget->backgroundRole()), borderColor);
  } else {
    alphaCornerColor = PlastikStyleHelper::mergeColors(
        option->palette.window().color(), borderColor);
  }
  QColor gradientStartColor = option->palette.button().color().lighter(104);
  QColor gradientStopColor = option->palette.button().color().darker(105);

  switch (control) {
#ifndef QT_NO_SLIDER
  // TODO: make the sliders look a tad bit nicer
  // TODO: paint in color from start to slider position
  // TODO: Add hover effect when sliderhandle is active
  case CC_Slider:
    if (const QStyleOptionSlider *slider =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      QRect grooveRegion =
          proxy()->subControlRect(CC_Slider, option, SC_SliderGroove, widget);
      QRect handle =
          proxy()->subControlRect(CC_Slider, option, SC_SliderHandle, widget);
      bool horizontal = slider->orientation == Qt::Horizontal;
      bool ticksAbove = slider->tickPosition & QSlider::TicksAbove;
      bool ticksBelow = slider->tickPosition & QSlider::TicksBelow;

      QRect groove;
      // The clickable region is 5 px wider than the visible groove for improved
      // usability
      if (grooveRegion.isValid())
        groove = horizontal ? grooveRegion.adjusted(0, 5, 0, -5)
                            : grooveRegion.adjusted(5, 0, -5, 0);

      QPixmap cache;

      if ((option->subControls & SC_SliderGroove) && groove.isValid()) {
        BEGIN_STYLE_PIXMAPCACHE(QString::fromLatin1("slider_groove-%0-%1")
                                    .arg(ticksAbove)
                                    .arg(ticksBelow))
        p->fillRect(groove, option->palette.window());

        rect = groove;
        rect.moveTo(groove.left() - option->rect.left(),
                    groove.top() - option->rect.top());

        // draw groove
        if (horizontal) {
          p->setPen(borderColor);
          const QLine lines[4] = {
              QLine(rect.left() + 1, rect.top(), rect.right() - 1, rect.top()),
              QLine(rect.left() + 1, rect.bottom(), rect.right() - 1,
                    rect.bottom()),
              QLine(rect.left(), rect.top() + 1, rect.left(),
                    rect.bottom() - 1),
              QLine(rect.right(), rect.top() + 1, rect.right(),
                    rect.bottom() - 1)};
          p->drawLines(lines, 4);

          p->setPen(alphaCornerColor);
          const QPoint points[4] = {QPoint(rect.left(), rect.top()),
                                    QPoint(rect.left(), rect.bottom()),
                                    QPoint(rect.right(), rect.top()),
                                    QPoint(rect.right(), rect.bottom())};
          p->drawPoints(points, 4);
        } else {
          p->setPen(borderColor);
          const QLine lines[4] = {
              QLine(rect.left() + 1, rect.top(), rect.right() - 1, rect.top()),
              QLine(rect.left() + 1, rect.bottom(), rect.right() - 1,
                    rect.bottom()),
              QLine(rect.left(), rect.top() + 1, rect.left(),
                    rect.bottom() - 1),
              QLine(rect.right(), rect.top() + 1, rect.right(),
                    rect.bottom() - 1)};
          p->drawLines(lines, 4);

          p->setPen(alphaCornerColor);
          const QPoint points[4] = {QPoint(rect.left(), rect.top()),
                                    QPoint(rect.right(), rect.top()),
                                    QPoint(rect.left(), rect.bottom()),
                                    QPoint(rect.right(), rect.bottom())};
          p->drawPoints(points, 4);
        }
        END_STYLE_PIXMAPCACHE
      }

      if ((option->subControls & SC_SliderHandle) && handle.isValid()) {
        QString handlePixmapName = QStyleHelper::uniqueName(
            QLatin1String("slider_handle"), option, handle.size());
        if (ticksAbove && !ticksBelow)
          handlePixmapName += QLatin1String("-flipped");
        if ((option->activeSubControls & SC_SliderHandle) &&
            (option->state & State_Sunken))
          handlePixmapName += QLatin1String("-sunken");

        if (!QPixmapCache::find(handlePixmapName, &cache)) {
          cache = QPixmap(handle.size());
          cache.fill(Qt::transparent);
          QRect pixmapRect(0, 0, handle.width(), handle.height());
          QPainter handlePainter(&cache);

          // draw handle
          if (horizontal) {
            QPainterPath path;
            if (ticksAbove && !ticksBelow) {
              path.moveTo(QPoint(pixmapRect.right(), pixmapRect.bottom()));
              path.lineTo(QPoint(pixmapRect.right(), pixmapRect.bottom() - 10));
              path.lineTo(
                  QPoint(pixmapRect.right() - 5, pixmapRect.bottom() - 14));
              path.lineTo(
                  QPoint(pixmapRect.left() + 1, pixmapRect.bottom() - 10));
              path.lineTo(QPoint(pixmapRect.left() + 1, pixmapRect.bottom()));
              path.lineTo(QPoint(pixmapRect.right(), pixmapRect.bottom()));
            } else {
              path.moveTo(QPoint(pixmapRect.right(), pixmapRect.top() + 1));
              path.lineTo(QPoint(pixmapRect.right(), pixmapRect.top() + 10));
              path.lineTo(
                  QPoint(pixmapRect.right() - 5, pixmapRect.top() + 14));
              path.lineTo(QPoint(pixmapRect.left() + 1, pixmapRect.top() + 10));
              path.lineTo(QPoint(pixmapRect.left() + 1, pixmapRect.top() + 1));
              path.lineTo(QPoint(pixmapRect.right(), pixmapRect.top() + 1));
            }
            if (slider->state & State_Enabled) {
              QLinearGradient gradient(
                  pixmapRect.center().x(), pixmapRect.top(),
                  pixmapRect.center().x(), pixmapRect.bottom());
              // paint the slider handle lighter if it is active and pressed
              // otherwise paint it normally
              // TODO: Add an indicator for when it is hovered over,
              if ((option->activeSubControls & SC_SliderHandle) &&
                  (option->state & State_Sunken)) {
                gradient.setColorAt(0, gradientStartColor.lighter(110));
                gradient.setColorAt(1, gradientStopColor.lighter(110));
              } else {
                gradient.setColorAt(0, gradientStartColor);
                gradient.setColorAt(1, gradientStopColor);
              }
              handlePainter.fillPath(path, gradient);

            } else {
              handlePainter.fillPath(path, slider->palette.window());
            }
          } else {
            QPainterPath path;
            if (ticksAbove && !ticksBelow) {
              path.moveTo(QPoint(pixmapRect.right(), pixmapRect.top() + 1));
              path.lineTo(
                  QPoint(pixmapRect.right() - 10, pixmapRect.top() + 1));
              path.lineTo(
                  QPoint(pixmapRect.right() - 14, pixmapRect.top() + 5));
              path.lineTo(QPoint(pixmapRect.right() - 10, pixmapRect.bottom()));
              path.lineTo(QPoint(pixmapRect.right(), pixmapRect.bottom()));
              path.lineTo(QPoint(pixmapRect.right(), pixmapRect.top() + 1));
            } else {
              path.moveTo(QPoint(pixmapRect.left() + 1, pixmapRect.top() + 1));
              path.lineTo(QPoint(pixmapRect.left() + 10, pixmapRect.top() + 1));
              path.lineTo(QPoint(pixmapRect.left() + 14, pixmapRect.top() + 5));
              path.lineTo(QPoint(pixmapRect.left() + 10, pixmapRect.bottom()));
              path.lineTo(QPoint(pixmapRect.left() + 1, pixmapRect.bottom()));
              path.lineTo(QPoint(pixmapRect.left() + 1, pixmapRect.top() + 1));
            }
            if (slider->state & State_Enabled) {
              QLinearGradient gradient(
                  pixmapRect.center().x(), pixmapRect.top(),
                  pixmapRect.center().x(), pixmapRect.bottom());
              gradient.setColorAt(0, gradientStartColor);
              gradient.setColorAt(1, gradientStopColor);
              handlePainter.fillPath(path, gradient);
            } else {
              handlePainter.fillPath(path, slider->palette.window());
            }
          }

          QImage image;
          if (horizontal) {
            image = QImage((ticksAbove && !ticksBelow)
                               ? PlastikStyleHelper::sliderHorizontalHandleUp
                               : PlastikStyleHelper::sliderHorizontalHandle);
          } else {
            image = QImage((ticksAbove && !ticksBelow)
                               ? PlastikStyleHelper::sliderVerticalHandleLeft
                               : PlastikStyleHelper::sliderVerticalHandle);
          }

          image.setColor(1, borderColor.rgba());
          image.setColor(2, gradientStartColor.rgba());
          image.setColor(3, alphaCornerColor.rgba());
          if (option->state & State_Enabled) {
            image.setColor(4, 0x80ffffff);
            image.setColor(5, 0x25000000);
          }
          handlePainter.drawImage(pixmapRect, image);
          handlePainter.end();
          QPixmapCache::insert(handlePixmapName, cache);
        }

        painter->drawPixmap(handle.topLeft(), cache);

        if (slider->state & State_HasFocus) {
          QStyleOptionFocusRect fropt;
          fropt.QStyleOption::operator=(*slider);
          fropt.rect = subElementRect(SE_SliderFocusRect, slider, widget);
          proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
        }
      }

      if (option->subControls & SC_SliderTickmarks) {
        QPen oldPen = painter->pen();
        painter->setPen(borderColor);
        int tickSize =
            proxy()->pixelMetric(PM_SliderTickmarkOffset, option, widget);
        int available =
            proxy()->pixelMetric(PM_SliderSpaceAvailable, slider, widget);
        int interval = slider->tickInterval;
        if (interval <= 0) {
          interval = slider->singleStep;
          if (QStyle::sliderPositionFromValue(slider->minimum, slider->maximum,
                                              interval, available) -
                  QStyle::sliderPositionFromValue(
                      slider->minimum, slider->maximum, 0, available) <
              3)
            interval = slider->pageStep;
        }
        if (interval <= 0)
          interval = 1;

        int v = slider->minimum;
        int len = proxy()->pixelMetric(PM_SliderLength, slider, widget);
        QVarLengthArray<QLine, 32> lines;
        while (v <= slider->maximum + 1) {
          if (v == slider->maximum + 1 && interval == 1)
            break;
          const int v_ = qMin(v, slider->maximum);
          int pos =
              sliderPositionFromValue(
                  slider->minimum, slider->maximum, v_,
                  (horizontal ? slider->rect.width() : slider->rect.height()) -
                      len,
                  slider->upsideDown) +
              len / 2;

          int extra =
              2 - ((v_ == slider->minimum || v_ == slider->maximum) ? 1 : 0);

          if (horizontal) {
            if (ticksAbove) {
              lines.append(QLine(pos, slider->rect.top() + extra, pos,
                                 slider->rect.top() + tickSize));
            }
            if (ticksBelow) {
              lines.append(QLine(pos, slider->rect.bottom() - extra, pos,
                                 slider->rect.bottom() - tickSize));
            }
          } else {
            if (ticksAbove) {
              lines.append(QLine(slider->rect.left() + extra, pos,
                                 slider->rect.left() + tickSize, pos));
            }
            if (ticksBelow) {
              lines.append(QLine(slider->rect.right() - extra, pos,
                                 slider->rect.right() - tickSize, pos));
            }
          }

          // in the case where maximum is max int
          int nextInterval = v + interval;
          if (nextInterval < v)
            break;
          v = nextInterval;
        }
        painter->drawLines(lines.constData(), lines.size());
        painter->setPen(oldPen);
      }
    }
    break;
#endif // QT_NO_SLIDER
#ifndef QT_NO_SPINBOX
  case CC_SpinBox:
    if (const QStyleOptionSpinBox *spinBox =
            qstyleoption_cast<const QStyleOptionSpinBox *>(option)) {
      painter->save();
      bool upSunken = (spinBox->activeSubControls & SC_SpinBoxUp) &&
                      (spinBox->state & (State_Sunken | State_On));
      bool downSunken = (spinBox->activeSubControls & SC_SpinBoxDown) &&
                        (spinBox->state & (State_Sunken | State_On));
      bool reverse = (spinBox->direction == Qt::RightToLeft);

      // Rects
      QRect upRect =
          proxy()->subControlRect(CC_SpinBox, option, SC_SpinBoxUp, widget);
      QRect downRect =
          proxy()->subControlRect(CC_SpinBox, option, SC_SpinBoxDown, widget);
      QRect buttonRect = upRect | downRect;

      // Brushes
      QBrush corner = PlastikStyleHelper::mapBrushToRectangle(
          option->palette.shadow(), buttonRect);
      PlastikStyleHelper::setBrushAlphaF(&corner, qreal(0.25));
      QBrush border = PlastikStyleHelper::mapBrushToRectangle(
          option->palette.shadow(), buttonRect);
      PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.4));

      QVarLengthArray<QPoint, 4> points;

      if (spinBox->buttonSymbols == QAbstractSpinBox::NoButtons) {
        QRect filledRect = option->rect.adjusted(1, 1, -1, -1);
        QBrush baseBrush = PlastikStyleHelper::mapBrushToRectangle(
            option->palette.base(), filledRect);
        painter->setBrushOrigin(filledRect.topLeft());
        painter->fillRect(filledRect.adjusted(1, 1, -1, -1), baseBrush);
        PlastikStyleHelper::drawFrame(painter, option->rect, option,
                                      QFrame::Sunken);
      } else {
        PlastikStyleHelper::drawPartialFrame(
            painter, option,
            proxy()->subControlRect(CC_SpinBox, spinBox, SC_SpinBoxEditField,
                                    widget),
            widget, this);
      }
      // Paint buttons
      if (spinBox->buttonSymbols == QAbstractSpinBox::NoButtons) {
        painter->restore();
        break;
      }
      // Button outlines
      painter->setPen(QPen(border, 0));
      if (!reverse)
        painter->drawLine(buttonRect.topLeft() + QPoint(0, 1),
                          buttonRect.bottomLeft() + QPoint(0, -1));
      else
        painter->drawLine(buttonRect.topRight() + QPoint(0, -1),
                          buttonRect.bottomRight() + QPoint(0, 1));

      if (!reverse) {
        const QLine lines[4] = {QLine(upRect.left(), upRect.top(),
                                      upRect.right() - 2, upRect.top()),
                                QLine(upRect.left() + 1, upRect.bottom(),
                                      upRect.right() - 1, upRect.bottom()),
                                QLine(downRect.left(), downRect.bottom(),
                                      downRect.right() - 2, downRect.bottom()),
                                QLine(buttonRect.right(), buttonRect.top() + 2,
                                      buttonRect.right(),
                                      buttonRect.bottom() - 2)};
        painter->drawLines(lines, 4);

        points.append(QPoint(upRect.right() - 1, upRect.top() + 1));
        points.append(QPoint(downRect.right() - 1, downRect.bottom() - 1));
        painter->drawPoints(points.constData(), points.size());
        points.clear();
        painter->setPen(QPen(corner, 0));
        points.append(QPoint(upRect.right() - 1, upRect.top()));
        points.append(QPoint(upRect.right(), upRect.top() + 1));
        points.append(QPoint(upRect.right(), downRect.bottom() - 1));
        points.append(QPoint(upRect.right() - 1, downRect.bottom()));
      } else {
        const QLine lines[4] = {QLine(upRect.right(), upRect.top(),
                                      upRect.left() + 2, upRect.top()),
                                QLine(upRect.right() - 1, upRect.bottom(),
                                      upRect.left() + 1, upRect.bottom()),
                                QLine(downRect.right(), downRect.bottom(),
                                      downRect.left() + 2, downRect.bottom()),
                                QLine(buttonRect.left(), buttonRect.top() + 2,
                                      buttonRect.left(),
                                      buttonRect.bottom() - 2)};
        painter->drawLines(lines, 4);

        points.append(QPoint(upRect.left() + 1, upRect.top() + 1));
        points.append(QPoint(downRect.left() + 1, downRect.bottom() - 1));
        painter->drawPoints(points.constData(), points.size());
        points.clear();
        painter->setPen(QPen(corner, 0));
        points.append(QPoint(upRect.left() + 1, upRect.top()));
        points.append(QPoint(upRect.left(), upRect.top() + 1));
        points.append(QPoint(upRect.left(), downRect.bottom() - 1));
        points.append(QPoint(upRect.left() + 1, downRect.bottom()));
      }
      painter->drawPoints(points.constData(), points.size());
      points.clear();

      // Button colors
      QBrush buttonGradientBrush;
      QBrush leftLineGradientBrush;
      QBrush rightLineGradientBrush;
      QBrush sunkenButtonGradientBrush;
      QBrush sunkenLeftLineGradientBrush;
      QBrush sunkenRightLineGradientBrush;
      QBrush buttonBrush = PlastikStyleHelper::mapBrushToRectangle(
          option->palette.button(), buttonRect);
      if (buttonBrush.gradient() || !buttonBrush.texture().isNull()) {
        buttonGradientBrush = buttonBrush;
        sunkenButtonGradientBrush =
            PlastikStyleHelper::darkenBrush(buttonBrush, 108);
        leftLineGradientBrush =
            PlastikStyleHelper::lightenBrush(buttonBrush, 105);
        rightLineGradientBrush =
            PlastikStyleHelper::darkenBrush(buttonBrush, 105);
        sunkenLeftLineGradientBrush =
            PlastikStyleHelper::darkenBrush(buttonBrush, 110);
        sunkenRightLineGradientBrush =
            PlastikStyleHelper::darkenBrush(buttonBrush, 106);
      } else {
        // Generate gradients
        QLinearGradient buttonGradient(buttonRect.topLeft(),
                                       buttonRect.bottomLeft());
        buttonGradient.setColorAt(0.0, buttonBrush.color().lighter(104));
        buttonGradient.setColorAt(1.0, buttonBrush.color().darker(110));
        buttonGradientBrush = QBrush(buttonGradient);

        QLinearGradient buttonGradient2(buttonRect.topLeft(),
                                        buttonRect.bottomLeft());
        buttonGradient2.setColorAt(0.0, buttonBrush.color().darker(113));
        buttonGradient2.setColorAt(1.0, buttonBrush.color().darker(103));
        sunkenButtonGradientBrush = QBrush(buttonGradient2);

        QLinearGradient buttonGradient3(buttonRect.topLeft(),
                                        buttonRect.bottomLeft());
        buttonGradient3.setColorAt(0.0, buttonBrush.color().lighter(105));
        buttonGradient3.setColorAt(1.0, buttonBrush.color());
        leftLineGradientBrush = QBrush(buttonGradient3);

        QLinearGradient buttonGradient4(buttonRect.topLeft(),
                                        buttonRect.bottomLeft());
        buttonGradient4.setColorAt(0.0, buttonBrush.color());
        buttonGradient4.setColorAt(1.0, buttonBrush.color().darker(110));
        rightLineGradientBrush = QBrush(buttonGradient4);

        QLinearGradient buttonGradient5(buttonRect.topLeft(),
                                        buttonRect.bottomLeft());
        buttonGradient5.setColorAt(0.0, buttonBrush.color().darker(113));
        buttonGradient5.setColorAt(1.0, buttonBrush.color().darker(107));
        sunkenLeftLineGradientBrush = QBrush(buttonGradient5);

        QLinearGradient buttonGradient6(buttonRect.topLeft(),
                                        buttonRect.bottomLeft());
        buttonGradient6.setColorAt(0.0, buttonBrush.color().darker(108));
        buttonGradient6.setColorAt(1.0, buttonBrush.color().darker(103));
        sunkenRightLineGradientBrush = QBrush(buttonGradient6);
      }

      // Main fill
      painter->fillRect(
          upRect.adjusted(2, 2, -2, -2),
          PlastikStyleHelper::mapBrushToRectangle(
              upSunken ? sunkenButtonGradientBrush : buttonGradientBrush,
              upRect));
      painter->fillRect(
          downRect.adjusted(2, 2, -2, -2),
          PlastikStyleHelper::mapBrushToRectangle(
              downSunken ? sunkenButtonGradientBrush : buttonGradientBrush,
              downRect));

      // Top line
      painter->setPen(QPen(
          PlastikStyleHelper::lightenBrush(
              PlastikStyleHelper::mapBrushToRectangle(
                  upSunken ? sunkenButtonGradientBrush : buttonGradientBrush,
                  upRect),
              105),
          0));
      if (!reverse) {
        painter->drawLine(upRect.left() + 1, upRect.top() + 1,
                          upRect.right() - 2, upRect.top() + 1);
      } else {
        painter->drawLine(upRect.right() - 1, upRect.top() + 1,
                          upRect.left() + 2, upRect.top() + 1);
      }
      painter->setPen(QPen(
          PlastikStyleHelper::lightenBrush(
              PlastikStyleHelper::mapBrushToRectangle(
                  downSunken ? sunkenButtonGradientBrush : buttonGradientBrush,
                  downRect),
              105),
          0));
      if (!reverse) {
        painter->drawLine(downRect.left() + 1, downRect.top() + 1,
                          downRect.right() - 1, downRect.top() + 1);
      } else {
        painter->drawLine(downRect.right() - 1, downRect.top() + 1,
                          downRect.left() + 1, downRect.top() + 1);
      }

      // Left line
      painter->setPen(QPen(
          PlastikStyleHelper::mapBrushToRectangle(
              upSunken ? sunkenLeftLineGradientBrush : leftLineGradientBrush,
              upRect),
          1));
      if (!reverse) {
        painter->drawLine(upRect.left() + 1, upRect.top() + 2,
                          upRect.left() + 1, upRect.bottom() - 1);
      } else {
        painter->drawLine(upRect.left() + 1, upRect.top() + 2,
                          upRect.left() + 1, upRect.bottom() - 1);
      }
      painter->setPen(QPen(
          PlastikStyleHelper::mapBrushToRectangle(
              downSunken ? sunkenLeftLineGradientBrush : leftLineGradientBrush,
              downRect),
          1));
      if (!reverse) {
        painter->drawLine(downRect.left() + 1, downRect.top() + 2,
                          downRect.left() + 1, downRect.bottom() - 1);
      } else {
        painter->drawLine(downRect.left() + 1, downRect.top() + 1,
                          downRect.left() + 1, downRect.bottom() - 2);
      }

      // Bottom line
      painter->setPen(QPen(
          PlastikStyleHelper::darkenBrush(
              PlastikStyleHelper::mapBrushToRectangle(
                  upSunken ? sunkenButtonGradientBrush : buttonGradientBrush,
                  upRect),
              105),
          0));
      if (!reverse) {
        painter->drawLine(upRect.left() + 2, upRect.bottom() - 1,
                          upRect.right() - 1, upRect.bottom() - 1);
      } else {
        painter->drawLine(upRect.right() - 2, upRect.bottom() - 1,
                          upRect.left() + 1, upRect.bottom() - 1);
      }
      painter->setPen(QPen(
          PlastikStyleHelper::darkenBrush(
              PlastikStyleHelper::mapBrushToRectangle(
                  downSunken ? sunkenButtonGradientBrush : buttonGradientBrush,
                  downRect),
              105),
          0));
      if (!reverse) {
        painter->drawLine(downRect.left() + 2, downRect.bottom() - 1,
                          downRect.right() - 2, downRect.bottom() - 1);
      } else {
        painter->drawLine(downRect.right() - 2, downRect.bottom() - 1,
                          downRect.left() + 2, downRect.bottom() - 1);
      }

      // Right line
      painter->setPen(QPen(
          PlastikStyleHelper::mapBrushToRectangle(
              upSunken ? sunkenRightLineGradientBrush : rightLineGradientBrush,
              upRect),
          1));
      if (!reverse) {
        painter->drawLine(upRect.right() - 1, upRect.top() + 2,
                          upRect.right() - 1, upRect.bottom() - 1);
      } else {
        painter->drawLine(upRect.right() - 1, upRect.top() + 2,
                          upRect.right() - 1, upRect.bottom() - 1);
      }
      painter->setPen(QPen(PlastikStyleHelper::mapBrushToRectangle(
                               downSunken ? sunkenRightLineGradientBrush
                                          : rightLineGradientBrush,
                               downRect),
                           1));
      if (!reverse) {
        painter->drawLine(downRect.right() - 1, downRect.top() + 1,
                          downRect.right() - 1, downRect.bottom() - 2);
      } else {
        painter->drawLine(downRect.right() - 1, downRect.top() + 2,
                          downRect.right() - 1, downRect.bottom() - 1);
      }

      QBrush indicatorBrush = PlastikStyleHelper::mapBrushToRectangle(
          option->palette.buttonText(), buttonRect);
      painter->setPen(QPen(indicatorBrush, 0));
      if (spinBox->buttonSymbols == QAbstractSpinBox::PlusMinus) {
        QPoint center;
        if (spinBox->subControls & SC_SpinBoxUp) {
          // .......
          // ...X...
          // ...X...
          // .XXXXX.
          // ...X...
          // ...X...
          // .......
          center = upRect.center();
          if (upSunken) {
            ++center.rx();
            ++center.ry();
          }
          painter->drawLine(center.x(), center.y() - 2, center.x(),
                            center.y() + 2);
          painter->drawLine(center.x() - 2, center.y(), center.x() + 2,
                            center.y());
        }
        if (spinBox->subControls & SC_SpinBoxDown) {
          // .......
          // .......
          // .......
          // .XXXXX.
          // .......
          // .......
          // .......
          center = downRect.center();
          if (downSunken) {
            ++center.rx();
            ++center.ry();
          }
          painter->drawLine(center.x() - 2, center.y(), center.x() + 2,
                            center.y());
        }
      } else {
        int offset;
        int centerX;
        if (spinBox->subControls & SC_SpinBoxUp) {
          // ...........
          // .....X.....
          // ....XXX....
          // ...XXXXX...
          // ..XXXXXXX..
          // ...........
          offset = upSunken ? 1 : 0;
          QRect upArrowRect(upRect.center().x() - 3 + offset,
                            upRect.center().y() - 2 + offset, 7, 4);
          centerX = upArrowRect.center().x();
          painter->drawPoint(centerX, upArrowRect.top());
          const QLine lines[3] = {QLine(centerX - 1, upArrowRect.top() + 1,
                                        centerX + 1, upArrowRect.top() + 1),
                                  QLine(centerX - 2, upArrowRect.top() + 2,
                                        centerX + 2, upArrowRect.top() + 2),
                                  QLine(centerX - 3, upArrowRect.top() + 3,
                                        centerX + 3, upArrowRect.top() + 3)};
          painter->drawLines(lines, 3);
        }
        if (spinBox->subControls & SC_SpinBoxDown) {
          // ...........
          // ..XXXXXXX..
          // ...XXXXX...
          // ....XXX....
          // .....X.....
          // ...........
          offset = downSunken ? 1 : 0;
          QRect downArrowRect(downRect.center().x() - 3 + offset,
                              downRect.center().y() - 2 + offset + 1, 7, 4);
          centerX = downArrowRect.center().x();
          const QLine lines[3] = {QLine(centerX - 3, downArrowRect.top(),
                                        centerX + 3, downArrowRect.top()),
                                  QLine(centerX - 2, downArrowRect.top() + 1,
                                        centerX + 2, downArrowRect.top() + 1),
                                  QLine(centerX - 1, downArrowRect.top() + 2,
                                        centerX + 1, downArrowRect.top() + 2)};
          painter->drawLines(lines, 3);
          painter->drawPoint(centerX, downArrowRect.top() + 3);
        }
      }
      painter->restore();
    }
    break;
#endif // QT_NO_SPINBOX
#ifndef QT_NO_COMBOBOX
  case CC_ComboBox:
    if (const QStyleOptionComboBox *comboBox =
            qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
      bool sunken =
          comboBox->state & State_On; // play dead if combobox has no items
      bool reverse = comboBox->direction == Qt::RightToLeft;
      int menuButtonWidth = 16;
      int xoffset = sunken ? (reverse ? -1 : 1) : 0;
      int yoffset = sunken ? 1 : 0;
      QRect rect = comboBox->rect;
      QPen oldPen = painter->pen();

      // Fill
      if (comboBox->editable) {
        // Button colors
        QBrush alphaCornerBrush =
            PlastikStyleHelper::darkenBrush(option->palette.button(), 165);
        PlastikStyleHelper::setBrushAlphaF(&alphaCornerBrush, 0.5);
        QBrush buttonGradientBrush;
        QBrush leftLineGradientBrush;
        QBrush rightLineGradientBrush;
        QBrush sunkenButtonGradientBrush;
        QBrush sunkenLeftLineGradientBrush;
        QBrush sunkenRightLineGradientBrush;
        QBrush button = option->palette.button();
        if (button.gradient() || !button.texture().isNull()) {
          buttonGradientBrush = button;
          sunkenButtonGradientBrush =
              PlastikStyleHelper::darkenBrush(button, 108);
          leftLineGradientBrush = PlastikStyleHelper::lightenBrush(button, 105);
          rightLineGradientBrush = PlastikStyleHelper::darkenBrush(button, 105);
          sunkenLeftLineGradientBrush =
              PlastikStyleHelper::darkenBrush(button, 110);
          sunkenRightLineGradientBrush =
              PlastikStyleHelper::darkenBrush(button, 106);
        } else {
          // Generate gradients
          QLinearGradient buttonGradient(option->rect.topLeft(),
                                         option->rect.bottomLeft());
          buttonGradient.setColorAt(0.0, button.color().lighter(104));
          buttonGradient.setColorAt(1.0, button.color().darker(110));
          buttonGradientBrush = QBrush(buttonGradient);

          QLinearGradient buttonGradient2(option->rect.topLeft(),
                                          option->rect.bottomLeft());
          buttonGradient2.setColorAt(0.0, button.color().darker(113));
          buttonGradient2.setColorAt(1.0, button.color().darker(103));
          sunkenButtonGradientBrush = QBrush(buttonGradient2);

          QLinearGradient buttonGradient3(option->rect.topLeft(),
                                          option->rect.bottomLeft());
          buttonGradient3.setColorAt(0.0, button.color().lighter(105));
          buttonGradient3.setColorAt(1.0, button.color());
          leftLineGradientBrush = QBrush(buttonGradient3);

          QLinearGradient buttonGradient4(option->rect.topLeft(),
                                          option->rect.bottomLeft());
          buttonGradient4.setColorAt(0.0, button.color());
          buttonGradient4.setColorAt(1.0, button.color().darker(110));
          rightLineGradientBrush = QBrush(buttonGradient4);

          QLinearGradient buttonGradient5(option->rect.topLeft(),
                                          option->rect.bottomLeft());
          buttonGradient5.setColorAt(0.0, button.color().darker(113));
          buttonGradient5.setColorAt(1.0, button.color().darker(107));
          sunkenLeftLineGradientBrush = QBrush(buttonGradient5);

          QLinearGradient buttonGradient6(option->rect.topLeft(),
                                          option->rect.bottomLeft());
          buttonGradient6.setColorAt(0.0, button.color().darker(108));
          buttonGradient6.setColorAt(1.0, button.color().darker(103));
          sunkenRightLineGradientBrush = QBrush(buttonGradient6);
        }

        // ComboBox starts with a lineedit in place already.
        QRect buttonRect;
        if (!reverse) {
          buttonRect.setRect(rect.right() - menuButtonWidth, rect.top(),
                             menuButtonWidth + 1, rect.height());
        } else {
          buttonRect.setRect(rect.left(), rect.top(), menuButtonWidth + 1,
                             rect.height());
        }

        PlastikStyleHelper::drawPartialFrame(
            painter, option,
            proxy()->subControlRect(CC_ComboBox, option, SC_ComboBoxEditField,
                                    widget),
            widget, this);

        QBrush border = PlastikStyleHelper::mapBrushToRectangle(
            option->palette.shadow(), buttonRect);
        PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.4));
        painter->setPen(QPen(border, 0));
        if (!reverse)
          painter->drawLine(buttonRect.topLeft() + QPoint(0, 1),
                            buttonRect.bottomLeft() + QPoint(0, -1));
        else
          painter->drawLine(buttonRect.topRight() + QPoint(0, -1),
                            buttonRect.bottomRight() + QPoint(0, 1));

        // Outline the button border
        if (!reverse) {
          const QLine lines[3] = {
              QLine(buttonRect.left(), buttonRect.top(), buttonRect.right() - 2,
                    buttonRect.top()),
              QLine(buttonRect.right(), buttonRect.top() + 2,
                    buttonRect.right(), buttonRect.bottom() - 2),
              QLine(buttonRect.left(), buttonRect.bottom(),
                    buttonRect.right() - 2, buttonRect.bottom())};
          painter->drawLines(lines, 3);
          {
            const QPoint points[2] = {
                QPoint(buttonRect.right() - 1, buttonRect.top() + 1),
                QPoint(buttonRect.right() - 1, buttonRect.bottom() - 1)};
            painter->drawPoints(points, 2);
          }

          QBrush corner = PlastikStyleHelper::mapBrushToRectangle(
              option->palette.shadow(), buttonRect);
          PlastikStyleHelper::setBrushAlphaF(&corner, qreal(0.16));
          painter->setPen(QPen(corner, 0));
          {
            const QPoint points[4] = {
                QPoint(buttonRect.right() - 1, buttonRect.top()),
                QPoint(buttonRect.right() - 1, buttonRect.bottom()),
                QPoint(buttonRect.right(), buttonRect.top() + 1),
                QPoint(buttonRect.right(), buttonRect.bottom() - 1)};
            painter->drawPoints(points, 4);
          }
        } else {
          const QLine lines[3] = {
              QLine(buttonRect.right(), buttonRect.top(), buttonRect.left() + 2,
                    buttonRect.top()),
              QLine(buttonRect.left(), buttonRect.top() + 2, buttonRect.left(),
                    buttonRect.bottom() - 2),
              QLine(buttonRect.right(), buttonRect.bottom(),
                    buttonRect.left() + 2, buttonRect.bottom())};
          painter->drawLines(lines, 3);
          {
            const QPoint points[2] = {
                QPoint(buttonRect.left() + 1, buttonRect.top() + 1),
                QPoint(buttonRect.left() + 1, buttonRect.bottom() - 1)};
            painter->drawPoints(points, 2);
          }

          QBrush corner = PlastikStyleHelper::mapBrushToRectangle(
              option->palette.shadow(), buttonRect);
          PlastikStyleHelper::setBrushAlphaF(&corner, qreal(0.16));
          painter->setPen(QPen(corner, 0));
          {
            const QPoint points[4] = {
                QPoint(buttonRect.left() + 1, buttonRect.top()),
                QPoint(buttonRect.left() + 1, buttonRect.bottom()),
                QPoint(buttonRect.left(), buttonRect.top() + 1),
                QPoint(buttonRect.left(), buttonRect.bottom() - 1)};
            painter->drawPoints(points, 4);
          }
        }

        QRect fillRect = buttonRect.adjusted(2, 2, -2, -2);
        // Main fill
        painter->fillRect(fillRect, PlastikStyleHelper::mapBrushToRectangle(
                                        sunken ? sunkenButtonGradientBrush
                                               : buttonGradientBrush,
                                        option->rect));

        // Top line
        painter->setPen(QPen(
            PlastikStyleHelper::lightenBrush(
                PlastikStyleHelper::mapBrushToRectangle(
                    sunken ? sunkenButtonGradientBrush : buttonGradientBrush,
                    option->rect),
                105),
            0));
        if (!reverse) {
          painter->drawLine(
              QPointF(buttonRect.left() + 1, buttonRect.top() + 1),
              QPointF(buttonRect.right() - 2, buttonRect.top() + 1));
        } else {
          painter->drawLine(
              QPointF(buttonRect.right() - 1, buttonRect.top() + 1),
              QPointF(buttonRect.left() + 2, buttonRect.top() + 1));
        }

        // Bottom line
        painter->setPen(QPen(
            PlastikStyleHelper::darkenBrush(
                PlastikStyleHelper::mapBrushToRectangle(
                    sunken ? sunkenButtonGradientBrush : buttonGradientBrush,
                    option->rect),
                105),
            0));
        if (!reverse) {
          painter->drawLine(
              QPointF(buttonRect.left() + 1, buttonRect.bottom() - 1),
              QPointF(buttonRect.right() - 2, buttonRect.bottom() - 1));
        } else {
          painter->drawLine(
              QPointF(buttonRect.right() - 1, buttonRect.bottom() - 1),
              QPointF(buttonRect.left() + 2, buttonRect.bottom() - 1));
        }

        // Left line
        painter->setPen(QPen(
            PlastikStyleHelper::mapBrushToRectangle(
                sunken ? sunkenLeftLineGradientBrush : leftLineGradientBrush,
                option->rect),
            1));
        if (!reverse) {
          painter->drawLine(
              QPointF(buttonRect.left() + 1, buttonRect.top() + 2),
              QPointF(buttonRect.left() + 1, buttonRect.bottom() - 2));
        } else {
          painter->drawLine(
              QPointF(buttonRect.left() + 1, buttonRect.top() + 2),
              QPointF(buttonRect.left() + 1, buttonRect.bottom() - 2));
        }

        // Right line
        painter->setPen(QPen(
            PlastikStyleHelper::mapBrushToRectangle(
                sunken ? sunkenRightLineGradientBrush : rightLineGradientBrush,
                option->rect),
            1));
        if (!reverse) {
          painter->drawLine(
              QPointF(buttonRect.right() - 1, buttonRect.top() + 2),
              QPointF(buttonRect.right() - 1, buttonRect.bottom() - 2));
        } else {
          painter->drawLine(
              QPointF(buttonRect.right() - 1, buttonRect.top() + 2),
              QPointF(buttonRect.right() - 1, buttonRect.bottom() - 2));
        }
      } else {
        // Start with a standard panel button fill
        QStyleOptionButton buttonOption;
        buttonOption.QStyleOption::operator=(*comboBox);
        if (!sunken) {
          buttonOption.state &= ~State_Sunken;
        }
        proxy()->drawPrimitive(PE_PanelButtonCommand, &buttonOption, painter,
                               widget);

        // Draw the menu button separator line
        QBrush border = PlastikStyleHelper::mapBrushToRectangle(
            option->palette.shadow(), rect);
        PlastikStyleHelper::setBrushAlphaF(&border, qreal(0.35));
        painter->setPen(QPen(border, 0));
        if (!reverse) {
          painter->drawLine(
              rect.right() - menuButtonWidth + xoffset, rect.top() + 1,
              rect.right() - menuButtonWidth + xoffset, rect.bottom() - 1);
        } else {
          painter->drawLine(
              rect.left() + menuButtonWidth + xoffset, rect.top() + 1,
              rect.left() + menuButtonWidth + xoffset, rect.bottom() - 1);
        }
      }

      // Draw the little arrow
      if (comboBox->subControls & SC_ComboBoxArrow) {
        int left = !reverse ? rect.right() - menuButtonWidth : rect.left();
        int right = !reverse ? rect.right() : rect.left() + menuButtonWidth;
        QRect arrowRect((left + right) / 2 - 3 + xoffset,
                        rect.center().y() - 1 + yoffset, 7, 4);
        painter->setPen(QPen(PlastikStyleHelper::mapBrushToRectangle(
                                 option->palette.buttonText(), rect),
                             0));
        const QLine lines[3] = {
            QLine(arrowRect.topLeft(), arrowRect.topRight()),
            QLine(arrowRect.left() + 1, arrowRect.top() + 1,
                  arrowRect.right() - 1, arrowRect.top() + 1),
            QLine(arrowRect.left() + 2, arrowRect.top() + 2,
                  arrowRect.right() - 2, arrowRect.top() + 2)};
        painter->drawLines(lines, 3);
        painter->drawPoint(arrowRect.center().x(), arrowRect.bottom());
      }

      // Draw the focus rect
      if ((option->state & State_HasFocus) && !comboBox->editable &&
          ((option->state & State_KeyboardFocusChange) ||
           styleHint(SH_UnderlineShortcut, option, widget))) {
        QStyleOptionFocusRect focus;
        focus.rect = proxy()
                         ->subControlRect(CC_ComboBox, option,
                                          SC_ComboBoxEditField, widget)
                         .adjusted(-2, 0, 2, 0);
        proxy()->drawPrimitive(PE_FrameFocusRect, &focus, painter, widget);
      }

      painter->setPen(oldPen);
    }
    break;
#endif // QT_NO_COMBOBOX
  case CC_TitleBar:
    if (const QStyleOptionTitleBar *titleBar =
            qstyleoption_cast<const QStyleOptionTitleBar *>(option)) {
      painter->save();
      bool active = (titleBar->titleBarState & State_Active);
      QRect fullRect = titleBar->rect;

      // ### use palette colors instead
      QColor titleBarGradientStart(active ? 0x3b508a : 0x6e6e6e);
      QColor titleBarGradientStop(active ? 0x5d6e9e : 0x818181);
      QColor titleBarFrameBorder(0x393939);
      QColor titleBarAlphaCorner(active ? 0x4b5e7f : 0x6a6a6a);
      QColor titleBarInnerTopLine(active ? 0x8e98ba : 0xa4a4a4);
      QColor titleBarInnerInnerTopLine(active ? 0x57699b : 0x808080);
      QColor leftCorner(active ? 0x6f7ea8 : 0x8e8e8e);
      QColor rightCorner(active ? 0x44537d : 0x676767);
      QColor textColor(active ? 0x282e40 : 0x282e40);
      QColor textAlphaColor(active ? 0x3f4862 : 0x3f4862);

      {
        // Fill title bar gradient
        PlastikStyleHelper::drawGradient(
            painter, option->rect.adjusted(1, 1, -1, 0), titleBarGradientStart,
            titleBarGradientStop);

        // Frame and rounded corners
        painter->setPen(titleBarFrameBorder);

        // top border line
        {
          const QLine lines[3] = {QLine(fullRect.left() + 2, fullRect.top(),
                                        fullRect.right() - 2, fullRect.top()),
                                  QLine(fullRect.left(), fullRect.top() + 2,
                                        fullRect.left(), fullRect.bottom()),
                                  QLine(fullRect.right(), fullRect.top() + 2,
                                        fullRect.right(), fullRect.bottom())};
          painter->drawLines(lines, 3);
          const QPoint points[2] = {
              QPoint(fullRect.left() + 1, fullRect.top() + 1),
              QPoint(fullRect.right() - 1, fullRect.top() + 1)};
          painter->drawPoints(points, 2);
        }

        // alpha corners
        painter->setPen(titleBarAlphaCorner);
        {
          const QPoint points[4] = {
              QPoint(fullRect.left() + 2, fullRect.top() + 1),
              QPoint(fullRect.left() + 1, fullRect.top() + 2),
              QPoint(fullRect.right() - 2, fullRect.top() + 1),
              QPoint(fullRect.right() - 1, fullRect.top() + 2)};
          painter->drawPoints(points, 4);
        }

        // inner top line
        painter->setPen(titleBarInnerTopLine);
        painter->drawLine(fullRect.left() + 3, fullRect.top() + 1,
                          fullRect.right() - 3, fullRect.top() + 1);

        // inner inner top line
        painter->setPen(titleBarInnerInnerTopLine);
        painter->drawLine(fullRect.left() + 2, fullRect.top() + 2,
                          fullRect.right() - 2, fullRect.top() + 2);

        // left and right inner
        painter->setPen(leftCorner);
        painter->drawLine(fullRect.left() + 1, fullRect.top() + 3,
                          fullRect.left() + 1, fullRect.bottom());
        painter->setPen(rightCorner);
        painter->drawLine(fullRect.right() - 1, fullRect.top() + 3,
                          fullRect.right() - 1, fullRect.bottom());

        if (titleBar->titleBarState & Qt::WindowMinimized) {
          painter->setPen(titleBarFrameBorder);
          painter->drawLine(fullRect.left() + 2, fullRect.bottom(),
                            fullRect.right() - 2, fullRect.bottom());
          {
            const QPoint points[2] = {
                QPoint(fullRect.left() + 1, fullRect.bottom() - 1),
                QPoint(fullRect.right() - 1, fullRect.bottom() - 1)};
            painter->drawPoints(points, 2);
          }
          painter->setPen(rightCorner);
          painter->drawLine(fullRect.left() + 2, fullRect.bottom() - 1,
                            fullRect.right() - 2, fullRect.bottom() - 1);
          painter->setPen(titleBarAlphaCorner);
          {
            const QPoint points[4] = {
                QPoint(fullRect.left() + 1, fullRect.bottom() - 2),
                QPoint(fullRect.left() + 2, fullRect.bottom() - 1),
                QPoint(fullRect.right() - 1, fullRect.bottom() - 2),
                QPoint(fullRect.right() - 2, fullRect.bottom() - 1)};
            painter->drawPoints(points, 4);
          }
        }
        // draw title
        QRect textRect = proxy()->subControlRect(CC_TitleBar, titleBar,
                                                 SC_TitleBarLabel, widget);

        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(titleBar->palette.text().color());

        // Attempt to align left if there is not enough room for the title
        // text. Otherwise, align center. QWorkspace does elliding for us,
        // and it doesn't know about the bold title, so we need to work
        // around some of the width mismatches.
        bool tooWide = (QFontMetrics(font).horizontalAdvance(titleBar->text) >
                        textRect.width());
        QTextOption option((tooWide ? Qt::AlignLeft : Qt::AlignHCenter) |
                           Qt::AlignVCenter);
        option.setWrapMode(QTextOption::NoWrap);

        painter->drawText(textRect.adjusted(1, 1, 1, 1), titleBar->text,
                          option);
        painter->setPen(titleBar->palette.highlightedText().color());
        painter->drawText(textRect, titleBar->text, option);
      }

      // min button
      if ((titleBar->subControls & SC_TitleBarMinButton) &&
          (titleBar->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
          !(titleBar->titleBarState & Qt::WindowMinimized)) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarMinButton) &&
                     (titleBar->state & State_MouseOver);
        bool sunken = (titleBar->activeSubControls & SC_TitleBarMinButton) &&
                      (titleBar->state & State_Sunken);

        QRect minButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarMinButton, widget);
        PlastikStyleHelper::drawMdiButton(painter, titleBar, minButtonRect,
                                          hover, sunken);

        int xoffset = minButtonRect.width() / 3;
        int yoffset = minButtonRect.height() / 3;

        QRect minButtonIconRect(minButtonRect.left() + xoffset,
                                minButtonRect.top() + yoffset,
                                minButtonRect.width() - xoffset * 2,
                                minButtonRect.height() - yoffset * 2);

        painter->setPen(textColor);
        {
          const QLine lines[2] = {QLine(minButtonIconRect.center().x() - 2,
                                        minButtonIconRect.center().y() + 3,
                                        minButtonIconRect.center().x() + 3,
                                        minButtonIconRect.center().y() + 3),
                                  QLine(minButtonIconRect.center().x() - 2,
                                        minButtonIconRect.center().y() + 4,
                                        minButtonIconRect.center().x() + 3,
                                        minButtonIconRect.center().y() + 4)};
          painter->drawLines(lines, 2);
        }
        painter->setPen(textAlphaColor);
        {
          const QLine lines[2] = {QLine(minButtonIconRect.center().x() - 3,
                                        minButtonIconRect.center().y() + 3,
                                        minButtonIconRect.center().x() - 3,
                                        minButtonIconRect.center().y() + 4),
                                  QLine(minButtonIconRect.center().x() + 4,
                                        minButtonIconRect.center().y() + 3,
                                        minButtonIconRect.center().x() + 4,
                                        minButtonIconRect.center().y() + 4)};
          painter->drawLines(lines, 2);
        }
      }

      // max button
      if ((titleBar->subControls & SC_TitleBarMaxButton) &&
          (titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
          !(titleBar->titleBarState & Qt::WindowMaximized)) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarMaxButton) &&
                     (titleBar->state & State_MouseOver);
        bool sunken = (titleBar->activeSubControls & SC_TitleBarMaxButton) &&
                      (titleBar->state & State_Sunken);

        QRect maxButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarMaxButton, widget);
        PlastikStyleHelper::drawMdiButton(painter, titleBar, maxButtonRect,
                                          hover, sunken);

        int xoffset = maxButtonRect.width() / 3;
        int yoffset = maxButtonRect.height() / 3;

        QRect maxButtonIconRect(maxButtonRect.left() + xoffset,
                                maxButtonRect.top() + yoffset,
                                maxButtonRect.width() - xoffset * 2,
                                maxButtonRect.height() - yoffset * 2);

        painter->setPen(textColor);
        painter->drawRect(maxButtonIconRect.adjusted(0, 0, -1, -1));
        painter->drawLine(
            maxButtonIconRect.left() + 1, maxButtonIconRect.top() + 1,
            maxButtonIconRect.right() - 1, maxButtonIconRect.top() + 1);
        painter->setPen(textAlphaColor);
        const QPoint points[4] = {
            maxButtonIconRect.topLeft(), maxButtonIconRect.topRight(),
            maxButtonIconRect.bottomLeft(), maxButtonIconRect.bottomRight()};
        painter->drawPoints(points, 4);
      }

      // close button
      if (titleBar->subControls & SC_TitleBarCloseButton &&
          titleBar->titleBarFlags & Qt::WindowSystemMenuHint) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarCloseButton) &&
                     (titleBar->state & State_MouseOver);
        bool sunken = (titleBar->activeSubControls & SC_TitleBarCloseButton) &&
                      (titleBar->state & State_Sunken);

        QRect closeButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarCloseButton, widget);
        PlastikStyleHelper::drawMdiButton(painter, titleBar, closeButtonRect,
                                          hover, sunken);

        int xoffset = closeButtonRect.width() / 3;
        int yoffset = closeButtonRect.height() / 3;

        QRect closeIconRect(closeButtonRect.left() + xoffset,
                            closeButtonRect.top() + yoffset,
                            closeButtonRect.width() - xoffset * 2,
                            closeButtonRect.height() - yoffset * 2);

        painter->setPen(textAlphaColor);
        {
          const QLine lines[4] = {
              QLine(closeIconRect.left() + 1, closeIconRect.top(),
                    closeIconRect.right(), closeIconRect.bottom() - 1),
              QLine(closeIconRect.left(), closeIconRect.top() + 1,
                    closeIconRect.right() - 1, closeIconRect.bottom()),
              QLine(closeIconRect.right() - 1, closeIconRect.top(),
                    closeIconRect.left(), closeIconRect.bottom() - 1),
              QLine(closeIconRect.right(), closeIconRect.top() + 1,
                    closeIconRect.left() + 1, closeIconRect.bottom())};
          painter->drawLines(lines, 4);
          const QPoint points[4] = {
              closeIconRect.topLeft(), closeIconRect.topRight(),
              closeIconRect.bottomLeft(), closeIconRect.bottomRight()};
          painter->drawPoints(points, 4);
        }
        painter->setPen(textColor);
        {
          const QLine lines[2] = {
              QLine(closeIconRect.left() + 1, closeIconRect.top() + 1,
                    closeIconRect.right() - 1, closeIconRect.bottom() - 1),
              QLine(closeIconRect.left() + 1, closeIconRect.bottom() - 1,
                    closeIconRect.right() - 1, closeIconRect.top() + 1)};
          painter->drawLines(lines, 2);
        }
      }

      // normalize button
      if ((titleBar->subControls & SC_TitleBarNormalButton) &&
          (((titleBar->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
            (titleBar->titleBarState & Qt::WindowMinimized)) ||
           ((titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
            (titleBar->titleBarState & Qt::WindowMaximized)))) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarNormalButton) &&
                     (titleBar->state & State_MouseOver);
        bool sunken = (titleBar->activeSubControls & SC_TitleBarNormalButton) &&
                      (titleBar->state & State_Sunken);

        QRect normalButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarNormalButton, widget);
        PlastikStyleHelper::drawMdiButton(painter, titleBar, normalButtonRect,
                                          hover, sunken);
        int xoffset = int(normalButtonRect.width() / 3.5);
        int yoffset = int(normalButtonRect.height() / 3.5);

        QRect normalButtonIconRect(normalButtonRect.left() + xoffset,
                                   normalButtonRect.top() + yoffset,
                                   normalButtonRect.width() - xoffset * 2,
                                   normalButtonRect.height() - yoffset * 2);

        QRect frontWindowRect = normalButtonIconRect.adjusted(0, 3, -3, 0);
        painter->setPen(textColor);
        painter->drawRect(frontWindowRect.adjusted(0, 0, -1, -1));
        painter->drawLine(frontWindowRect.left() + 1, frontWindowRect.top() + 1,
                          frontWindowRect.right() - 1,
                          frontWindowRect.top() + 1);
        painter->setPen(textAlphaColor);
        {
          const QPoint points[4] = {
              frontWindowRect.topLeft(), frontWindowRect.topRight(),
              frontWindowRect.bottomLeft(), frontWindowRect.bottomRight()};
          painter->drawPoints(points, 4);
        }

        QRect backWindowRect = normalButtonIconRect.adjusted(3, 0, 0, -3);
        QRegion clipRegion = backWindowRect;
        clipRegion -= frontWindowRect;
        painter->save();
        painter->setClipRegion(clipRegion);
        painter->setPen(textColor);
        painter->drawRect(backWindowRect.adjusted(0, 0, -1, -1));
        painter->drawLine(backWindowRect.left() + 1, backWindowRect.top() + 1,
                          backWindowRect.right() - 1, backWindowRect.top() + 1);
        painter->setPen(textAlphaColor);
        {
          const QPoint points[4] = {
              backWindowRect.topLeft(), backWindowRect.topRight(),
              backWindowRect.bottomLeft(), backWindowRect.bottomRight()};
          painter->drawPoints(points, 4);
        }
        painter->restore();
      }

      // context help button
      if (titleBar->subControls & SC_TitleBarContextHelpButton &&
          (titleBar->titleBarFlags & Qt::WindowContextHelpButtonHint)) {
        bool hover =
            (titleBar->activeSubControls & SC_TitleBarContextHelpButton) &&
            (titleBar->state & State_MouseOver);
        bool sunken =
            (titleBar->activeSubControls & SC_TitleBarContextHelpButton) &&
            (titleBar->state & State_Sunken);

        QRect contextHelpButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarContextHelpButton, widget);

        PlastikStyleHelper::drawMdiButton(painter, titleBar,
                                          contextHelpButtonRect, hover, sunken);

        QColor blend;
        // ### Use palette colors
        if (active) {
          blend = PlastikStyleHelper::mergeColors(
              QColor(hover ? 0x7d8bb1 : 0x55689a),
              QColor(hover ? 0x939ebe : 0x7381ab));
        } else {
          blend = PlastikStyleHelper::mergeColors(
              QColor(hover ? 0x9e9e9e : 0x818181),
              QColor(hover ? 0xababab : 0x929292));
        }
        QImage image(PlastikStyleHelper::titleBarContextHelp);
        image.setColor(4, textColor.rgba());
        image.setColor(
            3, PlastikStyleHelper::mergeColors(blend, textColor, 30).rgba());
        image.setColor(
            2, PlastikStyleHelper::mergeColors(blend, textColor, 70).rgba());
        image.setColor(
            1, PlastikStyleHelper::mergeColors(blend, textColor, 90).rgba());

        painter->drawImage(contextHelpButtonRect, image);
      }

      // shade button
      if (titleBar->subControls & SC_TitleBarShadeButton) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarShadeButton) &&
                     (titleBar->state & State_MouseOver);
        bool sunken = (titleBar->activeSubControls & SC_TitleBarShadeButton) &&
                      (titleBar->state & State_Sunken);

        QRect shadeButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarShadeButton, widget);
        PlastikStyleHelper::drawMdiButton(painter, titleBar, shadeButtonRect,
                                          hover, sunken);

        int xoffset = shadeButtonRect.width() / 3;
        int yoffset = shadeButtonRect.height() / 3;

        QRect shadeButtonIconRect(shadeButtonRect.left() + xoffset,
                                  shadeButtonRect.top() + yoffset,
                                  shadeButtonRect.width() - xoffset * 2,
                                  shadeButtonRect.height() - yoffset * 2);

        QPainterPath path(shadeButtonIconRect.bottomLeft());
        path.lineTo(shadeButtonIconRect.center().x(),
                    shadeButtonIconRect.bottom() -
                        shadeButtonIconRect.height() / 2);
        path.lineTo(shadeButtonIconRect.bottomRight());
        path.lineTo(shadeButtonIconRect.bottomLeft());

        painter->setPen(textAlphaColor);
        painter->setBrush(textColor);
        painter->drawPath(path);
      }

      // unshade button
      if (titleBar->subControls & SC_TitleBarUnshadeButton) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarUnshadeButton) &&
                     (titleBar->state & State_MouseOver);
        bool sunken =
            (titleBar->activeSubControls & SC_TitleBarUnshadeButton) &&
            (titleBar->state & State_Sunken);

        QRect unshadeButtonRect = proxy()->subControlRect(
            CC_TitleBar, titleBar, SC_TitleBarUnshadeButton, widget);
        PlastikStyleHelper::drawMdiButton(painter, titleBar, unshadeButtonRect,
                                          hover, sunken);

        int xoffset = unshadeButtonRect.width() / 3;
        int yoffset = unshadeButtonRect.height() / 3;

        QRect unshadeButtonIconRect(unshadeButtonRect.left() + xoffset,
                                    unshadeButtonRect.top() + yoffset,
                                    unshadeButtonRect.width() - xoffset * 2,
                                    unshadeButtonRect.height() - yoffset * 2);

        int midY =
            unshadeButtonIconRect.bottom() - unshadeButtonIconRect.height() / 2;
        QPainterPath path(QPoint(unshadeButtonIconRect.left(), midY));
        path.lineTo(unshadeButtonIconRect.right(), midY);
        path.lineTo(unshadeButtonIconRect.center().x(),
                    unshadeButtonIconRect.bottom());
        path.lineTo(unshadeButtonIconRect.left(), midY);

        painter->setPen(textAlphaColor);
        painter->setBrush(textColor);
        painter->drawPath(path);
      }

      // from QCommonStyle.cpp
      if ((titleBar->subControls & SC_TitleBarSysMenu) &&
          (titleBar->titleBarFlags & Qt::WindowSystemMenuHint)) {
        bool hover = (titleBar->activeSubControls & SC_TitleBarSysMenu) &&
                     (titleBar->state & State_MouseOver);
        bool sunken = (titleBar->activeSubControls & SC_TitleBarSysMenu) &&
                      (titleBar->state & State_Sunken);

        QRect iconRect = proxy()->subControlRect(CC_TitleBar, titleBar,
                                                 SC_TitleBarSysMenu, widget);
        if (hover)
          PlastikStyleHelper::drawMdiButton(painter, titleBar, iconRect, hover,
                                            sunken);

        if (!titleBar->icon.isNull()) {
          titleBar->icon.paint(painter, iconRect);
        } else {
          QStyleOption tool(0);
          tool.palette = titleBar->palette;
          QPixmap pm = standardPixmap(SP_TitleBarMenuButton, &tool, widget);
          tool.rect = iconRect;
          painter->save();
          proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pm);
          painter->restore();
        }
      }
      painter->restore();
    }
    break;
#ifndef QT_NO_DIAL
  case CC_Dial:
    if (const QStyleOptionSlider *dial =
            qstyleoption_cast<const QStyleOptionSlider *>(option))
      QStyleHelper::drawDial(dial, painter);
    break;
#endif // QT_NO_DIAL
  default:
    QCommonStyle::drawComplexControl(control, option, painter, widget);
    break;
  }
}

/*!
  \reimp
*/
QSize PlastikStyle::sizeFromContents(ContentsType type,
                                     const QStyleOption *option,
                                     const QSize &size,
                                     const QWidget *widget) const {
  QSize newSize = QCommonStyle::sizeFromContents(type, option, size, widget);
  ;
  switch (type) {
  case QStyle::CT_PushButton:
    if (const QStyleOptionButton *button =
            qstyleoption_cast<const QStyleOptionButton *>(option)) {
      int width = newSize.width();
      int height = newSize.height();

      int defWidth = 0;

      if (button->features & QStyleOptionButton::AutoDefaultButton) {
        defWidth =
            2 * proxy()->pixelMetric(PM_ButtonDefaultIndicator, button, widget);
      }

      const qreal dpi = this->dpi(option);
      int minimumWidth = int(dpiScaled(75, dpi));
      int minimumHeight = int(dpiScaled(23, dpi));

      if (width < minimumWidth + defWidth && !button->text.isEmpty())
        width = minimumWidth + defWidth;
      if (height < minimumHeight + defWidth)
        height = minimumHeight + defWidth;

      newSize = QSize(width, height);
    }
    break;
  case CT_RadioButton:
    ++newSize.rheight();
    ++newSize.rwidth();
    break;
#ifndef QT_NO_SLIDER
  case CT_Slider:
    if (const QStyleOptionSlider *slider =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      int tickSize =
          proxy()->pixelMetric(PM_SliderTickmarkOffset, option, widget);
      if (slider->tickPosition & QSlider::TicksBelow) {
        if (slider->orientation == Qt::Horizontal)
          newSize.rheight() += tickSize;
        else
          newSize.rwidth() += tickSize;
      }
      if (slider->tickPosition & QSlider::TicksAbove) {
        if (slider->orientation == Qt::Horizontal)
          newSize.rheight() += tickSize;
        else
          newSize.rwidth() += tickSize;
      }
    }
    break;
#endif // QT_NO_SLIDER
#ifndef QT_NO_SCROLLBAR
  case CT_ScrollBar:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      int scrollBarExtent =
          proxy()->pixelMetric(PM_ScrollBarExtent, option, widget);
      int scrollBarSliderMinimum =
          proxy()->pixelMetric(PM_ScrollBarSliderMin, option, widget);
      if (scrollBar->orientation == Qt::Horizontal) {
        newSize = QSize(scrollBarExtent * 3 + scrollBarSliderMinimum,
                        scrollBarExtent);
      } else {
        newSize = QSize(scrollBarExtent,
                        scrollBarExtent * 3 + scrollBarSliderMinimum);
      }
    }
    break;
#endif // QT_NO_SCROLLBAR
#ifndef QT_NO_SPINBOX
  case CT_SpinBox:
    // Make sure the size is odd
    newSize.rheight() -= ((1 - newSize.rheight()) & 1);
    break;
#endif
#ifndef QT_NO_TOOLBUTTON
  case CT_ToolButton:
    if (qstyleoption_cast<const QStyleOptionToolButton *>(option))
      newSize += QSize(7, 6);
    newSize.rheight() += 3;
    newSize.rwidth() += 3;
    break;
#endif
#ifndef QT_NO_COMBOBOX
  case CT_ComboBox:
    newSize = sizeFromContents(CT_PushButton, option, size, widget);
    newSize.rwidth() += 30; // Make room for drop-down indicator
    newSize.rheight() += 4;
    break;
#endif
  case CT_MenuItem:
    if (const QStyleOptionMenuItem *menuItem =
            qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
      int width = size.width();
      if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
        newSize = QSize(10, seperatorHeight);
      } else if (menuItem->icon.isNull()) {
        newSize.setHeight(newSize.height() - 2);
        width -= 6;
      }

      if (menuItem->menuItemType != QStyleOptionMenuItem::Separator &&
          !menuItem->icon.isNull()) {
        int iconExtent = proxy()->pixelMetric(PM_SmallIconSize, option, widget);
        newSize.setHeight(qMax(
            newSize.height(),
            menuItem->icon.actualSize(QSize(iconExtent, iconExtent)).height() +
                2 * itemFrameWidth));
      }
      int maxpmw = menuItem->maxIconWidth;
      int tabSpacing = 20;
      if (menuItem->text.contains(u'\t'))
        width += tabSpacing;
      else if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu)
        width += 2 * arrowHMargin;
      else if (menuItem->menuItemType == QStyleOptionMenuItem::DefaultItem) {
        // adjust the font and add the difference in size.
        // it would be better if the font could be adjusted in the
        // initStyleOption qmenu func!!
        QFontMetrics fm(menuItem->font);
        QFont fontBold = menuItem->font;
        fontBold.setBold(true);
        QFontMetrics fmBold(fontBold);
        width += fmBold.horizontalAdvance(menuItem->text) -
                 fm.horizontalAdvance(menuItem->text);
      }

      int checkcol = qMax<int>(
          maxpmw, checkMarkWidth); // Windows always shows a check column
      width += checkcol;
      width += int(rightBorder) + 10;

      newSize.setWidth(width);
      if (menuItem->menuItemType == QStyleOptionMenuItem::Separator)
        newSize.setHeight(
            menuItem->text.isEmpty() ? 2 : menuItem->fontMetrics.height());
    }
    break;
  case CT_MenuBarItem:
    if (!newSize.isEmpty())
      newSize += QSize(itemHMargin * 4, itemVMargin * 2);
    newSize.setHeight(newSize.height());
    break;
  default:
    break;
  }

  return newSize;
}

/*!
  \reimp
*/
QRect PlastikStyle::subElementRect(SubElement element,
                                   const QStyleOption *option,
                                   const QWidget *widget) const {
  QRect rect;
  switch (element) {
  case QStyle::SE_SliderFocusRect:
  case QStyle::SE_ToolBoxTabContents:
    rect = visualRect(option->direction, option->rect, option->rect);
    break;
  case QStyle::SE_DockWidgetTitleBarText: {
    rect = QCommonStyle::subElementRect(element, option, widget);
    const QStyleOptionDockWidget *dockWidget =
        qstyleoption_cast<const QStyleOptionDockWidget *>(option);
    const bool verticalTitleBar = dockWidget && dockWidget->verticalTitleBar;

    int margin = proxy()->pixelMetric(PM_DockWidgetTitleMargin, option, widget);

    if (verticalTitleBar) {
      rect.adjust(0, 0, 0, -margin);
    } else {
      if (option->direction == Qt::LeftToRight) {
        rect.adjust(margin, 0, 0, 0);
      } else {
        rect.adjust(0, 0, -margin, 0);
      }
    }
  } break;
  case SE_RadioButtonIndicator:
    rect = visualRect(option->direction, option->rect,
                      QCommonStyle::subElementRect(element, option, widget))
               .adjusted(0, 0, 1, 1);
    break;
#ifndef QT_NO_PROGRESSBAR
  case SE_ProgressBarLabel:
  case SE_ProgressBarContents:
  case SE_ProgressBarGroove:
    return option->rect;
#endif // QT_NO_PROGRESSBAR
  default:
    return QCommonStyle::subElementRect(element, option, widget);
  }

  return visualRect(option->direction, option->rect, rect);
}

/*!
  \reimp
*/
QRect PlastikStyle::subControlRect(ComplexControl control,
                                   const QStyleOptionComplex *option,
                                   SubControl subControl,
                                   const QWidget *widget) const {
  QRect rect =
      QCommonStyle::subControlRect(control, option, subControl, widget);

  switch (control) {
#ifndef QT_NO_SLIDER
  case CC_Slider:
    if (const QStyleOptionSlider *slider =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      int tickSize =
          proxy()->pixelMetric(PM_SliderTickmarkOffset, option, widget);

      switch (subControl) {
      case SC_SliderHandle:
        if (slider->orientation == Qt::Horizontal) {
          rect.setWidth(11);
          rect.setHeight(15);
          int centerY = slider->rect.center().y() - rect.height() / 2;
          if (slider->tickPosition & QSlider::TicksAbove)
            centerY += tickSize;
          if (slider->tickPosition & QSlider::TicksBelow)
            centerY -= tickSize;
          rect.moveTop(centerY);
        } else {
          rect.setWidth(15);
          rect.setHeight(11);
          int centerX = slider->rect.center().x() - rect.width() / 2;
          if (slider->tickPosition & QSlider::TicksAbove)
            centerX += tickSize;
          if (slider->tickPosition & QSlider::TicksBelow)
            centerX -= tickSize;
          rect.moveLeft(centerX);
        }
        break;
      case SC_SliderGroove: {
        QPoint grooveCenter = slider->rect.center();
        if (slider->orientation == Qt::Horizontal) {
          rect.setHeight(14);
          --grooveCenter.ry();
          if (slider->tickPosition & QSlider::TicksAbove)
            grooveCenter.ry() += tickSize;
          if (slider->tickPosition & QSlider::TicksBelow)
            grooveCenter.ry() -= tickSize;
        } else {
          rect.setWidth(14);
          --grooveCenter.rx();
          if (slider->tickPosition & QSlider::TicksAbove)
            grooveCenter.rx() += tickSize;
          if (slider->tickPosition & QSlider::TicksBelow)
            grooveCenter.rx() -= tickSize;
        }
        rect.moveCenter(grooveCenter);
        break;
      }
      default:
        break;
      }
    }
    break;
#endif // QT_NO_SLIDER
#ifndef QT_NO_SCROLLBAR
  case CC_ScrollBar:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      int scrollBarExtent =
          proxy()->pixelMetric(PM_ScrollBarExtent, scrollBar, widget);
      int sliderMaxLength = ((scrollBar->orientation == Qt::Horizontal)
                                 ? scrollBar->rect.width()
                                 : scrollBar->rect.height()) -
                            (scrollBarExtent * 3);
      int sliderMinLength =
          proxy()->pixelMetric(PM_ScrollBarSliderMin, scrollBar, widget);
      int sliderLength;

      // calculate slider length
      if (scrollBar->maximum != scrollBar->minimum) {
        uint valueRange = scrollBar->maximum - scrollBar->minimum;
        sliderLength = (scrollBar->pageStep * sliderMaxLength) /
                       (valueRange + scrollBar->pageStep);

        if (sliderLength < sliderMinLength || valueRange > INT_MAX / 2)
          sliderLength = sliderMinLength;
        if (sliderLength > sliderMaxLength)
          sliderLength = sliderMaxLength;
      } else {
        sliderLength = sliderMaxLength;
      }

      int sliderStart =
          scrollBarExtent +
          sliderPositionFromValue(
              scrollBar->minimum, scrollBar->maximum, scrollBar->sliderPosition,
              sliderMaxLength - sliderLength, scrollBar->upsideDown);

      QRect scrollBarRect = scrollBar->rect;

      switch (subControl) {
      case SC_ScrollBarSubLine: // top/left button
        if (scrollBar->orientation == Qt::Horizontal) {
          rect.setRect(scrollBarRect.left(), scrollBarRect.top(),
                       scrollBarRect.width() - scrollBarExtent,
                       scrollBarRect.height());
        } else {
          rect.setRect(scrollBarRect.left(), scrollBarRect.top(),
                       scrollBarRect.width(),
                       scrollBarRect.height() - scrollBarExtent);
        }
        break;
      case SC_ScrollBarAddLine: // bottom/right button
        if (scrollBar->orientation == Qt::Horizontal) {
          rect.setRect(scrollBarRect.right() - (scrollBarExtent - 1),
                       scrollBarRect.top(), scrollBarExtent,
                       scrollBarRect.height());
        } else {
          rect.setRect(scrollBarRect.left(),
                       scrollBarRect.bottom() - (scrollBarExtent - 1),
                       scrollBarRect.width(), scrollBarExtent);
        }
        break;
      case SC_ScrollBarSubPage:
        if (scrollBar->orientation == Qt::Horizontal) {
          rect.setRect(scrollBarRect.left() + scrollBarExtent,
                       scrollBarRect.top(),
                       sliderStart - (scrollBarRect.left() + scrollBarExtent),
                       scrollBarRect.height());
        } else {
          rect.setRect(scrollBarRect.left(),
                       scrollBarRect.top() + scrollBarExtent,
                       scrollBarRect.width(),
                       sliderStart - (scrollBarRect.left() + scrollBarExtent));
        }
        break;
      case SC_ScrollBarAddPage:
        if (scrollBar->orientation == Qt::Horizontal)
          rect.setRect(sliderStart + sliderLength, 0,
                       sliderMaxLength - sliderStart - sliderLength +
                           scrollBarExtent,
                       scrollBarRect.height());
        else
          rect.setRect(0, sliderStart + sliderLength, scrollBarRect.width(),
                       sliderMaxLength - sliderStart - sliderLength +
                           scrollBarExtent);
        break;
      case SC_ScrollBarGroove:
        if (scrollBar->orientation == Qt::Horizontal) {
          rect = scrollBarRect.adjusted(scrollBarExtent, 0,
                                        -2 * scrollBarExtent, 0);
        } else {
          rect = scrollBarRect.adjusted(0, scrollBarExtent, 0,
                                        -2 * scrollBarExtent);
        }
        break;
      case SC_ScrollBarSlider:
        if (scrollBar->orientation == Qt::Horizontal) {
          rect.setRect(sliderStart, 0, sliderLength, scrollBarRect.height());
        } else {
          rect.setRect(0, sliderStart, scrollBarRect.width(), sliderLength);
        }
        break;
      default:
        break;
      }
      rect = visualRect(scrollBar->direction, scrollBarRect, rect);
    }
    break;
#endif // QT_NO_SCROLLBAR
#ifndef QT_NO_SPINBOX
  case CC_SpinBox:
    if (const QStyleOptionSpinBox *spinBox =
            qstyleoption_cast<const QStyleOptionSpinBox *>(option)) {
      int center = spinBox->rect.height() / 2;
      switch (subControl) {
      case SC_SpinBoxUp:
        if (spinBox->buttonSymbols == QAbstractSpinBox::NoButtons)
          return QRect();
        rect = visualRect(spinBox->direction, spinBox->rect, rect);
        rect.setRect(spinBox->rect.right() - 16, spinBox->rect.top(), 17,
                     center + 1);
        rect = visualRect(spinBox->direction, spinBox->rect, rect);
        break;
      case SC_SpinBoxDown:
        if (spinBox->buttonSymbols == QAbstractSpinBox::NoButtons)
          return QRect();
        rect = visualRect(spinBox->direction, spinBox->rect, rect);
        rect.setRect(spinBox->rect.right() - 16, spinBox->rect.top() + center,
                     17, spinBox->rect.height() - center);
        rect = visualRect(spinBox->direction, spinBox->rect, rect);
        break;
      case SC_SpinBoxEditField:
        if (spinBox->buttonSymbols != QAbstractSpinBox::NoButtons) {
          rect = spinBox->rect.adjusted(0, 0, -16, 0);
        } else {
          rect = spinBox->rect;
        }
        rect.adjust(blueFrameWidth, blueFrameWidth, -blueFrameWidth,
                    -blueFrameWidth);
        rect = visualRect(spinBox->direction, spinBox->rect, rect);
        break;
      default:
        break;
      }
    }
    break;
#endif // QT_NO_SPINBOX
#ifndef QT_NO_COMBOBOX
  case CC_ComboBox:
    switch (subControl) {
    case SC_ComboBoxArrow:
      rect = visualRect(option->direction, option->rect, rect);
      rect.setRect(rect.right() - 17, rect.top() - 2, 19, rect.height() + 4);
      rect = visualRect(option->direction, option->rect, rect);
      break;
    case SC_ComboBoxEditField: {
      if (const QStyleOptionComboBox *box =
              qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
        int frameWidth = proxy()->pixelMetric(PM_DefaultFrameWidth);
        rect = visualRect(option->direction, option->rect, rect);

        if (box->editable) {
          rect = box->rect.adjusted(blueFrameWidth, blueFrameWidth,
                                    -blueFrameWidth, -blueFrameWidth);
          rect.setRight(rect.right() -
                        16); // Overlaps the combobox button by 2 pixels
        } else {
          rect.setRect(option->rect.left() + frameWidth,
                       option->rect.top() + frameWidth,
                       option->rect.width() - 16 - 2 * frameWidth,
                       option->rect.height() - 2 * frameWidth);
          rect.setLeft(rect.left() + 2);
          rect.setRight(rect.right() - 2);
          if (box->state & (State_Sunken | State_On))
            rect.translate(1, 1);
        }
        rect = visualRect(option->direction, option->rect, rect);
      }
      break;
    }
    default:
      break;
    }
    break;
#endif // QT_NO_COMBOBOX
  case CC_TitleBar:
    if (const QStyleOptionTitleBar *tb =
            qstyleoption_cast<const QStyleOptionTitleBar *>(option)) {
      SubControl sc = subControl;
      QRect &ret = rect;
      const int indent = 3;
      const int controlTopMargin = 4;
      const int controlBottomMargin = 3;
      const int controlWidthMargin = 1;
      const int controlHeight =
          tb->rect.height() - controlTopMargin - controlBottomMargin;
      const int delta = controlHeight + controlWidthMargin;
      int offset = 0;

      bool isMinimized = tb->titleBarState & Qt::WindowMinimized;
      bool isMaximized = tb->titleBarState & Qt::WindowMaximized;

      switch (sc) {
      case SC_TitleBarLabel:
        if (tb->titleBarFlags &
            (Qt::WindowTitleHint | Qt::WindowSystemMenuHint)) {
          ret = tb->rect;
          if (tb->titleBarFlags & Qt::WindowSystemMenuHint)
            ret.adjust(delta, 0, -delta, 0);
          if (tb->titleBarFlags & Qt::WindowMinimizeButtonHint)
            ret.adjust(0, 0, -delta, 0);
          if (tb->titleBarFlags & Qt::WindowMaximizeButtonHint)
            ret.adjust(0, 0, -delta, 0);
          if (tb->titleBarFlags & Qt::WindowShadeButtonHint)
            ret.adjust(0, 0, -delta, 0);
          if (tb->titleBarFlags & Qt::WindowContextHelpButtonHint)
            ret.adjust(0, 0, -delta, 0);
          ret = ret.adjusted(indent, 0, -indent, 0);
        }
        break;
      case SC_TitleBarContextHelpButton:
        if (tb->titleBarFlags & Qt::WindowContextHelpButtonHint)
          offset += delta;
      case SC_TitleBarMinButton:
        if (!isMinimized && (tb->titleBarFlags & Qt::WindowMinimizeButtonHint))
          offset += delta;
        else if (sc == SC_TitleBarMinButton)
          break;
      case SC_TitleBarNormalButton:
        if (isMinimized && (tb->titleBarFlags & Qt::WindowMinimizeButtonHint))
          offset += delta;
        else if (isMaximized &&
                 (tb->titleBarFlags & Qt::WindowMaximizeButtonHint))
          offset += delta;
        else if (sc == SC_TitleBarNormalButton)
          break;
      case SC_TitleBarMaxButton:
        if (!isMaximized && (tb->titleBarFlags & Qt::WindowMaximizeButtonHint))
          offset += delta;
        else if (sc == SC_TitleBarMaxButton)
          break;
      case SC_TitleBarShadeButton:
        if (!isMinimized && (tb->titleBarFlags & Qt::WindowShadeButtonHint))
          offset += delta;
        else if (sc == SC_TitleBarShadeButton)
          break;
      case SC_TitleBarUnshadeButton:
        if (isMinimized && (tb->titleBarFlags & Qt::WindowShadeButtonHint))
          offset += delta;
        else if (sc == SC_TitleBarUnshadeButton)
          break;
      case SC_TitleBarCloseButton:
        if (tb->titleBarFlags & Qt::WindowSystemMenuHint)
          offset += delta;
        else if (sc == SC_TitleBarCloseButton)
          break;
        ret.setRect(tb->rect.right() - indent - offset,
                    tb->rect.top() + controlTopMargin, controlHeight,
                    controlHeight);
        break;
      case SC_TitleBarSysMenu:
        if (tb->titleBarFlags & Qt::WindowSystemMenuHint) {
          ret.setRect(tb->rect.left() + controlWidthMargin + indent,
                      tb->rect.top() + controlTopMargin, controlHeight,
                      controlHeight);
        }
        break;
      default:
        break;
      }
      ret = visualRect(tb->direction, tb->rect, ret);
    }
    break;
  default:
    break;
  }

  return rect;
}

/*!
  \reimp
*/
int PlastikStyle::styleHint(StyleHint hint, const QStyleOption *option,
                            const QWidget *widget,
                            QStyleHintReturn *returnData) const {
  int ret = 0;
  switch (hint) {
  case SH_EtchDisabledText: {
    const QPalette pal =
        option   ? option->palette
        : widget ? widget->palette()
                 : QPalette(); // use available palette, or default palette
    ret = (pal.window().color().lightness() > pal.text().color().lightness())
              ? 1
              : 0;
    break;
  }

  case SH_Slider_SnapToValue:
  case SH_PrintDialog_RightAlignButtons:
  case SH_FontDialog_SelectAssociatedText:
  case SH_MenuBar_AltKeyNavigation:
  case SH_MenuBar_MouseTracking:
  case SH_Menu_MouseTracking:
  case SH_ComboBox_ListMouseTracking:
  case SH_Slider_StopMouseOverSlider:
  case SH_Menu_AllowActiveAndDisabled:
    ret = 1;
    break;
  case SH_WindowFrame_Mask:
    ret = 1;
    if (QStyleHintReturnMask *mask =
            qstyleoption_cast<QStyleHintReturnMask *>(returnData)) {
      mask->region = option->rect;
      mask->region -= QRect(option->rect.left(), option->rect.top(), 2, 1);
      mask->region -= QRect(option->rect.right() - 1, option->rect.top(), 2, 1);
      mask->region -= QRect(option->rect.left(), option->rect.top() + 1, 1, 1);
      mask->region -= QRect(option->rect.right(), option->rect.top() + 1, 1, 1);

      const QStyleOptionTitleBar *titleBar =
          qstyleoption_cast<const QStyleOptionTitleBar *>(option);
      if (titleBar && (titleBar->titleBarState & Qt::WindowMinimized)) {
        mask->region -= QRect(option->rect.left(), option->rect.bottom(), 2, 1);
        mask->region -=
            QRect(option->rect.right() - 1, option->rect.bottom(), 2, 1);
        mask->region -=
            QRect(option->rect.left(), option->rect.bottom() - 1, 1, 1);
        mask->region -=
            QRect(option->rect.right(), option->rect.bottom() - 1, 1, 1);
      } else {
        mask->region -= QRect(option->rect.bottomLeft(), QSize(1, 1));
        mask->region -= QRect(option->rect.bottomRight(), QSize(1, 1));
      }
    }
    break;
  case SH_TitleBar_NoBorder:
    ret = 1;
    break;
  case SH_TitleBar_AutoRaise:
    ret = 1;
    break;
  case SH_ItemView_ShowDecorationSelected:
    ret = true;
    break;
  case SH_ItemView_ChangeHighlightOnFocus:
    ret = 1;
    break;
  case SH_ToolBox_SelectedPageTitleBold:
  case SH_ScrollBar_MiddleClickAbsolutePosition:
    ret = true;
    break;
  case SH_MainWindow_SpaceBelowMenuBar:
    ret = 0;
    break;
  case SH_FormLayoutWrapPolicy:
    ret = QFormLayout::DontWrapRows;
    break;
  case SH_FormLayoutFieldGrowthPolicy:
    ret = QFormLayout::ExpandingFieldsGrow;
    break;
  case SH_FormLayoutFormAlignment:
    ret = Qt::AlignLeft | Qt::AlignTop;
    break;
  case SH_FormLayoutLabelAlignment:
    ret = Qt::AlignRight;
    break;
  case SH_MessageBox_TextInteractionFlags:
    ret = Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse;
    break;
  case SH_LineEdit_PasswordCharacter:
    ret = QCommonStyle::styleHint(hint, option, widget, returnData);
    break;
  case SH_ItemView_ArrowKeysNavigateIntoChildren:
    ret = true;
    break;
  case SH_Menu_SubMenuSloppyCloseTimeout:
    ret = 400;
    break;
  case SH_Menu_SubMenuPopupDelay:
    ret = 96; // from Plastik
    break;
  case SH_DialogButtonBox_ButtonsHaveIcons:
    // do not get this from stylehints, to avoid private headers
    ret = true;
    break;
  case SH_RubberBand_Mask: {
    if (const QStyleOptionRubberBand *rubberBand =
            qstyleoption_cast<const QStyleOptionRubberBand *>(option)) {
      ret = 0;
      if (rubberBand->shape == QRubberBand::Rectangle) {
        ret = true;
        if (QStyleHintReturnMask *mask =
                qstyleoption_cast<QStyleHintReturnMask *>(returnData)) {
          mask->region = option->rect;
          int size = 1;
          if (widget && widget->isWindow()) {
            size = 4;
          }
          mask->region -= option->rect.adjusted(size, size, -size, -size);
        }
      }
    }
    break;
  }
  case SH_WizardStyle:
    ret = QWizard::ModernStyle;

  default:
    ret = QCommonStyle::styleHint(hint, option, widget, returnData);
    break;
  }
  return ret;
}

/*!
  \reimp
*/
QStyle::SubControl PlastikStyle::hitTestComplexControl(
    ComplexControl control, const QStyleOptionComplex *option,
    const QPoint &pos, const QWidget *widget) const {
  SubControl ret = SC_None;
  switch (control) {
#ifndef QT_NO_SCROLLBAR
  case CC_ScrollBar:
    if (const QStyleOptionSlider *scrollBar =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      QRect slider = proxy()->subControlRect(control, scrollBar,
                                             SC_ScrollBarSlider, widget);
      if (slider.contains(pos)) {
        ret = SC_ScrollBarSlider;
        break;
      }

      QRect scrollBarAddLine = proxy()->subControlRect(
          control, scrollBar, SC_ScrollBarAddLine, widget);
      if (scrollBarAddLine.contains(pos)) {
        ret = SC_ScrollBarAddLine;
        break;
      }

      QRect scrollBarSubPage = proxy()->subControlRect(
          control, scrollBar, SC_ScrollBarSubPage, widget);
      if (scrollBarSubPage.contains(pos)) {
        ret = SC_ScrollBarSubPage;
        break;
      }

      QRect scrollBarAddPage = proxy()->subControlRect(
          control, scrollBar, SC_ScrollBarAddPage, widget);
      if (scrollBarAddPage.contains(pos)) {
        ret = SC_ScrollBarAddPage;
        break;
      }

      QRect scrollBarSubLine = proxy()->subControlRect(
          control, scrollBar, SC_ScrollBarSubLine, widget);
      if (scrollBarSubLine.contains(pos)) {
        ret = SC_ScrollBarSubLine;
        break;
      }
    }
    break;
#endif // QT_NO_SCROLLBAR
  default:
    break;
  }

  return ret != SC_None ? ret
                        : QCommonStyle::hitTestComplexControl(control, option,
                                                              pos, widget);
}

/*!
  \reimp
*/
int PlastikStyle::pixelMetric(PixelMetric metric, const QStyleOption *option,
                              const QWidget *widget) const {
  QT_END_NAMESPACE
  int ret = -1;
  switch (metric) {
  case PM_MenuVMargin:
  case PM_MenuHMargin:
    ret = 0;
    break;
  case PM_ButtonShiftHorizontal:
  case PM_ButtonShiftVertical:
    ret = 1;
    break;
  case PM_ButtonDefaultIndicator:
    ret = 0;
    break;
#ifndef QT_NO_SLIDER
  case PM_SliderThickness:
    ret = 15;
    break;
  case PM_SliderLength:
  case PM_SliderControlThickness:
    ret = 11;
    break;
  case PM_SliderTickmarkOffset:
    ret = 5;
    break;
  case PM_SliderSpaceAvailable:
    if (const QStyleOptionSlider *slider =
            qstyleoption_cast<const QStyleOptionSlider *>(option)) {
      int size = 15;
      if (slider->tickPosition & QSlider::TicksBelow)
        ++size;
      if (slider->tickPosition & QSlider::TicksAbove)
        ++size;
      ret = size;
      break;
    }
#endif // QT_NO_SLIDER
  case PM_ScrollBarExtent:
    ret = 16;
    break;
  case PM_ScrollBarSliderMin:
    ret = 26;
    break;
  case PM_ProgressBarChunkWidth:
    ret = 1;
    break;
  case PM_MenuBarItemSpacing:
    ret = 3;
    break;
  case PM_MenuBarVMargin:
    ret = 2;
    break;
  case PM_MenuBarHMargin:
    ret = 0;
    break;
  case PM_MenuBarPanelWidth:
    ret = 1;
    break;
  case PM_SmallIconSize:
    ret = 16;
    break;
  case PM_LargeIconSize:
    ret = 32;
    break;
  case PM_ToolBarHandleExtent:
    ret = 9;
    break;
  case PM_ToolBarSeparatorExtent:
    ret = 2;
    break;
  case PM_ToolBarItemSpacing:
    ret = 1;
    break;
  case PM_ToolBarItemMargin:
    ret = 1;
    break;
  case PM_ToolBarFrameWidth:
    ret = 2;
    break;
  case PM_SplitterWidth:
    ret = 6;
    break;
  case PM_DockWidgetSeparatorExtent:
    ret = 6;
    break;
  case PM_DockWidgetHandleExtent:
    ret = 20;
    break;
  case PM_DefaultFrameWidth:
#ifndef QT_NO_MENU
    if (qobject_cast<const QMenu *>(widget)) {
      ret = 1;
      break;
    }
#endif
    ret = 2;
    break;
#ifndef QT_NO_TABBAR
  case PM_TabBarTabShiftHorizontal:
    ret = 0;
    break;
  case PM_TabBarTabShiftVertical:
    ret = 2;
    break;
#endif
  case PM_MdiSubWindowFrameWidth:
    ret = 4;
    break;
  case PM_TitleBarHeight:
    ret = qMax(widget ? widget->fontMetrics().height()
                      : (option ? option->fontMetrics.height() : 0),
               30);
    break;
  case PM_MaximumDragDistance:
    ret = QCommonStyle::pixelMetric(PM_MaximumDragDistance, option, widget);
    if (ret == -1) {
      ret = 60;
    }
    return ret; // don't scale
                // make sure it is not negative
  case PM_IconViewIconSize:
    proxy()->pixelMetric(PM_LargeIconSize, option, widget);
    break;
  case PM_DockWidgetTitleMargin:
    ret = 2;
  case PM_DockWidgetTitleBarButtonMargin:
  case PM_DockWidgetFrameWidth:
    ret = 4;
    break;
  case PM_LayoutHorizontalSpacing:
  case PM_LayoutVerticalSpacing:
    return -1; // rely on layoutHorizontalSpacing()
  case PM_LayoutLeftMargin:
  case PM_LayoutTopMargin:
  case PM_LayoutRightMargin:
  case PM_LayoutBottomMargin: {
    bool isWindow = false;
    if (option) {
      isWindow = (option->state & State_Window);
    } else if (widget) {
      isWindow = widget->isWindow();
    }

    if (isWindow) {
      ret = 11;
    } else {
      ret = 9;
    }
  }
  default:
    return QCommonStyle::pixelMetric(metric, option, widget);
  }
  if (ret < 0)
    return QCommonStyle::pixelMetric(metric, option, widget);
  ret = dpiScaled(ret, option);
  return ret;
}

/*!
  \reimp
*/
QPalette PlastikStyle::standardPalette() const {
  QPalette palette;

  palette.setBrush(QPalette::Disabled, QPalette::WindowText,
                   QColor(QRgb(0xff808080)));
  palette.setBrush(QPalette::Disabled, QPalette::Button,
                   QColor(QRgb(0xffdddfe4)));
  palette.setBrush(QPalette::Disabled, QPalette::Light,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Disabled, QPalette::Midlight,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Disabled, QPalette::Dark,
                   QColor(QRgb(0xff555555)));
  palette.setBrush(QPalette::Disabled, QPalette::Mid, QColor(QRgb(0xffc7c7c7)));
  palette.setBrush(QPalette::Disabled, QPalette::Text,
                   QColor(QRgb(0xffc7c7c7)));
  palette.setBrush(QPalette::Disabled, QPalette::BrightText,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Disabled, QPalette::ButtonText,
                   QColor(QRgb(0xff808080)));
  palette.setBrush(QPalette::Disabled, QPalette::Base,
                   QColor(QRgb(0xffefefef)));
  palette.setBrush(
      QPalette::Disabled, QPalette::AlternateBase,
      palette.color(QPalette::Disabled, QPalette::Base).darker(110));
  palette.setBrush(QPalette::Disabled, QPalette::Window,
                   QColor(QRgb(0xffefefef)));
  palette.setBrush(QPalette::Disabled, QPalette::Shadow,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Disabled, QPalette::Highlight,
                   QColor(QRgb(0xff567594)));
  palette.setBrush(QPalette::Disabled, QPalette::HighlightedText,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Disabled, QPalette::Link,
                   QColor(QRgb(0xff0000ee)));
  palette.setBrush(QPalette::Disabled, QPalette::LinkVisited,
                   QColor(QRgb(0xff52188b)));
  palette.setBrush(QPalette::Active, QPalette::WindowText,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Active, QPalette::Button,
                   QColor(QRgb(0xffdddfe4)));
  palette.setBrush(QPalette::Active, QPalette::Light, QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Active, QPalette::Midlight,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Active, QPalette::Dark, QColor(QRgb(0xff555555)));
  palette.setBrush(QPalette::Active, QPalette::Mid, QColor(QRgb(0xffc7c7c7)));
  palette.setBrush(QPalette::Active, QPalette::Text, QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Active, QPalette::BrightText,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Active, QPalette::ButtonText,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Active, QPalette::Base, QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Active, QPalette::AlternateBase,
                   palette.color(QPalette::Active, QPalette::Base).darker(110));
  palette.setBrush(QPalette::Active, QPalette::Window,
                   QColor(QRgb(0xffefefef)));
  palette.setBrush(QPalette::Active, QPalette::Shadow,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Active, QPalette::Highlight,
                   QColor(QRgb(0xff678db2)));
  palette.setBrush(QPalette::Active, QPalette::HighlightedText,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Active, QPalette::Link, QColor(QRgb(0xff0000ee)));
  palette.setBrush(QPalette::Active, QPalette::LinkVisited,
                   QColor(QRgb(0xff52188b)));
  palette.setBrush(QPalette::Inactive, QPalette::WindowText,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Inactive, QPalette::Button,
                   QColor(QRgb(0xffdddfe4)));
  palette.setBrush(QPalette::Inactive, QPalette::Light,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Inactive, QPalette::Midlight,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Inactive, QPalette::Dark,
                   QColor(QRgb(0xff555555)));
  palette.setBrush(QPalette::Inactive, QPalette::Mid, QColor(QRgb(0xffc7c7c7)));
  palette.setBrush(QPalette::Inactive, QPalette::Text,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Inactive, QPalette::BrightText,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Inactive, QPalette::ButtonText,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Inactive, QPalette::Base,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(
      QPalette::Inactive, QPalette::AlternateBase,
      palette.color(QPalette::Inactive, QPalette::Base).darker(110));
  palette.setBrush(QPalette::Inactive, QPalette::Window,
                   QColor(QRgb(0xffefefef)));
  palette.setBrush(QPalette::Inactive, QPalette::Shadow,
                   QColor(QRgb(0xff000000)));
  palette.setBrush(QPalette::Inactive, QPalette::Highlight,
                   QColor(QRgb(0xff678db2)));
  palette.setBrush(QPalette::Inactive, QPalette::HighlightedText,
                   QColor(QRgb(0xffffffff)));
  palette.setBrush(QPalette::Inactive, QPalette::Link,
                   QColor(QRgb(0xff0000ee)));
  palette.setBrush(QPalette::Inactive, QPalette::LinkVisited,
                   QColor(QRgb(0xff52188b)));
  return palette;
}

/*!
  \reimp
*/
void PlastikStyle::polish(QWidget *widget) {
  if (qobject_cast<QPushButton *>(widget)
#ifndef QT_NO_COMBOBOX
      || qobject_cast<QComboBox *>(widget)
#endif
#ifndef QT_NO_SPINBOX
      || qobject_cast<QAbstractSpinBox *>(widget)
#endif
      || qobject_cast<QCheckBox *>(widget)
#ifndef QT_NO_GROUPBOX
      || qobject_cast<QGroupBox *>(widget)
#endif
      || qobject_cast<QRadioButton *>(widget)
#ifndef QT_NO_SPLITTER
      || qobject_cast<QSplitterHandle *>(widget)
#endif
#ifndef QT_NO_TABBAR
      || qobject_cast<QTabBar *>(widget)
#endif
#ifndef QT_NO_SLIDER // make slider handle be hoverable necessary for accenting
                     // hovered slider handle
      || qobject_cast<QSlider *>(widget)
#endif
  ) {
    widget->setAttribute(Qt::WA_Hover);
  }

  QT_END_NAMESPACE
  if (widget->inherits("QDockSeparator") ||
      widget->inherits("QDockWidgetSeparator")) {
    widget->setAttribute(Qt::WA_Hover);
  }

  if (false // to simplify the #ifdefs
#ifndef QT_NO_MENUBAR
      || qobject_cast<QMenuBar *>(widget)
#endif
#ifndef QT_NO_TOOLBAR
      || qobject_cast<QToolBar *>(widget) ||
      (widget && qobject_cast<QToolBar *>(widget->parent()))
#endif
  ) {
    widget->setBackgroundRole(QPalette::Window);
  }

#ifndef QT_NO_PROGRESSBAR
  if (AnimateBusyProgressBar && qobject_cast<QProgressBar *>(widget))
    widget->installEventFilter(this);
#endif

#if defined QPlastique_MaskButtons
  if (qobject_cast<QPushButton *>(widget) ||
      qobject_cast<QToolButton *>(widget))
    widget->installEventFilter(this);
#endif
}

/*!
  \reimp
*/
void PlastikStyle::unpolish(QWidget *widget) {
  if (qobject_cast<QPushButton *>(widget)
#ifndef QT_NO_COMBOBOX
      || qobject_cast<QComboBox *>(widget)
#endif
#ifndef QT_NO_SPINBOX
      || qobject_cast<QAbstractSpinBox *>(widget)
#endif
      || qobject_cast<QCheckBox *>(widget)
#ifndef QT_NO_GROUPBOX
      || qobject_cast<QGroupBox *>(widget)
#endif
#ifndef QT_NO_SPLITTER
      || qobject_cast<QSplitterHandle *>(widget)
#endif
#ifndef QT_NO_TABBAR
      || qobject_cast<QTabBar *>(widget)
#endif
      || qobject_cast<QRadioButton *>(widget)) {
    widget->setAttribute(Qt::WA_Hover, false);
  }

#ifndef QT_NO_SLIDER
  if (qobject_cast<QSlider *>(widget)) {
    widget->setAttribute(Qt::WA_Hover, true);
  }
#endif
  if (widget->inherits("QDockSeparator") ||
      widget->inherits("QDockWidgetSeparator")) {
    widget->setAttribute(Qt::WA_Hover, false);
  }

  if (false // to simplify the #ifdefs
#ifndef QT_NO_MENUBAR
      || qobject_cast<QMenuBar *>(widget)
#endif
#ifndef QT_NO_TOOLBOX
      || qobject_cast<QToolBox *>(widget)
#endif
#ifndef QT_NO_TOOLBAR
      || qobject_cast<QToolBar *>(widget) ||
      (widget && qobject_cast<QToolBar *>(widget->parent()))
#endif
  ) {
    widget->setBackgroundRole(QPalette::Button);
  }

#ifndef QT_NO_PROGRESSBAR
  if (AnimateBusyProgressBar && qobject_cast<QProgressBar *>(widget)) {
    widget->removeEventFilter(this);
    bars.removeOne(static_cast<QProgressBar *>(widget));
  }
#endif

#if defined QPlastique_MaskButtons
  if (qobject_cast<QPushButton *>(widget) ||
      qobject_cast<QToolButton *>(widget))
    widget->removeEventFilter(this);
#endif
}

/*!
  \reimp
*/
void PlastikStyle::polish(QApplication *app) {
  QCommonStyle::polish(app);
  if (!proxy()->styleHint(SH_UnderlineShortcut, nullptr) && app) {
    app->installEventFilter(this);
  }

  const auto &palette = QGuiApplication::palette();
  activeGradientCaptionColor = palette.highlight().color();
  activeCaptionColor = activeGradientCaptionColor;
  inactiveGradientCaptionColor = palette.dark().color();
  inactiveCaptionColor = inactiveGradientCaptionColor;
  inactiveCaptionText = palette.window().color();
}

/*!
QT_END_NAMESPACE
  \reimp
*/
void PlastikStyle::polish(QPalette &pal) {
  QCommonStyle::polish(pal);
#ifdef Q_WS_MAC
  pal.setBrush(QPalette::Shadow, Qt::black);
#endif
}

/*!
  \reimp
*/
void PlastikStyle::unpolish(QApplication *app) { QCommonStyle::unpolish(app); }

/*!
    \reimp
*/
QIcon PlastikStyle::standardIcon(StandardPixmap standardIcon,
                                 const QStyleOption *option,
                                 const QWidget *widget) const {
  return QCommonStyle::standardIcon(standardIcon, option, widget);
}

/*!
    \reimp
*/
QPixmap PlastikStyle::standardPixmap(StandardPixmap standardPixmap,
                                     const QStyleOption *opt,
                                     const QWidget *widget) const {
  return QCommonStyle::standardPixmap(standardPixmap, opt, widget);
}

// this works as long as we have at most 16 different control types
#define CT1(c) CT2(c, c)
#define CT2(c1, c2) (((uint)c1 << 16) | (uint)c2)

/*!
    \reimp
*/
int PlastikStyle::layoutSpacing(QSizePolicy::ControlType control1,
                                QSizePolicy::ControlType control2,
                                Qt::Orientation orientation,
                                const QStyleOption * /* option */,
                                const QWidget * /* widget */) const {
  const int ButtonMask = QSizePolicy::ButtonBox | QSizePolicy::PushButton;

  if (control2 == QSizePolicy::ButtonBox)
    return 11;

  if ((control1 | control2) & ButtonMask)
    return (orientation == Qt::Horizontal) ? 10 : 9;

  switch (CT2(control1, control2)) {
  case CT1(QSizePolicy::Label):
  case CT2(QSizePolicy::Label, QSizePolicy::DefaultType):
  case CT2(QSizePolicy::Label, QSizePolicy::CheckBox):
  case CT2(QSizePolicy::Label, QSizePolicy::ComboBox):
  case CT2(QSizePolicy::Label, QSizePolicy::LineEdit):
  case CT2(QSizePolicy::Label, QSizePolicy::RadioButton):
  case CT2(QSizePolicy::Label, QSizePolicy::Slider):
  case CT2(QSizePolicy::Label, QSizePolicy::SpinBox):
  case CT2(QSizePolicy::Label, QSizePolicy::ToolButton):
    return 5;
  case CT2(QSizePolicy::CheckBox, QSizePolicy::RadioButton):
  case CT2(QSizePolicy::RadioButton, QSizePolicy::CheckBox):
  case CT1(QSizePolicy::CheckBox):
    if (orientation == Qt::Vertical)
      return 2;
  case CT1(QSizePolicy::RadioButton):
    if (orientation == Qt::Vertical)
      return 1;
  }

  if (orientation == Qt::Horizontal &&
      (control2 & (QSizePolicy::CheckBox | QSizePolicy::RadioButton)))
    return 8;

  if ((control1 | control2) &
      (QSizePolicy::Frame | QSizePolicy::GroupBox | QSizePolicy::TabWidget)) {
    return 11;
  }

  if ((control1 | control2) &
      (QSizePolicy::Line | QSizePolicy::Slider | QSizePolicy::LineEdit |
       QSizePolicy::ComboBox | QSizePolicy::SpinBox))
    return 7;

  return 6;
}

/*!
    \reimp
*/
bool PlastikStyle::eventFilter(QObject *watched, QEvent *event) {
  if (!watched->isWidgetType()) {
    return QObject::eventFilter(watched, event);
  }

  QWidget *widget = qobject_cast<QWidget *>(watched);

#ifndef QT_NO_PROGRESSBAR
  switch (event->type()) {
  case QEvent::KeyPress: // if alt is pressed  add any widget that is not a
                         // window, visible or if stylehing SH_UnderlineShortcut
                         // returns something
    if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Alt) {
      widget = widget->window();

      const QList<QWidget *> children = widget->findChildren<QWidget *>();
      auto ignorable = [](QWidget *w) {
        return w->isWindow() || !w->isVisible() ||
               w->style()->styleHint(SH_UnderlineShortcut, nullptr, w);
      };
      seenAlt.append(widget);
      alt_down = true;
      for (QWidget *w : children) {
        if (!ignorable(w)) {
          w->update();
        }
      }
    }
    break;
  case QEvent::KeyRelease:
    if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Alt) {
      widget = widget->window();
      alt_down = false;

#ifndef QT_NO_MENUBAR
      const QList<QMenuBar *> menuBars = widget->findChildren<QMenuBar *>();
      for (QWidget *w : menuBars) {
        w->update();
      }
#endif
    }
    break;
  case QEvent::Close:
    // Reset widget when closing
    seenAlt.removeAll(widget);
    seenAlt.removeAll(widget->window());
    break;
  case QEvent::StyleChange:
  case QEvent::Paint:
  case QEvent::Show:
    if (QProgressBar *bar = qobject_cast<QProgressBar *>(watched)) {
      // Animation by timer for progress bars that have their min and
      // max values the same and if progressbars should be animated in general
      if ((bar->minimum() == bar->maximum() && AnimateBusyProgressBar) ||
          AnimateProgressBar) {
        if (bar->value() == bar->maximum() &&
            !(bar->minimum() ==
              bar->maximum())) // We need to stop animating if the max value has
                               // been reached
        {
          stopProgressAnimation(reinterpret_cast<QProgressBar *>(watched));
        } else {
          startProgressAnimation(reinterpret_cast<QProgressBar *>(watched));
        }
      } else {
        stopProgressAnimation(reinterpret_cast<QProgressBar *>(watched));
      }
    }
    break;
  case QEvent::Destroy:
  case QEvent::Hide:
    stopProgressAnimation(reinterpret_cast<QProgressBar *>(watched));
    break;
#if defined QPlastique_MaskButtons
  case QEvent::Resize:
    if (qobject_cast<QPushButton *>(watched) ||
        qobject_cast<QToolButton *>(watched)) {
      QWidget *widget = qobject_cast<QWidget *>(watched);
      QRect rect = widget->rect();
      QRegion region(rect);
      region -= QRect(rect.left(), rect.top(), 2, 1);
      region -= QRect(rect.left(), rect.top() + 1, 1, 1);
      region -= QRect(rect.left(), rect.bottom(), 2, 1);
      region -= QRect(rect.left(), rect.bottom() - 1, 1, 1);
      region -= QRect(rect.right() - 1, rect.top(), 2, 1);
      region -= QRect(rect.right(), rect.top() + 1, 1, 1);
      region -= QRect(rect.right() - 1, rect.bottom(), 2, 1);
      region -= QRect(rect.right(), rect.bottom() - 1, 1, 1);
      widget->setMask(region);
    }
    break;
#endif
  default:
    break;
  }
#endif // QT_NO_PROGRESSBAR

  return QCommonStyle::eventFilter(watched, event);
}

/*!
    \reimp
*/
bool PlastikStyle::event(QEvent *event) {
  switch (event->type()) {
  case QEvent::Timer: {
#ifndef QT_NO_PROGRESSBAR
    QTimerEvent *timerEvent = reinterpret_cast<QTimerEvent *>(event);
    if (timerEvent->timerId() == progressBarAnimateTimer) {
      Q_ASSERT(ProgressBarFps > 0);
      animateStep = timer.elapsed() / (1000 / ProgressBarFps);
      foreach (QProgressBar *bar, bars) {
        if (AnimateProgressBar || (bar->minimum() == 0 && bar->maximum() == 0))
          bar->update();
      }
    }
#endif // QT_NO_PROGRESSBAR
    event->ignore();
  }
  default:
    break;
  }

  return QCommonStyle::event(event);
}

void PlastikStyle::startProgressAnimation(QProgressBar *bar) {
  if (!bars.contains(bar)) {
    bars << bar;
    if (bars.size() == 1) {
      Q_ASSERT(ProgressBarFps > 0);
      animateStep = 0;
      timer.start();
      progressBarAnimateTimer = startTimer(1000 / ProgressBarFps);
    }
  }
}

void PlastikStyle::stopProgressAnimation(QProgressBar *bar) {
  if (!bars.isEmpty()) {
    bars.removeOne(bar);
    if (bars.isEmpty() && progressBarAnimateTimer) {
      killTimer(progressBarAnimateTimer);
      progressBarAnimateTimer = 0;
    }
  }
}

// helper function, needs only be called where alphaTextColor was previously
// used
QColor PlastikStyle::calcAlphaTextColor(const QStyleOption *option) const {
  return PlastikStyleHelper::mergeColors(option->palette.window().color(),
                                         option->palette.text().color());
}

QColor PlastikStyle::calcAlphaCornerColor(const QStyleOption *option,
                                          const QWidget *widget) const {
  QColor borderColor = option->palette.window().color().darker(178);
  QColor alphaCornerColor;
  if (widget) {
    // ### backgroundrole/foregroundrole should be part of the style option
    alphaCornerColor = PlastikStyleHelper::mergeColors(
        option->palette.color(widget->backgroundRole()), borderColor);
  } else {
    alphaCornerColor = PlastikStyleHelper::mergeColors(
        option->palette.window().color(), borderColor);
  }
  return alphaCornerColor;
}
qreal PlastikStyle::defaultDPI() const {
  return QGuiApplication::primaryScreen()
      ->logicalDotsPerInchX(); // might be another function
}
qreal PlastikStyle::dpiScaled(qreal toScale, qreal dpi) const {
  return toScale * dpi / baseDPI;
}
qreal PlastikStyle::dpiScaled(qreal toScale, const QStyleOption *option) const {
  qreal dpi = defaultDPI();
  if (option) {
    dpi = option->fontMetrics.fontDpi();
  }
  return dpiScaled(toScale, dpi);
}

qreal PlastikStyle::dpiScaled(qreal toScale, const QPaintDevice *device) const {
  return dpiScaled(toScale, device->logicalDpiX());
}

qreal PlastikStyle::dpi(const QStyleOption *option) const {
  if (QCoreApplication::testAttribute(Qt::AA_Use96Dpi)) {
    return 96;
  }

  if (option)
    return option->fontMetrics.fontDpi();

  return defaultDPI(); // fallback, if no option is given
}
bool PlastikStyle::hasSeenAlt(const QWidget *widget) const {
  widget = widget->window();
  return seenAlt.contains(widget);
}
