#ifndef MAPLAYOUT_H
#define MAPLAYOUT_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QPointF>
#include <QRectF>

namespace MindMapRa {

class MapNode;

class MapLayout : public QObject
{
    Q_OBJECT
public:
    explicit MapLayout(QObject *parent = 0);

public slots:
    void OnNodeAdded(MapNode* node);
    void OnNodeDeleted(MapNode* node);
    void OnConnected(MapNode* from, MapNode* to);
    void OnDisconnected(MapNode* from, MapNode* to);

signals:
    void OnNodePosition(MapNode* node, QPointF oldPos, QPointF newPos);

private:
    void FixAllPositions();
    int UpdateAndGetNodeSize(MapNode* node, int topPos, int parentLeftPos);

    struct Block {
        MapNode* parent;
        QVector<MapNode*> nodes;
        QVector<int> nodeVertPos;
        QPointF pos;
    };

    QMap<MapNode*, Block> m_blocks;
    QMap<MapNode*, QPointF> m_prevBlocks;
    Block m_orphans;
};

}

#endif // MAPLAYOUT_H
