#include "mapnodewidget.h"
#include "mapnode.h"

#include <QTextEdit>
#include <QGridLayout>
#include <QPlainTextEdit>

class NodeTextEdit : public QPlainTextEdit
{
    MapNodeWidget* m_parent;

public:
    explicit NodeTextEdit(MapNodeWidget *parent = 0)
        : QPlainTextEdit(parent)
        , m_parent(parent)
    {}

    void mousePressEvent(QMouseEvent* ev) Q_DECL_OVERRIDE
    {
        if (isReadOnly())
        {
            ev->ignore();
            return;
        }

        QPlainTextEdit::mousePressEvent(ev);
    }

    void keyPressEvent(QKeyEvent* ev) Q_DECL_OVERRIDE
    {
        update();

        if (isReadOnly() && ev->key() == Qt::Key_Space) {
            setReadOnly(false);
        }
        else if (!isReadOnly() && ev->key() == Qt::Key_Escape) {
            setReadOnly(true);
        }
        else if (!isReadOnly() && ev->key() == Qt::Key_Tab) {
            setReadOnly(true);
            m_parent->keyPressEvent(ev);
        }
        else if (!isReadOnly() && ev->key() == Qt::Key_Return && (ev->modifiers() & Qt::ShiftModifier) == 0) {
            setReadOnly(true);
            m_parent->keyPressEvent(ev);
        }
        else if (!isReadOnly()) {
            QPlainTextEdit::keyPressEvent(ev);
        }
        else {
            m_parent->keyPressEvent(ev);
        }
    }

    bool focusNextPrevChild(bool) Q_DECL_OVERRIDE
    {
        // prevent internal qt logic with tab focus change
        return false;
    }

    void focusInEvent(QFocusEvent*) Q_DECL_OVERRIDE
    {
        setReadOnly(true);
        m_parent->OnTextareaFocusChange(true);
    }

    void focusOutEvent(QFocusEvent *) Q_DECL_OVERRIDE
    {
        setReadOnly(true);
        m_parent->OnTextareaFocusChange(false);
    }
};

MapNodeWidget::MapNodeWidget(MindMapRa::MapNode* node, QWidget* parent)
    : QFrame(parent)
    , m_isFolded(false)
    , m_node(node)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizeIncrement(10, 10);
    setFocusPolicy(Qt::ClickFocus);

    QGridLayout* layout = new QGridLayout(this);

    QString addrStr = QString("0x%1").arg((size_t)this,sizeof(size_t)*2,16,QLatin1Char('0'));

    m_label = new NodeTextEdit(this);
    m_label->setPlaceholderText(addrStr);
    m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_label->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_label->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_label->setReadOnly(true);
    m_label->setFocusPolicy(Qt::ClickFocus);
    m_label->document()->setPlainText(m_node->GetText());
    layout->addWidget(m_label);
    layout->setMargin(0);

    connect(m_label, SIGNAL(textChanged()),
            this, SLOT(OnTextChanged()));

    this->setFrameShape(QFrame::Box);

    clearFocus();
    OnTextChanged();
}

MapNodeWidget::~MapNodeWidget()
{
    delete m_label;
}

void MapNodeWidget::SetText(const QString &text)
{
    m_label->document()->setPlainText(text);
}

void MapNodeWidget::SetFolded(bool isFolded)
{
    m_isFolded = isFolded;
    UpdateTextareaStyle();
}

void MapNodeWidget::mousePressEvent(QMouseEvent *)
{
    emit OnChangeFocusUserRequest(this);
}

void MapNodeWidget::keyPressEvent(QKeyEvent *ev)
{
    // input keys are processed in label
    // control keys are processed in parent
    emit OnKeypress(ev);
}

void MapNodeWidget::focusInEvent(QFocusEvent*)
{
    // focus should be on label
    m_label->setFocus();
}

bool MapNodeWidget::focusNextPrevChild(bool)
{
    // prevent internal qt logic with tab focus change
    return false;
}

void MapNodeWidget::OnTextareaFocusChange(bool hasFocus)
{
    Q_UNUSED(hasFocus);
    UpdateTextareaStyle();
}

void MapNodeWidget::UpdateTextareaStyle()
{
    if (m_label->hasFocus())
    {
        if (!m_isFolded)
            m_label->setStyleSheet("QPlainTextEdit { background-color : #D6B0FF; color : #1D1624; }");
        else
            m_label->setStyleSheet("QPlainTextEdit { background-color : #7c7385; color : #1D1624; }");
    }
    else
    {
        if (!m_isFolded)
            m_label->setStyleSheet("QPlainTextEdit { background-color : #FCFFFF; color : #1D1624; }");
        else
            m_label->setStyleSheet("QPlainTextEdit { background-color : #7ac483; color : #1f1f1b; }");
    }
}


void MapNodeWidget::OnTextChanged()
{
    const QString dataText = m_label->document()->toPlainText();

    m_node->SetText(dataText);

    const QFontMetrics fm(m_label->font());
    const QSize addSize(15, 10);
    const QSize maxSize(300, 60);

    const QString placeholderText = m_label->placeholderText();
    const QString text = ((dataText.size() == 0) ? placeholderText : dataText);
    const QRect textRect = fm.boundingRect(
                QRect(QPoint(0,0), maxSize),
                Qt::TextWordWrap | Qt::TextWrapAnywhere,
                text);

    const QSize textSizeAdd(
                textRect.width() + addSize.width(),
                textRect.height() + addSize.height() );

    m_size = QSize( qMin(maxSize.width(), textSizeAdd.width()),
                    qMin(maxSize.height(), textSizeAdd.height()));

    resize(m_size);
    emit OnWidgetResize(this, m_size);
}

