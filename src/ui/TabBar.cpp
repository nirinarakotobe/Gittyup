//
//          Copyright (c) 2016, Scientific Toolworks, Inc.
//
// This software is licensed under the MIT License. The LICENSE.md file
// describes the conditions under which this software may be distributed.
//
// Author: Jason Haslam
//

#include "TabBar.h"

#include <QStyle>
#include <QToolButton>

TabBar::TabBar(QWidget *parent) : QTabBar(parent) {
  setAutoHide(true);
  setDocumentMode(true);
}

void TabBar::tabInserted(int index) {
  QTabBar::tabInserted(index);

  QToolButton *button = new QToolButton(this);
  button->setAutoRaise(true);
  button->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
  button->setIconSize(QSize(10, 10));
  button->setFixedSize(22, 16);
  button->setFocusPolicy(Qt::NoFocus);
  button->setStyleSheet("QToolButton { padding-left: 6px; }");
  button->setToolTip(tr("Close Tab"));
  connect(button, &QToolButton::clicked, this, [this, button] {
    for (int i = 0; i < count(); ++i) {
      if (tabButton(i, QTabBar::LeftSide) == button) {
        emit closeTabRequested(i);
        return;
      }
    }
  });

  setTabButton(index, QTabBar::LeftSide, button);
}

QSize TabBar::minimumTabSizeHint(int index) const {
  mCalculatingMinimumSize = true;
  QSize size = QTabBar::minimumTabSizeHint(index);
  mCalculatingMinimumSize = false;
  return size;
}

QSize TabBar::tabSizeHint(int index) const {
  if (!count() || mCalculatingMinimumSize)
    return QTabBar::tabSizeHint(index);

  int height = fontMetrics().lineSpacing() + 12;
  return QSize(parentWidget()->width() / count() + 1, height);
}
