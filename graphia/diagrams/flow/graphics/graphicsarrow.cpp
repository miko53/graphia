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

#include "graphicsarrow.h"
#include "graphicswriter.h"
#include "diagrams/generic/toolitemmodel.h"
#include "global.h"
#include "graphia.h"
#include "diagrams/flow/model/output.h"
#include "diagrams/flow/model/flow.h"
#include "diagrams/flow/dialog/dialog.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QDomDocument>
#include <QDomElement>

CGraphicsArrow::CGraphicsArrow(QPointF& startPoint, CItem* associatedObj, QGraphicsItem* parent)
: QGraphicsItem(parent) , CGraphicsItem(associatedObj)
{
  m_linkInGoing = true;
  m_points.insert(0, startPoint);
  m_points.insert(m_points.size(), startPoint);
  addAnchor(0);
  addAnchor(1);
  
  m_currentIndexOfInsertion = 1; // the end point is the current point in mouvment
  m_beginItem = NULL;
  m_endItem = NULL;

  m_arrowAngle = getApp()->getGraphicsConfig()->arrowAngle();
  m_arrowLength = getApp()->getGraphicsConfig()->arrowLength();
  m_color = getApp()->getGraphicsConfig()->lineColor();
  m_font = getApp()->getGraphicsConfig()->font();
  m_fontColor = getApp()->getGraphicsConfig()->fontColor();
  m_brush = QBrush(m_color);
  m_isLabelDisplayed = getApp()->getGraphicsConfig()->displayLabel();
  
  m_itemProperties = (IS_LINK_ITEM | NEED_MORE_STEP | IS_REMOVABLE );

  setZValue(60.0);
  setFlags(ItemIsSelectable | ItemSendsGeometryChanges | ItemIsFocusable /*| ItemIsMovable*/); //TODO meed setPos to move the items, and work also in relative...
  
  m_labelRect = boundingRectString(startPoint);
  m_moveOfLabelOnGoing = false;
  m_isLabelManuallyMoved = false;
  /*qDebug("rect top x%f, top y %f, height %f, width %f", 
         m_labelRect.topLeft().x(),
         m_labelRect.topLeft().y(),
         m_labelRect.height(),
         m_labelRect.width());*/
 }

QRectF CGraphicsArrow::boundingRectString(QPointF startPoint)
{
  qreal PADDING = 1.0;
  QRectF rect;
  QFontMetricsF fontMetrics(m_font);
 
  rect.setTopLeft(startPoint);
  rect.setWidth(fontMetrics.width(getFlowName()));
  rect.setHeight(fontMetrics.height());
  rect.adjust(0, 0, PADDING, PADDING);
  
  return rect;
}

CGraphicsArrow::~CGraphicsArrow()
{
  for(int i = 0; i< m_anchors.size(); i++)
  {
    if (m_anchors[i])
      delete m_anchors[i];
  }
  m_anchors.clear();
}

void CGraphicsArrow::addAnchor(int no)
{
  CGraphicsAnchor* anchor;
  QPointF topLeft;
  
  topLeft.setX(m_points.at(no).x() - 5.0/2);
  topLeft.setY(m_points.at(no).y() - 5.0/2);
  anchor = new CGraphicsAnchor(topLeft, this);
  anchor->setCursor(Qt::SizeAllCursor);
  anchor->setPrivData(no);
  m_anchors.insert(no, anchor);
  anchor->setVisible(false);
}

void CGraphicsArrow::setAndUpdateGraphicalData ( void )
{
  for(int i = 0; i< m_anchors.size(); i++)
  {
    if (m_anchors[i])
      delete m_anchors[i];
  }
  m_anchors.clear();
  
  for(int i = 0; i< m_points.size(); i++)
  {
    addAnchor(i);
  }
}


