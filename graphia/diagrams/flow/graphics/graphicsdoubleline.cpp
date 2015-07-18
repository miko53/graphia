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

#include "graphicsdoubleline.h"
#include "graphia.h"
#include "global.h"
#include <QPainter>
#include <QMenu>
#include <QAction>
#include "diagrams/flow/dialog/dialogsetname.h"
#include "diagrams/flow/model/processcontext.h"
#include <math.h>
#include <QMessageBox>
#include "graphicswriter.h"

CGraphicsDoubleLine::CGraphicsDoubleLine(QPointF& center, CItem* obj, QGraphicsItem* parent):
  QGraphicsItem(parent), CGraphicsItem(obj)
{
  qreal height = getApp()->getGraphicsConfig()->storeHeight();
  qreal length = getApp()->getGraphicsConfig()->storeLength();
  QPointF topLeft;
  
  topLeft.setX(center.x() - length/2);
  topLeft.setY(center.y() - height/2);
  
  setPos(topLeft);

  m_font = getApp()->getGraphicsConfig()->font();
  m_fontColor = getApp()->getGraphicsConfig()->fontColor();
  m_lineColor = getApp()->getGraphicsConfig()->lineColor();
  m_alignmentH = getApp()->getGraphicsConfig()->alignmentH();
  m_alignmentV = getApp()->getGraphicsConfig()->alignmentV();
  m_brush = QBrush(Qt::SolidPattern);
  m_brush.setColor(getApp()->getGraphicsConfig()->background());
  
  buildEnclosedRect(height, length);

  buildAnchor();
  updateAnchorPosition();
  anchorSetVisible(false);

  m_itemProperties = (CAN_BE_LINKED | IS_REMOVABLE);
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
  setZValue(50.0);

}


void CGraphicsDoubleLine::setAndUpdateGraphicalData ( void )
{
 // prepareGeometryChange();
  buildEnclosedRect(m_rect.height(), m_rect.width());
//  buildAnchor();
  updateAnchorPosition();
  anchorSetVisible(false);
//  update();
}


void CGraphicsDoubleLine::buildEnclosedRect(qreal height, qreal length)
{
  m_rect = QRectF(0.0,
                  0.0,
                  length,
                  height);
  
  double distance = hypot(length, height);
  m_rectAngle = acos(length/distance);
//   qDebug("m_recAngle = %g", m_rectAngle * 180/PI);
  updateTextRectangle();

}

void CGraphicsDoubleLine::updateWidthRect(qreal width)
{
  m_rect.setWidth(width);
  double distance = hypot(width, m_rect.height());
  m_rectAngle = acos(width/distance);  
}

void CGraphicsDoubleLine::updateHeightRect(qreal height)
{
  m_rect.setHeight(height);
  double distance = hypot(m_rect.width(), height);
  m_rectAngle = acos(m_rect.width()/distance);
}

const qreal PADDING = 1.0;

void CGraphicsDoubleLine::updateTextRectangle ( void )
{
  m_rectName = m_rect.adjusted(+PADDING, +PADDING, -PADDING, -PADDING);
}



CGraphicsDoubleLine::~CGraphicsDoubleLine()
{
  for(int i=0; i<4; i++)
    if (m_anchor[i] != NULL)
      delete m_anchor[i];
}

enum
{
  ANCHOR_TOP_LEFT,
  ANCHOR_TOP_RIGHT,
  ANCHOR_BOTTOM_LEFT,
  ANCHOR_BOTTOM_RIGHT
};

void CGraphicsDoubleLine::buildAnchor()
{
  for(int i = 0; i < 4; i++)
    m_anchor[i] = new CGraphicsAnchor(this);

  m_anchor[ANCHOR_TOP_LEFT]->setCursor(Qt::SizeFDiagCursor);
  m_anchor[ANCHOR_TOP_LEFT]->setPrivData(ANCHOR_TOP_LEFT);

  m_anchor[ANCHOR_TOP_RIGHT]->setCursor(Qt::SizeBDiagCursor);
  m_anchor[ANCHOR_TOP_RIGHT]->setPrivData(ANCHOR_TOP_RIGHT);

  m_anchor[ANCHOR_BOTTOM_LEFT]->setCursor(Qt::SizeBDiagCursor);
  m_anchor[ANCHOR_BOTTOM_LEFT]->setPrivData(ANCHOR_BOTTOM_LEFT);

  m_anchor[ANCHOR_BOTTOM_RIGHT]->setCursor(Qt::SizeFDiagCursor);
  m_anchor[ANCHOR_BOTTOM_RIGHT]->setPrivData(ANCHOR_BOTTOM_RIGHT);
}

