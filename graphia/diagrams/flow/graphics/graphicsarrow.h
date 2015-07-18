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

#ifndef GRAPHICSARROW_H
#define GRAPHICSARROW_H

#include <QBrush>
#include <QFont>
#include "diagrams/generic/graphicsitem.h"
#include "graphicsanchor.h"

class CData;
class CGraphicsWriter;
class CGraphicsReader;

class CGraphicsArrow : public QGraphicsItem, public CGraphicsItem
{
public:
  CGraphicsArrow(QPointF& startPoint, CItem* associatedObj, QGraphicsItem* parent = 0);
  virtual ~CGraphicsArrow();
  
  //from CGraphicsItem
  virtual void updateCurrentPoint(const QPointF& point);
  virtual bool onLeftClickEvent(const QPointF& point);
  virtual bool associate(CGraphicsItem* gItem, CGraphicsItem::LinkOrder order);

  virtual void onPositionHasChanged(void);
  virtual void onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor);
  //fct not used in this case 
  virtual void linkTo(CGraphicsItem* item) { Q_UNUSED(item); }
  virtual void unlink(void);
  virtual void removeLinkTo(CGraphicsItem* item);
  virtual bool getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint) { Q_UNUSED(refPoint);Q_UNUSED(interPoint); return false; }
  virtual QList<CPropertyDefinition::Type> getListOfGraphicalProperties(void);
  virtual QVariant getGraphicalProperties(CPropertyDefinition::Type type);
  virtual void setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value);
  virtual void insertItemIntoDomDocument ( QDomDocument& doc, QDomElement& element );

  virtual void setAndUpdateGraphicalData(void);

protected:
  //from QGraphicsItem
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
  virtual QRectF boundingRect() const;
  virtual QPainterPath shape() const;
  virtual int type() const { return CGraphicsItem::GraphicsItem; }
  
  virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent* event );
  virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event );
    
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
  virtual void keyPressEvent(QKeyEvent* event);
  virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event );
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
  virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event );
  
protected:
  void updatePositionFirstPoint(void);
  void updatePositionLastPoint(void);
  void updateTextPosition(void);
  void calculateArrowPoint(void);
  void anchorSetVisible(bool visible);
  void dissociate(CGraphicsItem::LinkOrder order);
    
protected:
  QList<QPointF> m_points;
  int m_currentIndexOfInsertion;
  CGraphicsItem* m_beginItem;
  CGraphicsItem* m_endItem;
  qreal m_arrowLength;
  qreal m_arrowAngle;
  QPointF m_arrowPoints[5]; //Note: the first two points for the first arrow, the last three for the second arrow if needed
  bool m_bArrowAvailable;
  
  QList<CGraphicsAnchor*> m_anchors;
  QRectF m_labelRect; 
  bool m_isLabelManuallyMoved; //TODO not used now
  bool m_isLabelDisplayed;
  bool m_moveOfLabelOnGoing;
  
  bool m_linkInGoing;
  QColor m_color;
  QFont m_font;
  QColor m_fontColor;
  QBrush m_brush;
  
private:
  QString getFlowName();
  void updateDataFlow(CData* data);
  void addAnchor(int no);
  void updateAnchor(int itemNb);
  void getAroundLine(const QLineF& line, QLineF& lineT) const;
  void replacePoint(int no, const QPointF& newPoint);
  void removePointAndAnchor(int no); 
  QRectF boundingRectString( QPointF startPoint );
  
  friend CGraphicsWriter;
  friend CGraphicsReader;
};

#endif // GRAPHICSARROW_H