void CGraphicsArrow::updateAnchor(int itemNb)
{
  CGraphicsAnchor* anchor;
  QPointF topLeft;
  
  topLeft.setX(m_points.at(itemNb).x() - 5.0/2);
  topLeft.setY(m_points.at(itemNb).y() - 5.0/2);
  anchor = m_anchors.at(itemNb);
  anchor->setTopLeft(topLeft);
}

void CGraphicsArrow::anchorSetVisible(bool visible)
{
  foreach(CGraphicsAnchor* anchor, m_anchors)
  {
    anchor->setVisible(visible);
  }
}


void CGraphicsArrow::updateCurrentPoint(const QPointF& point)
{
  //qDebug("CGraphicsArrow::updateCurrentPoint");

  if (m_currentIndexOfInsertion == 1)
  {
    updatePositionFirstPoint();
  }
  replacePoint(m_currentIndexOfInsertion, point);
  
  updateTextPosition();
}

void CGraphicsArrow::replacePoint(int no, const QPointF& newPoint)
{
  prepareGeometryChange();
  m_points.replace(no, newPoint);
  updateAnchor(no);
  update();
}


bool CGraphicsArrow::onLeftClickEvent(const QPointF& point)
{
  //return TRUE when ended, FALSE not
  // in this case add a point in the list and always return FALSE
  // it is possible to create a type of arrow without list of point in this case
  // in this function it will return true
  qDebug("CGraphicsArrow::onLeftClickEvent");
  m_points.insert(m_points.size(), point);
  addAnchor(m_points.size()-1);
  m_currentIndexOfInsertion++;
  return false;
}

bool CGraphicsArrow::associate(CGraphicsItem* gItem, CGraphicsItem::LinkOrder order)
{
  Q_ASSERT(m_item != NULL);
  CItem* item = gItem->getAssociatedItem();
  bool b;
  
  qDebug("try association '%s' with '%s' (%s)", 
         m_item->getName().toLocal8Bit().data(),
         gItem == NULL ? "NULL" : gItem->getAssociatedItem()->getName().toLocal8Bit().data(),
         order == CGraphicsItem::BEGIN ? "BEGIN" : "END");
  
  if (item != NULL)
  {
    b = item->associateWith(m_item, order);
    //b = m_item->associateWith(item, order);
    if (b)
    {
      // do the link with the item.
      qDebug("association ok");
      switch (order)
      {
        case CGraphicsItem::BEGIN:
          m_beginItem = gItem;
          break;
          
        case CGraphicsItem::END:
          m_linkInGoing = false; //the item is end to be linked (at least at the end) this flag is used to block the escape key behavior
          m_endItem = gItem;
          updatePositionLastPoint();
          break;
          
        default:
          Q_ASSERT(false);
          break;
      }
      gItem->linkTo(this);
    }
  }
  else 
    b = false; //no associatedItem, so association can't be done...
  
  return b;
}


void CGraphicsArrow::onPositionHasChanged(void)
{
  updatePositionFirstPoint();
  updatePositionLastPoint();
  updateTextPosition();
}

void CGraphicsArrow::updatePositionFirstPoint(void)
{
  Q_ASSERT(m_points.size() >= 2);
  //update the first point
  if (m_beginItem != NULL)
  {
    QPointF newFirstPoint;
    bool pointExist;
    pointExist = m_beginItem->getIntersectionPoint(m_points.at(1), newFirstPoint);

    if (pointExist)
    {
      replacePoint(0, newFirstPoint);
    }
  }
}

void CGraphicsArrow::updatePositionLastPoint(void)
{
  Q_ASSERT(m_points.size() >= 2);
  //update the last point

  if (m_endItem != NULL)
  {
    QPointF newEndPoint;
    bool pointExist;
    pointExist = m_endItem->getIntersectionPoint(m_points.at(m_points.size()-2), newEndPoint);

    if (pointExist)
    {
      replacePoint(m_points.size()-1, newEndPoint);
    }
  }
}

void CGraphicsArrow::updateTextPosition ( void )
{
  QPointF pos;
  pos.setX((m_points.at(0).x() + m_points.at(1).x())/2);
  pos.setY((m_points.at(0).y() + m_points.at(1).y())/2);
  
  m_labelRect = boundingRectString(pos);
}


