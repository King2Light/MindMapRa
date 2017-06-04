#ifndef MAPNODE_H
#define MAPNODE_H

#include <QString>
#include <QPointF>

namespace MindMapRa {

class MapNode
{
public:
    MapNode(const QString& text, const QPointF& pos)
        : m_text(text)
        , m_pos(pos)
    {}

    QString GetText() const { return m_text; }
    QPointF GetPos() const { return m_pos; }
    float GetWidth() const { return 100.0f; }

private:
    QString m_text;
    QPointF m_pos;
};

}

#endif // MAPNODE_H