void CGraphicsDoubleLine::updateAnchorPosition()
{
  qreal anchorSize_2 = getApp()->getGraphicsConfig()->anchorSize()/2.0;
  //qreal width_2 = m_rect.width()/2.0;
  //qreal height_2 = m_rect.height()/2.0;

  QPointF anchorPoint;
  
  anchorPoint = m_rect.topLeft();
  anchorPoint.setX(anchorPoint.x() - anchorSize_2);
  anchorPoint.setY(anchorPoint.y() - anchorSize_2);
  m_anchor[ANCHOR_TOP_LEFT]->setTopLeft(anchorPoint);

  anchorPoint = m_rect.topRight();
  anchorPoint.setX(anchorPoint.x() - anchorSize_2);
  anchorPoint.setY(anchorPoint.y() - anchorSize_2);  
  m_anchor[ANCHOR_TOP_RIGHT]->setTopLeft(anchorPoint);

  anchorPoint = m_rect.bottomLeft();
  anchorPoint.setX(anchorPoint.x() - anchorSize_2);
  anchorPoint.setY(anchorPoint.y() - anchorSize_2);
  m_anchor[ANCHOR_BOTTOM_LEFT]->setTopLeft(anchorPoint);

  anchorPoint = m_rect.bottomRight();
  anchorPoint.setX(anchorPoint.x() - anchorSize_2);
  anchorPoint.setY(anchorPoint.y() - anchorSize_2);
  m_anchor[ANCHOR_BOTTOM_RIGHT]->setTopLeft(anchorPoint);
}

void CGraphicsDoubleLine::anchorSetVisible(bool visible)
{
  for(int i = 0; i < 4 ; i++)
  {
    m_anchor[i]->setVisible(visible);
  }
}

void CGraphicsDoubleLine::onPositionHasChanged(void)
{

}

bool CGraphicsDoubleLine::getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint)
{
  QPointF center;
  qreal dx;
  qreal dy;
  qreal angle;
  QPointF refPointInItemCoordinate;
  //const qreal OFFSET = 1.0;

  refPointInItemCoordinate = mapFromScene(refPoint);
  
  center = m_rect.center();
  //center = mapToScene(center);
  
  //qDebug("refPoint.x = %g, refPoint.y = %g\n", refPoint.x(), refPoint.y());
  //qDebug("center.x = %g, center.y = %g\n", center.x(), center.y());
  
  dx = center.x() - refPointInItemCoordinate.x();
  dy = center.y() - refPointInItemCoordinate.y();
  
  qreal b = center.y() - center.x() * dy/dx;
  qreal a = dy/dx;  
  
  qreal distance;
  distance = hypot(dx, dy);

  
  angle = acos(-dx/distance);
  if (dy > 0.0)
    angle = 2 * PI - angle;
 
  //qDebug("distance = %g", distance);
  //qDebug("dx = %g\n", dx);
  //qDebug("dy = %g\n", dy);
  
  //qDebug("m_rectAngle = %g", m_rectAngle * 180/PI);
  //qDebug("angle = %g", angle * 180/PI);
/*  QPointF refPointInInterm = mapFromScene(refPoint);
  if (m_rect.contains(refPoint))
  {
    qDebug("in rectangle");
    return false;
  }*/

  int area;
  if (angle <= m_rectAngle)
    area = 0;
  else if ((angle > m_rectAngle) && (angle <= (PI - m_rectAngle)))
    area = 1;
  else if ((angle > (PI - m_rectAngle)) && (angle <= (PI + m_rectAngle)))
    area = 2;
  else if ((angle > (PI + m_rectAngle)) && (angle <= (2*PI - m_rectAngle)))
    area = 3;
  else
    area = 0;
  
 //qDebug("area = %i", area);
  qreal width = m_rect.width();///2.0;
  qreal height = m_rect.height();//()/2.0; 

  
  switch (area)
  {
    case 0:
      interPoint = QPointF(width, a * width + b);
      break;
      
    case 1:
      interPoint = QPointF((height-b)/a, height);
      break;
      
    case 2:
      interPoint = QPointF(0, b);
      break;
      
    case 3:
      interPoint = QPointF(-b/a, 0);
      break;
      
    default:
      //no default
      break;
  }
  
  interPoint = mapToScene(interPoint);
  return true;
}

void CGraphicsDoubleLine::linkTo(CGraphicsItem* item)
{
  m_listAttachedItem.insert(item);
}

bool CGraphicsDoubleLine::associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);
  qDebug("CGraphicsDoubleLine::associate");
  Q_ASSERT(false);
  return false;
}

void CGraphicsDoubleLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QPen currentPen;

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::TextAntialiasing);
  painter->setFont(m_font);

  currentPen.setStyle(Qt::SolidLine);
  currentPen.setColor(m_lineColor);
  currentPen.setWidthF(1.0);
  currentPen.setJoinStyle(Qt::MiterJoin);
  painter->setPen(currentPen);
  //painter->setPen(m_lineColor);
  painter->fillRect(m_rect, m_brush);
  painter->drawLine(m_rect.topLeft(), m_rect.topRight());
  painter->drawLine(m_rect.bottomLeft(), m_rect.bottomRight());
  
  painter->setPen(m_fontColor);
  painter->drawText(m_rectName, m_alignmentH | m_alignmentV |Qt::TextWordWrap/*|Qt::TextWordWrap*/, m_item->getName());  
}

QRectF CGraphicsDoubleLine::boundingRect() const
{
  QRectF rect(m_rect);
  const qreal PADDING = 1.0;
  rect.adjust(-PADDING,-PADDING,
                PADDING, PADDING
             );
  return rect;
}


QVariant CGraphicsDoubleLine::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionHasChanged)
  {
    warmThatPosHasChanged();
    emit itemChanged(this, CPropertyDefinition::POS, pos());
  }
  else if (change == ItemSelectedChange)
  {
    anchorSetVisible(value.toBool());
    if (value.toBool() == true)
      setZValue(95.0);
    else
      setZValue(50.0);
  }
  return QGraphicsItem::itemChange(change, value);
}

void CGraphicsDoubleLine::warmThatPosHasChanged(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->onPositionHasChanged();
  }
}

void CGraphicsDoubleLine::onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor)
{
  Q_ASSERT(anchor != NULL);
  Q_ASSERT(anchor->parentItem() == this);

  qreal dx;
  qreal dy;
  qreal newHeight;
  qreal newWidth;
  QPointF lastPoint;
  QPointF point;

  lastPoint = sceneEvent->lastScenePos();
  point = sceneEvent->scenePos();
  dx = point.x() - lastPoint.x();
  dy = point.y() - lastPoint.y();

  newHeight = m_rect.height();
  newWidth = m_rect.width();
  
  //qDebug("dx = %g, dy=%g", dx, dy);
  
  switch (anchor->privData())
  {
    case ANCHOR_TOP_LEFT:
      dx = -dx;
      dy = -dy;
      newWidth += dx;
      newHeight += dy;
      if ((newHeight > 0) && (newWidth > 0))
      {
        setPos(pos().x() - dx, pos().y() - dy);
      }
      break;

    case ANCHOR_BOTTOM_LEFT:
      dx = -dx;
      newWidth += dx;
      newHeight += dy;
      if ((newHeight > 0) && (newWidth > 0))
      {
        setPos(pos().x() - dx, pos().y() );
      }
      break;

    case ANCHOR_BOTTOM_RIGHT:
      newWidth += dx;
      newHeight += dy;
      //if ((newHeight > 0) && (newWidth > 0))
      //{
      //  setPos(pos().x() + dx/2.0, pos().y() + dy/2.0);
      //}
      break;
      
    case ANCHOR_TOP_RIGHT:
      dy = -dy;
      newWidth += dx;
      newHeight += dy;
      if ((newHeight > 0) && (newWidth > 0))
      {
        setPos(pos().x(), pos().y() - dy);
      }
      break;

    default:
      Q_ASSERT(false);
      break;
  }
  
  if ((newHeight > 0) && (newWidth > 0))
  {
    prepareGeometryChange();
    buildEnclosedRect(newHeight, newWidth);
    updateAnchorPosition();
    update();
    warmThatPosHasChanged();
    
    emit itemChanged(this, CPropertyDefinition::HEIGHT, newHeight);
    emit itemChanged(this, CPropertyDefinition::WIDTH, newWidth);
  }
}


void CGraphicsDoubleLine::unlink(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->removeLinkTo(this);
  }
}


void CGraphicsDoubleLine::removeLinkTo(CGraphicsItem* item)
{
  bool b;
  b = m_listAttachedItem.remove(item);
  Q_ASSERT(b == true);
}

QList< CPropertyDefinition::Type > CGraphicsDoubleLine::getListOfGraphicalProperties(void )
{
  QList<CPropertyDefinition::Type> list;

  list << CPropertyDefinition::NAME;
  list << CPropertyDefinition::POS;
  list << CPropertyDefinition::FONT;
  list << CPropertyDefinition::FONT_COLOR;
  list << CPropertyDefinition::LINE_COLOR;
  list << CPropertyDefinition::BACKGND_COLOR;
  list << CPropertyDefinition::ALIGNMENT_H;
  list << CPropertyDefinition::ALIGNMENT_V;
  list << CPropertyDefinition::HEIGHT;
  list << CPropertyDefinition::WIDTH;

  return list;
}