void CGraphicsArrow::calculateArrowPoint(void)
{
  //calculate first two points of the arrow
  //  prendre l'angle d'inclinaison entre la droite m-start/m-end
  //  arccos est defini sur 0-PI, changer l'angle pour la fleche au desssus (delta_y <0)
  // appliquer la rotation autour de l'angle_a +/- l'angle de la fleche du point (1,0)
  
  QPointF startPoint = m_points.at(m_points.size() - 2);
  QPointF endPoint = m_points.at(m_points.size()- 1);

  double delta_x = endPoint.x() - startPoint.x();
  double delta_y = endPoint.y() - startPoint.y();
  double distance = hypot(delta_x, delta_y);

  if (distance == 0.0)
  {
    //qDebug("arrow::distance == 0");
    m_bArrowAvailable = false;
    return;
  }

  //QPen pen(Qt::black);
  double angle_a = acos(delta_x/distance);

  //qDebug("arrow : distance = %f", distance);
  //qDebug("arrow : delta_x = %f", delta_x);
  //qDebug("arrow : delta_y = %f", delta_y);

  if (delta_y < 0.0)
    angle_a = 2*PI - angle_a;
  
  //qDebug("arrow : angle = %f", (angle_a *180)/PI);

  double addition = angle_a + m_arrowAngle;
  double soustraction = angle_a - m_arrowAngle;

  m_arrowPoints[0].setX(endPoint.x() - m_arrowLength * cos(addition));
  m_arrowPoints[0].setY(endPoint.y() - m_arrowLength * sin(addition));

  m_arrowPoints[1].setX(endPoint.x() - m_arrowLength * cos(soustraction));
  m_arrowPoints[1].setY(endPoint.y() - m_arrowLength * sin(soustraction));
  
  m_bArrowAvailable = true;
}


void CGraphicsArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  //qDebug("CGraphicsArrow::paint");
  Q_UNUSED(widget);
  Q_ASSERT(m_points.size() >= 2);
  bool canDisplayArrow = true;

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::TextAntialiasing);
  painter->setPen(m_color);
  
  for (int i = 0; i < m_points.size()-1; i++)
  {
    painter->drawLine(m_points.at(i), m_points.at(i+1));
  }

  //draw the arrow.
  QPointF endPoint = m_points.at(m_points.size() - 1);
  
  calculateArrowPoint(); //TODO faire le calcul a un meilleur moment

  if (m_endItem != NULL)
  {
    if (m_endItem->getAssociatedItem()->type() == COutput::COUTPUT)
    {
      canDisplayArrow = false;
    }
  }
  
  if (m_bArrowAvailable && canDisplayArrow)
  {
    QPolygonF arrow;
    arrow << endPoint << m_arrowPoints[0] << m_arrowPoints[1];
    painter->setBrush(m_brush);
    painter->drawPolygon(arrow);
    painter->setBrush(Qt::NoBrush);
    //painter->drawLine(endPoint, m_arrowPoints[0]);
    //painter->drawLine(endPoint, m_arrowPoints[1]);
    //painter->drawLine(m_arrowPoints[0], m_arrowPoints[1]);
  }

  //DBG:painter->drawRect(boundingRect());
  
  if (option->state & QStyle::State_Selected)
  {
    anchorSetVisible(true);
    QPen pen;
    pen.setStyle(Qt::DotLine);    
    painter->setPen(pen);
    //painter->drawPath(shape());
    //painter->drawRect(m_rect);
  }
  
  if (m_isLabelDisplayed)
  {
    painter->setFont(m_font);
    painter->setPen(m_fontColor);
    painter->drawText(m_labelRect, Qt::AlignLeft|Qt::TextWrapAnywhere/*|Qt::TextWordWrap*/, getFlowName());
  }
}

