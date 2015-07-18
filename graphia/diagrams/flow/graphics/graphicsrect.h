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

#ifndef GRAPHICSRECT_H
#define GRAPHICSRECT_H

#include <QGraphicsItem>
#include "diagrams/generic/graphicsitem.h"
#include <QFont>
#include <QBrush>
#include "diagrams/flow/model/data.h"

class CGraphicsReader;
class CGraphicsWriter;

class CGraphicsRect : public QGraphicsItem, public CGraphicsItem
{
public:
  CGraphicsRect(QPointF& center, CItem* obj, QGraphicsItem* parent = 0);
  virtual ~CGraphicsRect();
  
public:
  //from QGraphicsItem
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
  virtual QRectF boundingRect() const;
  virtual int type() const { return CGraphicsItem::GraphicsItem; }
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
  
  //from CGraphicsItem
  virtual void onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor);
  virtual void onPositionHasChanged(void);
  virtual bool getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint);
  virtual void linkTo(CGraphicsItem* item);
  virtual bool onLeftClickEvent(const QPointF& point) { Q_UNUSED(point); return false; }
  virtual void updateCurrentPoint(const QPointF& point) { Q_UNUSED(point); } 
  virtual bool associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order);

  virtual void unlink(void);
  virtual void removeLinkTo(CGraphicsItem* item);
  
  virtual QList<CPropertyDefinition::Type> getListOfGraphicalProperties();
  virtual QVariant getGraphicalProperties(CPropertyDefinition::Type type);
  virtual void setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value);
  virtual void insertItemIntoDomDocument(QDomDocument &doc, QDomElement &element);

  virtual void setAndUpdateGraphicalData(void);
  
protected:
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
  virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event );
  
protected:
  void buildEnclosedRect(qreal height, qreal length);
  void warmThatPosHasChanged(void);
  void buildAnchor(void);
  void updateAnchorPosition(void);
  void anchorSetVisible(bool visible);
  void updateTextRectangle(void);
  void updateWidthRect(qreal width);
  void updateHeightRect(qreal height);
  
protected:
  QRectF m_rect;
  QRectF m_rectName;
  QFont m_font;
  QColor m_fontColor;
  QColor m_lineColor;
  QBrush m_brush;
  qreal m_rectAngle;
  int m_alignmentH;
  int m_alignmentV;
  
  QSet<CGraphicsItem*> m_listAttachedItem;
  CGraphicsAnchor* m_anchor[4];
  
private:
  void updateDataFlow(CData* data);
  QString getFlowName();

  friend CGraphicsWriter;
  friend CGraphicsReader;
};

#endif // GRAPHICSRECT_H