QVariant CGraphicsDoubleLine::getGraphicalProperties(CPropertyDefinition::Type type)
{
  QVariant property;

  switch (type)
  {
    case CPropertyDefinition::NAME:
    property.setValue(m_item->getName());
    break;

    case CPropertyDefinition::POS:
    property.setValue(pos());
    break;
    
    case CPropertyDefinition::FONT:
    property.setValue(m_font);
    break;

    case CPropertyDefinition::FONT_COLOR:
    property.setValue(m_fontColor);
    break;
    
    case CPropertyDefinition::LINE_COLOR:
    property.setValue(m_lineColor);
    break;
    
    case CPropertyDefinition::BACKGND_COLOR:
    property.setValue(m_brush.color());
    break;

    case CPropertyDefinition::HEIGHT:
    property.setValue(m_rect.height());
    break;

    case CPropertyDefinition::WIDTH:
    property.setValue(m_rect.width());
    break;
    
    case CPropertyDefinition::ALIGNMENT_H:
    property.setValue((int)CPropertyDefinition::intToAlignmentH(m_alignmentH));
    break;

    case CPropertyDefinition::ALIGNMENT_V:
    property.setValue((int)CPropertyDefinition::intToAlignmentV(m_alignmentV));
    break;
    
    default:
    Q_ASSERT(false);
    break;
  }

  return property;
}


void CGraphicsDoubleLine::setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value)
{
  prepareGeometryChange();

  switch (type)
  {
    case CPropertyDefinition::NAME:
    m_item->setName(value.toString());
    break;

    case CPropertyDefinition::POS:
    setPos(value.toPointF());
    break;
    
    case CPropertyDefinition::FONT:
    m_font = value.value<QFont>();
    break;

    case CPropertyDefinition::FONT_COLOR:
    m_fontColor = value.value<QColor>();
    break;

    case CPropertyDefinition::LINE_COLOR:
    m_lineColor= value.value<QColor>();
    break;

    case CPropertyDefinition::BACKGND_COLOR:
    m_brush.setColor(value.value<QColor>());
    break;

    case CPropertyDefinition::HEIGHT:
    updateHeightRect(value.value<qreal>());
    break;

    case CPropertyDefinition::WIDTH:
    updateWidthRect(value.value<qreal>());
    break;
    
    case CPropertyDefinition::ALIGNMENT_H:
    m_alignmentH = CPropertyDefinition::alignementHToInt(
        (CPropertyDefinition::alignHorizontal) value.value<int>());
    break;

    case CPropertyDefinition::ALIGNMENT_V:
    m_alignmentV = CPropertyDefinition::alignementVToInt(
        (CPropertyDefinition::alignVertical) value.value<int>());
    break;
    
    default:
    Q_ASSERT(false);
    break;
  }

  updateAnchorPosition();
  update();
  warmThatPosHasChanged();
}


void CGraphicsDoubleLine::contextMenuEvent ( QGraphicsSceneContextMenuEvent* event )
{
  QMenu menu;
  int result;
  QAction *setName = menu.addAction(tr("change name..."));
  QAction *selectedAction = menu.exec(event->screenPos());
  if (selectedAction == setName)
  {
    CDialogSetName dialog;
    bool renameOk;
    result = dialog.exec();
    if (result == QDialog::Accepted)
    {
      CItemContainer* processParent = dynamic_cast<CItemContainer*>(m_item->getParent());
      Q_ASSERT(processParent != NULL);
      renameOk = processParent->renameObject(m_item, dialog.getName());
      if (renameOk == false)
      {
        QMessageBox::warning(0, tr("graphia"), tr("operation not possible, a terminator has already this name"),
                             QMessageBox::Discard);
      }
      else
      {
        emit itemChanged(this, CPropertyDefinition::NAME, m_item->getName());
      }
    }    
  }
  
  QGraphicsItem::contextMenuEvent ( event );
}

void CGraphicsDoubleLine::insertItemIntoDomDocument(QDomDocument &doc, QDomElement &element)
{
  CGraphicsWriter writer(this);
  writer.buildDomDocument(doc, element);
}

void CGraphicsDoubleLine::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent ( event );
  CGraphicsScene* theScene = (CGraphicsScene*) scene();
  
  theScene->checkItemLocation(this);
}


void CGraphicsDoubleLine::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseDoubleClickEvent ( event );
  
  qreal OFFSET = 2.0;
  QFontMetricsF fontMetrics(m_font);
  qreal newLenght  = fontMetrics.width(m_item->getName()) + OFFSET;
  
  if (newLenght > m_rect.width())
  {
    prepareGeometryChange(); //TODO faire une fonction commune de mise à jour
    buildEnclosedRect(m_rect.height(), newLenght);
    updateAnchorPosition();
    update();
    warmThatPosHasChanged();
    
    emit itemChanged(this, CPropertyDefinition::WIDTH, newLenght);
  }
}