QRectF CGraphicsArrow::boundingRect() const
{
  qreal extra = m_arrowLength;//TODO: on peut gagner un peu... //(2 + 25)/2.0;//(pen().width() + 20) / 2.0;
  Q_ASSERT(m_points.size() >= 2);
  //qDebug("CGraphicsArrow::boundingRect");

  QRectF rectResult;
  QRectF rectCurrent;
  int i;

  //the bounding rect is the union of each rectangle of each line segment
  for(i=0; i< m_points.size()-1; i++)
  {
    rectCurrent = QRectF(m_points.at(i),
                         QSizeF(m_points.at(i+1).x() - m_points.at(i).x(),
                                m_points.at(i+1).y() - m_points.at(i).y()));
    rectResult = rectResult.united(rectCurrent);
  }
  
  
  //add rect from flowName
  if (m_isLabelDisplayed)
    rectResult = rectResult.united(m_labelRect);
  
  return rectResult.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath CGraphicsArrow::shape() const
{
  QPainterPath path;
  path.setFillRule(Qt::WindingFill);  
  //QPointF lastPoint;
  //for each line, take the line translated above the line and 
  //below the line for the return path.  
  for (int i = 0; i < m_points.size()-1; i++)
  {
    QLineF line(m_points.at(i), m_points.at(i+1));
    QLineF lineT;
    getAroundLine(line, lineT);
    if (i == 0)
      path.moveTo(lineT.p1());
    else
      path.lineTo(lineT.p1());
    
    path.lineTo(lineT.p2());
  }

  for (int i = m_points.size()-1; i > 0; i--)
  {
    QLineF line(m_points.at(i), m_points.at(i-1));
    QLineF lineT;
    getAroundLine(line, lineT);
    path.lineTo(lineT.p1());
    path.lineTo(lineT.p2());
  }
  
  if (m_isLabelDisplayed)
    path.addRect(m_labelRect);

  return path;
}

// IN line
// OUT lineT = line translated
void CGraphicsArrow::getAroundLine(const QLineF& line, QLineF& lineT) const
{
  QLineF normalVect = line.normalVector().unitVector();
  qreal width = m_arrowLength * sin(m_arrowAngle); //TODO faire un calcul en amont

  lineT = line;

  if (line.p1() != line.p2())
    lineT.translate(width*normalVect.dx(), width*normalVect.dy());
}

QVariant CGraphicsArrow::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  qDebug("CGraphicsArrow::itemChange");
  switch (change)
  {
    case QGraphicsItem::ItemSelectedChange:
      //qDebug("CGraphicsArrow::itemChange --> ItemSelectedChange");
      anchorSetVisible(value.toBool());
      
      if (value.toBool() == true)
        setZValue(95.0);
      else
        setZValue(60.0);
      break;
    
    case QGraphicsItem::ItemPositionHasChanged:
#if 0
    {
      QPointF newPoint, vector;
      //update position of each point
      newPoint = value.toPointF();
      /*vector.setX(newPoint.x() - m_points[0].x());
      vector.setY(newPoint.y() - m_points[0].y());
      qDebug("new pos = %f, %f", newPoint.x(), newPoint.y());
      qDebug("start = %f, %f", m_points[0].x(), m_points[0].y());
      qDebug("vector = %f, %f", vector.x(), vector.y());*/
      prepareGeometryChange();
      for(int i = 0; i < m_points.size(); i++)
      {
        m_points.replace(0, m_points.at(0) + newPoint);
        updateAnchor(i);
      }
      
      onPositionHasChanged();
      update();
    }
      break;
#endif      
    default:
      //event ignored
      break;
  }
  
  return QGraphicsItem::itemChange(change, value);
}


