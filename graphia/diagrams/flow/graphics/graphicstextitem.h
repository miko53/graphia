/*
    graphia - little tool for building designer diagram
    Copyright (C) 2012  Mickael Sergent

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H

#include <QGraphicsItem>
#include "diagrams/generic/graphicsitem.h"
#include "graphicsanchor.h"
#include "graphicstextitemprivate.h"

class CGraphicsReader;
class CGraphicsWriter;

class CGraphicsTextItem : public QGraphicsTextItem, public  CGraphicsItem
{

public:
  CGraphicsTextItem(QPointF& point, QGraphicsItem* parent = 0);
  virtual ~CGraphicsTextItem();

  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
  
  //from QGraphicsItem
  virtual int type() const { return CGraphicsItem::GraphicsItem; }

protected:
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );

public:
  //from CGraphicsItem
  virtual bool associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order);
  virtual bool getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint);
  virtual void linkTo(CGraphicsItem* item);
  virtual bool onLeftClickEvent(const QPointF& point);
  virtual void onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor);
  virtual void onPositionHasChanged(void );
  virtual void updateCurrentPoint(const QPointF& point);

  virtual void unlink(void) {  }
  virtual void removeLinkTo(CGraphicsItem* item) { Q_UNUSED(item); }

  virtual QList<CPropertyDefinition::Type> getListOfGraphicalProperties(void);
  virtual QVariant getGraphicalProperties(CPropertyDefinition::Type type);
  virtual void setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value);
  virtual void insertItemIntoDomDocument(QDomDocument& doc, QDomElement& element);
    
  virtual void setAndUpdateGraphicalData ( void );
//private slots:
//    void updateGeometry(int, int, int);

protected:
  bool isInEdge(const QPointF& pos);
  void updateAlignment();

protected:
  int m_alignmentH;

private:
  bool m_isFirstClick;
  CGraphicsTextItemPrivate* d;
  friend class CGraphicsTextItemPrivate;
  friend class CGraphicsWriter;
  friend class CGraphicsReader;
};

#endif // GRAPHICSTEXTITEM_H
