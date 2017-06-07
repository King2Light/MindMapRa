#include "mapnodewidget.h"

#include <QLabel>
#include <QGridLayout>

MapNodeWidget::MapNodeWidget(QWidget* parent)
    : QFrame(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setSizeIncrement(200, 100);

    QGridLayout* layout = new QGridLayout(this);

    m_label = new QLabel;
    m_label->setText("Hello world!");
    layout->addWidget(m_label);

    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);

    SetFocusNode(false);
}

void MapNodeWidget::SetText(const QString &text)
{
    m_label->setText(text);
}

void MapNodeWidget::SetFocusNode(bool isFocus)
{
    if (isFocus)
        setBackgroundRole(QPalette::Light);
    else
        setBackgroundRole(QPalette::Dark);
}

void MapNodeWidget::EnableTextEdit(bool isEnable)
{
    // m_label->
}

void MapNodeWidget::mousePressEvent(QMouseEvent *ev)
{
    emit OnChangeFocusUserRequest(this);
}