void CGraphicsArrow::onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor)
{
  //update position of given point
  Q_ASSERT(anchor != NULL);
  int noPoint;
  bool bCheckIfNeedDissociate = false;
  CGraphicsItem::LinkOrder typeAssoc;
  noPoint = anchor->privData();

  QPointF newPoint;
  newPoint = sceneEvent->scenePos();

  if (noPoint == 0)
  {
    typeAssoc = CGraphicsItem::BEGIN;
    bCheckIfNeedDissociate = true;
  }
  else
    if (noPoint == m_points.size() - 1)
    {
      typeAssoc = CGraphicsItem::END;
      bCheckIfNeedDissociate = true;
    }
  
  if (bCheckIfNeedDissociate)
  {
    CGraphicsScene* pScene = (CGraphicsScene*) scene();
    CGraphicsItem* itemFound;
    itemFound = pScene->searchItem(newPoint);
  
    if (itemFound != NULL)
    {
      if ((itemFound != m_beginItem) && (itemFound != m_endItem))
      {
        Q_ASSERT(itemFound != this);
        //here item is found and different of previous one
        static int cnt = 1;
        qDebug("GraphicsArrow::onMouseEventOnAnchor, new item deconnection a faire + new connection %i", ++cnt);
        qDebug("GraphicsArrow::onMouseEventOnAnchor itemFound = %p, m_beginItem = %p", itemFound, m_beginItem);
        dissociate(typeAssoc);
        associate(itemFound, typeAssoc);
      }
      else
      {
        qDebug("item found equal un des item lies, on ne fait rien");
      }
    }
    else
    {
      dissociate(typeAssoc);
      qDebug("GraphicsArrow::onMouseEventOnAnchor, pas item trouve dissociate");
    }
  }
  
  replacePoint(noPoint, newPoint);
  onPositionHasChanged();  
}

