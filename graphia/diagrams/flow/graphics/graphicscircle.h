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

#ifndef GRAPHICSCIRCLE_H
#define GRAPHICSCIRCLE_H

#include "diagrams/generic/graphicsitem.h"
#include <QSet>
#include <QFont>
#include <QBrush>
#include "graphicsanchor.h"

class CGraphicsWriter;
class CGraphicsReader;
class CGraphicsCircle : public QGraphicsItem, public CGraphicsItem
{

public:
  CGraphicsCircle(QPointF& center, CItem* obj, QGraphicsItem* parent = 0);
  virtual ~CGraphicsCircle();

  virtual int type() const { return CGraphicsItem::GraphicsItem; }

  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
  virtual QRectF boundingRect() const;

  virtual bool associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order);
  virtual void linkTo(CGraphicsItem* item);
  virtual bool getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint);
  virtual QPainterPath shape() const;
  virtual void unlink(void);
  virtual void removeLinkTo(CGraphicsItem* item);

  virtual QList<CPropertyDefinition::Type> getListOfGraphicalProperties();
  virtual QVariant getGraphicalProperties(CPropertyDefinition::Type type);
  virtual void setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value);
  virtual void insertItemIntoDomDocument ( QDomDocument& doc, QDomElement& element );
  
  virtual void setAndUpdateGraphicalData(void);
  
protected:
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
  virtual void onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor);
    
  //following fct are not used in the case of graphics circle
  virtual bool onLeftClickEvent(const QPointF& point) { Q_UNUSED(point); return false; }
  virtual void onPositionHasChanged(void ) {} 
  virtual void updateCurrentPoint(const QPointF& point) { Q_UNUSED(point); }
  virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent* event );
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
  virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event );
  
protected:
  void buildAnchor();
  void updateAnchorPosition();
  void warmThatPosHasChanged(void);
  void anchorSetVisible(bool visible);
  void buildEnclosedRect(void);

protected:
  qreal m_radius;
  QRectF m_rect;
  QRectF m_rectName;
  QSet<CGraphicsItem*> m_listAttachedItem;
  CGraphicsAnchor* m_anchor[4];
  QFont m_font;
  QColor m_fontColor;
  QColor m_lineColor;
  QBrush m_brush;
  int m_alignmentH;
  int m_alignmentV;

private:
  void updateProcessName(const QString& newName);
  
  friend CGraphicsWriter;
  friend CGraphicsReader;
};

#endif // GRAPHICSCIRCLE_H
