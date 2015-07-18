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

#ifndef GRAPHICSOUTPUT_H
#define GRAPHICSOUTPUT_H

#include <QGraphicsItem>
#include "diagrams/generic/graphicsitem.h"
#include <QFont>
#include <QBrush>
#include "diagrams/flow/graphics/graphicsanchor.h"
#include "diagrams/flow/model/data.h"

class CGraphicsWriter;
class CGraphicsReader;

class CGraphicsOutput : public QGraphicsItem, public CGraphicsItem
{
public:
  CGraphicsOutput(QPointF& center, CItem* obj, QGraphicsItem* parent = 0);
  virtual ~CGraphicsOutput();

public:
  //from QGraphicsItem
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
  virtual QRectF boundingRect() const;
  virtual int type() const { return CGraphicsItem::GraphicsItem; }

protected:
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);  
  
public:
  //from CGraphicsItem
  virtual void onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor);
  virtual void onPositionHasChanged(void ) {}
  virtual bool getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint);
  virtual void linkTo(CGraphicsItem* item);
  virtual bool onLeftClickEvent(const QPointF& point) { Q_UNUSED(point); return false; }
  virtual void updateCurrentPoint(const QPointF& point) { Q_UNUSED(point); }
  virtual bool associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order);
  virtual void unlink(void);
  virtual void removeLinkTo(CGraphicsItem* item);
  virtual QList<CPropertyDefinition::Type> getListOfGraphicalProperties(void);
  virtual QVariant getGraphicalProperties(CPropertyDefinition::Type type);
  virtual void setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value);
  virtual void insertItemIntoDomDocument(QDomDocument& doc, QDomElement& element);
  
  virtual void setAndUpdateGraphicalData(void);
  
protected:
  QRectF boundingRectString() const;
  void updateAnchorPosition(void);
  void warmThatPosHasChanged(void);
  QString getFlowName() const;
  void updateDataFlow(CData* data);

protected:
  qreal m_arrowLength;
  qreal m_arrowAngle;
  QFont m_font;
  QColor m_fontColor;
  QColor m_lineColor;
  QBrush m_arrowBrush;

  QSet<CGraphicsItem*> m_listAttachedItem;
  qreal m_offsetX1; //offse at the end with the arrow (output)
  qreal m_offsetY;
  qreal m_offsetX2; //offset at the begin
  qreal m_offsetYmin;
  qreal m_offsetArrowLenMin;

  CGraphicsAnchor* m_anchors[2];
  
private:
  friend CGraphicsWriter;
  friend CGraphicsReader;
};

#endif // GRAPHICSOUTPUT_H