void CGraphicsArrow::dissociate(CGraphicsItem::LinkOrder order)
{
  Q_ASSERT(m_item != NULL);
  bool bOk;

  switch (order)
  {
    case CGraphicsItem::BEGIN:
      if (m_beginItem != NULL)
      {
        qDebug("dissociate '%s' with '%s' (%s)", 
              m_item->getName().toLocal8Bit().data(),
              m_beginItem->getAssociatedItem()->getName().toLocal8Bit().data(),
              "BEGIN");
        bOk = m_beginItem->getAssociatedItem()->dissociate(m_item, CGraphicsItem::BEGIN);
        if (bOk)
        {
          m_beginItem = NULL;
        }
      }
      break;
      
    case CGraphicsItem::END:
      if (m_endItem != NULL)
      {
        qDebug("dissociate '%s' with '%s' (%s)", 
              m_item->getName().toLocal8Bit().data(),
              m_endItem->getAssociatedItem()->getName().toLocal8Bit().data(),
              "END");
        bOk = m_endItem->getAssociatedItem()->dissociate(m_item, CGraphicsItem::END);
        if (bOk)
        {
          m_endItem = NULL;
        }
      }
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
}

void CGraphicsArrow::keyPressEvent(QKeyEvent* event)
{
  qDebug("CGraphicsArrow::keyPressEvent");
  //qDebug("m_linkInGoing = %i", m_linkInGoing);
  
  if ((event->key() == Qt::Key_Escape) && (m_linkInGoing == true))
  {
    m_linkInGoing = false;
    //remove last point
    //if no point, remove item and warn parent to remove from list of these items
    if (m_points.size() > 2)
    {
      removePointAndAnchor(m_points.size()-1);
    }
    else
    {
      //remove the item request it to the scene
      //qDebug("autodelete");
      ((CGraphicsScene*) scene())->removeGraphicalItem(this);
    }
  }
  else
  {
    //event is positionned to ignore the event and propagate to the scene
    event->ignore();
  }
  //QGraphicsItem::keyPressEvent(event);
}

void CGraphicsArrow::removePointAndAnchor(int no)
{
    prepareGeometryChange();
    m_points.removeAt(no);
    CGraphicsAnchor* anchorToDelete;
    anchorToDelete = m_anchors.at(no);
    m_anchors.removeAt(no);
    delete anchorToDelete;
    m_currentIndexOfInsertion--;
    update();  
}

void CGraphicsArrow::unlink(void)
{
  if (m_beginItem != NULL)
    m_beginItem->removeLinkTo(this);

  if (m_endItem != NULL)
    m_endItem->removeLinkTo(this);
}

void CGraphicsArrow::removeLinkTo(CGraphicsItem* item)
{
  if (m_beginItem == item)
    m_beginItem = NULL;

  if (m_endItem == item)
    m_endItem = NULL;
}


QList<CPropertyDefinition::Type> CGraphicsArrow::getListOfGraphicalProperties(void)
{
  QList<CPropertyDefinition::Type> list;

  list << CPropertyDefinition::NAME
      << CPropertyDefinition::LINE_COLOR
      << CPropertyDefinition::FONT
      << CPropertyDefinition::FONT_COLOR
      << CPropertyDefinition::DISPLAY_LABEL
      << CPropertyDefinition::FLOW_NAME
      ;

  return list;
}


QVariant CGraphicsArrow::getGraphicalProperties(CPropertyDefinition::Type type)
{
  QVariant property;

  switch (type)
  {
    case CPropertyDefinition::FLOW_NAME:
    property.setValue(getFlowName());
    break;
    
    case CPropertyDefinition::DISPLAY_LABEL:
    property.setValue(m_isLabelDisplayed);
    break;
      
    case CPropertyDefinition::NAME:
    property.setValue(m_item->getName());
    break;

    case CPropertyDefinition::LINE_COLOR:
    property.setValue(m_color);
    break;

    case CPropertyDefinition::FONT_COLOR:
    property.setValue(m_fontColor);
    break;

    case CPropertyDefinition::FONT:
    property.setValue(m_font);
    break;

    default:
    Q_ASSERT(false);
    break;
  }

  return property;

}

void CGraphicsArrow::setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value)
{
  prepareGeometryChange();
  switch (type)
  {
    case CPropertyDefinition::NAME:
    m_item->setName(value.toString());
    break;

    case CPropertyDefinition::LINE_COLOR:
    m_color = value.value<QColor>(); //TODO: faire une f° commune pour eviter de mettre un jour un sans l'autre
    m_brush = QBrush(m_color);
    break;
    
    case CPropertyDefinition::DISPLAY_LABEL:
    m_isLabelDisplayed = value.toBool();
    break;
      
    case CPropertyDefinition::FLOW_NAME:
      //no possible ?
    break;
    
    case CPropertyDefinition::FONT_COLOR:
    m_fontColor = value.value<QColor>();
    break;

    case CPropertyDefinition::FONT:
    m_font = value.value<QFont>();
    break;

    
    default:
    Q_ASSERT(false);
    break;
  }
  update();
}


QString CGraphicsArrow::getFlowName()
{
  Q_ASSERT(m_item != NULL);
  
  CData* dataFlow = dynamic_cast<CFlow*>(m_item)->getDataFlow();
  if (dataFlow == NULL)
  {
    return QString(tr("none"));
  }

  return dataFlow->getName();
}


