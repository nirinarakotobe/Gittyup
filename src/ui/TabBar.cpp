//
//          Copyright (c) 2016, Scientific Toolworks, Inc.
//
// This software is licensed under the MIT License. The LICENSE.md file
// describes the conditions under which this software may be distributed.
//
// Author: Jason Haslam
//

#include "TabBar.h"

#include <QAbstractButton>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

namespace {

class CloseButton : public QAbstractButton {
public:
  CloseButton(QWidget *parent = nullptr) : QAbstractButton(parent) {
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::NoFocus);
    setToolTip(tr("Close Tab"));
  }

  QSize sizeHint() const override {
    return QSize(style()->pixelMetric(QStyle::PM_TabCloseIndicatorWidth),
                 style()->pixelMetric(QStyle::PM_TabCloseIndicatorHeight));
  }

protected:
  void enterEvent(QEnterEvent *event) override {
    QAbstractButton::enterEvent(event);
    update();
  }

  void leaveEvent(QEvent *event) override {
    QAbstractButton::leaveEvent(event);
    update();
  }

  void paintEvent(QPaintEvent *event) override {
    Q_UNUSED(event)

    QStyleOption option;
    option.initFrom(this);
    option.rect = rect();
    if (underMouse()) {
      option.state |= QStyle::State_MouseOver;
      option.state |= QStyle::State_Selected;
    }

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_IndicatorTabClose, &option, &painter,
                           this);
  }
};

} // namespace

TabBar::TabBar(QWidget *parent) : QTabBar(parent) {
  setAutoHide(true);
  setDocumentMode(true);
}

void TabBar::tabInserted(int index) {
  QTabBar::tabInserted(index);

  CloseButton *button = new CloseButton(this);
  connect(button, &CloseButton::clicked, this, [this, button] {
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