void CGraphicsArrow::contextMenuEvent ( QGraphicsSceneContextMenuEvent* event )
{
  //qDebug("CGraphicsArrow::contextMenuEvent");
  CDocument* doc = getApp()->getDocument();
  QMenu menu;
  int result;
  QAction *selectData = menu.addAction(tr("select a data..."));
  QAction *addData = menu.addAction(tr("create and select a new data..."));
  QAction *removeWayPoint = menu.addAction(tr("remove all waypoints"));
  
  QAction *selectedAction = menu.exec(event->screenPos());
  if (selectedAction == addData)
  {

    CDialogCreateData dialog(doc->getItemFactory()->getDataFactory(),
                             doc->getItemFactory()->getTypeFactory());
    result = dialog.exec();
    if (result == QDialog::Accepted)
    {
      CData* data = dialog.getDataFlowCreated();
      updateDataFlow(data);
    }
  }
  else if (selectedAction == selectData)
  {
    CDialogChooseData dialog(doc->getItemFactory()->getDataFactory(),
                             doc->getItemFactory()->getTypeFactory());
    result = dialog.exec();
    if (result == QDialog::Accepted)
    {
      //qDebug("accepted");
      CData* data = dialog.getChosenData();
      updateDataFlow(data);
    }
    
  } else if (selectedAction == removeWayPoint)
  {
    CGraphicsAnchor* currentAnchor;
    int nbPoints;
    nbPoints = m_points.size();
    prepareGeometryChange();
    for(int j = 0; j < nbPoints-2; j++)
    {
      m_points.removeAt(1);
      currentAnchor = m_anchors.takeAt(1);
      if (currentAnchor)
        delete currentAnchor;
    }    
    m_anchors.at(0)->setPrivData(0);
    m_anchors.at(1)->setPrivData(1);
    onPositionHasChanged();
    update();    
  }
 
  QGraphicsItem::contextMenuEvent (event);
}

void CGraphicsArrow::updateDataFlow(CData* data)
{
  CFlow* flow = dynamic_cast<CFlow*>(m_item);
  flow->setDataFlow(data);
  emit itemChanged(this, CPropertyDefinition::FLOW_NAME, getFlowName());
}


void CGraphicsArrow::mousePressEvent ( QGraphicsSceneMouseEvent* event )
{
//  qDebug("CGraphicsArrow::mousePressEvent");
  
  if (m_labelRect.contains(event->scenePos()))
  {
    m_moveOfLabelOnGoing = true;
  }
  
  QGraphicsItem::mousePressEvent ( event );
}

void CGraphicsArrow::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
//  qDebug("CGraphicsArrow::mouseMoveEvent");
  if (m_moveOfLabelOnGoing)
  {
    QPointF lastPoint= event->lastScenePos();
    QPointF currentPoint = event->scenePos();
    QPointF offset;
    offset.setX(currentPoint.x() - lastPoint.x());
    offset.setY(currentPoint.y() - lastPoint.y());
    m_isLabelManuallyMoved = true;
    prepareGeometryChange();
    m_labelRect = m_labelRect.translated(offset);
    update();
  }
  QGraphicsItem::mouseMoveEvent ( event );
}

void CGraphicsArrow::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
{
//  qDebug("CGraphicsArrow::mouseReleaseEvent");

  if (m_labelRect.contains(event->scenePos()))
  {
    m_moveOfLabelOnGoing = false;
  }
  
  QGraphicsItem::mouseReleaseEvent ( event );
}


void CGraphicsArrow::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event )
{
  //qDebug("CGraphicsArrow::mouseDoubleClickEvent");
  
  for (int i = 0; i < m_points.size()-1; i++)
  {
    QLineF line(m_points.at(i), m_points.at(i+1));
    QLineF line2(m_points.at(i+1), m_points.at(i));
    QLineF lineT1;
    QLineF lineT2;
    
    getAroundLine(line, lineT1);
    getAroundLine(line2, lineT2);
    
    QPolygonF polygon;
    polygon << lineT1.p1() << lineT1.p2() << lineT2.p1() << lineT2.p2();
    
    bool isInArea = polygon.containsPoint(event->scenePos(), Qt::OddEvenFill);    
    if (isInArea)
    {
      //qDebug("area found %i to %i", i, i+1);
      prepareGeometryChange();
      m_points.insert(i+1, event->scenePos());
      addAnchor(i+1);
      for(int j = 0; j < m_anchors.size(); j++)
      {
        m_anchors.at(j)->setPrivData(j);
        updateAnchor(j);
      }
      onPositionHasChanged();
      update();
      break;
    }
  }

  QGraphicsItem::mouseDoubleClickEvent (event);
}


void CGraphicsArrow::insertItemIntoDomDocument ( QDomDocument& doc, QDomElement& element )
{
  CGraphicsWriter writer(this);
  writer.buildDomDocument(doc,element);
}

