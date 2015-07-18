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
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include "graphicscircle.h"
#include "diagrams/generic/toolitemmodel.h"
#include "global.h"
#include "graphia.h"
#include "diagrams/flow/dialog/dialogsetname.h"
#include "diagrams/flow/model/processcontext.h"
#include <QSet>
#include <QGraphicsSceneEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "graphicswriter.h"

const int PADDING = 0;

CGraphicsCircle::CGraphicsCircle(QPointF& center, CItem* obj, QGraphicsItem* parent)
 : QGraphicsItem(parent), CGraphicsItem(obj)
{
  qreal x,y;
  
  m_radius = getApp()->getGraphicsConfig()->circleRadius();
  m_font = getApp()->getGraphicsConfig()->font();

  m_alignmentH = getApp()->getGraphicsConfig()->alignmentH();
  m_alignmentV = getApp()->getGraphicsConfig()->alignmentV();

  m_fontColor = getApp()->getGraphicsConfig()->fontColor();
  m_lineColor = getApp()->getGraphicsConfig()->lineColor();

  m_brush = QBrush(Qt::SolidPattern);
  m_brush.setColor(getApp()->getGraphicsConfig()->background());

  x = center.x() - m_radius;
  y = center.y() - m_radius;
  
  setPos(x, y);
  
  buildEnclosedRect();
  buildAnchor();
  updateAnchorPosition();
  anchorSetVisible(false);
  
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
  setZValue(50.0);
  
  m_itemProperties = (CAN_BE_LINKED | IS_REMOVABLE);
  //qDebug("x=%f, y=%f", x, y);
  //qDebug("cos(PI/4) = %f, %f", cos(PI/4), PI/4);
  //qDebug("x_str=%f, y_str=%f", xName, yName);
}


void CGraphicsCircle::setAndUpdateGraphicalData ( void )
{
//  prepareGeometryChange();
  buildEnclosedRect();
  buildAnchor();
  updateAnchorPosition();
  anchorSetVisible(false);
 // update();
}


CGraphicsCircle::~CGraphicsCircle()
{
  for (int i = 0; i < 4; i++)
  {
    if (m_anchor[i])
      delete m_anchor[i];
  }
}

void CGraphicsCircle::buildEnclosedRect(void)
{
  qreal xName, yName;
  qreal diameter;
  
  diameter = m_radius*2;
  xName = 0 /*x*/ + m_radius * (1 - cos(PI/4)) + PADDING;
  yName = 0 /*y*/ + m_radius * (1 - sin(PI/4)) + PADDING;
  
  m_rect = QRectF(0/*x*/, 0/*y*/, diameter, diameter);
  m_rectName = QRectF(xName, yName, diameter * cos(PI/4) - 2 * PADDING, diameter * sin(PI/4) - 2* PADDING);
}

enum
{
  ANCHOR_TOP_LEFT,
  ANCHOR_TOP_RIGHT,
  ANCHOR_BOTTOM_LEFT,
  ANCHOR_BOTTOM_RIGHT
};

void CGraphicsCircle::buildAnchor(void)
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

void CGraphicsCircle::updateAnchorPosition(void)
{
  qreal anchorSize = getApp()->getGraphicsConfig()->anchorSize();
  
  m_anchor[ANCHOR_TOP_LEFT]->setTopLeft(0,0);
  m_anchor[ANCHOR_TOP_RIGHT]->setTopLeft(m_radius*2-anchorSize, 0);  
  m_anchor[ANCHOR_BOTTOM_LEFT]->setTopLeft(0, m_radius*2-anchorSize);
  m_anchor[ANCHOR_BOTTOM_RIGHT]->setTopLeft(m_radius*2-anchorSize, m_radius*2-anchorSize);
}

void CGraphicsCircle::anchorSetVisible(bool visible)
{
  for(int i = 0; i < 4 ; i++)
  {
    m_anchor[i]->setVisible(visible);
  }
}

bool CGraphicsCircle::associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);
  qDebug("CGraphicsCircle::associate");
  Q_ASSERT(false);
  return false;
}

void CGraphicsCircle::linkTo(CGraphicsItem* item)
{
  m_listAttachedItem.insert(item);
}

void CGraphicsCircle::removeLinkTo(CGraphicsItem* item)
{
  bool b;
  b = m_listAttachedItem.remove(item);
  Q_ASSERT(b == true);
}

bool CGraphicsCircle::getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint)
{
  QPointF center = m_rect.center() + pos();
  double distance, a, b, ix, iy;
  double rayon = m_radius;

  a = (refPoint.x() - center.x()); //a = a*a;
  b = (refPoint.y() - center.y()); //b = b*b;
  //distance = sqrt(a+b);
  distance = hypot(a, b);
  //qDebug("distance = %f", distance);

  if (distance < m_radius)
    return false;

  ix = refPoint.x() + (distance-rayon) * (center.x() - refPoint.x()) / distance;
  iy = refPoint.y() + (distance-rayon) * (center.y() - refPoint.y()) / distance;
  //qDebug("x = %f, y = %f", ix, iy);


  interPoint.setX(ix);
  interPoint.setY(iy);

  return true;
}


QRectF CGraphicsCircle::boundingRect() const
{
  //qDebug("CProcessItem::boundingRect");
  //QRectF rect(m_rect);
  //rect.translate(-m_rect.center());
  //return rect;
  return m_rect;  
}

QPainterPath CGraphicsCircle::shape() const
{
  //qDebug("CGraphicsCircle::shape(%p)", this);
  QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
  //return QGraphicsItem::shape();
}

#include <QDebug>
void CGraphicsCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(widget);
  //qDebug("CProcessItem::paint");
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::TextAntialiasing);
  painter->setFont(m_font);
  
  //QPen pen(Qt::black);
  if (option->state & QStyle::State_Selected)
  {
    //anchorSetVisible(true);
    //pen.setStyle(Qt::DotLine);
    //painter->setPen(pen);
    //painter->drawPath(shape());
    //painter->drawRect(m_rect);
    //pen.setStyle(Qt::SolidLine);
    //painter->setPen(pen);
  }

  painter->setPen(m_lineColor);
  painter->setBrush(m_brush);
  painter->drawEllipse(m_rect);
  //painter->drawRect(m_rect_str);
  painter->setPen(m_fontColor);
  painter->drawText(m_rectName, m_alignmentH | m_alignmentV |Qt::TextWordWrap/*|Qt::TextWordWrap*/, m_item->getName());
}

QVariant CGraphicsCircle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  //qDebug("CGraphicsCircle::itemChange (%d,%d)", change, value.toInt());
  //qDebug() << change;

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

void CGraphicsCircle::warmThatPosHasChanged(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->onPositionHasChanged();
  }
}


void CGraphicsCircle::onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor)
{
  //qDebug("CGraphicsCircle::onMouseEventOnAnchor");
  Q_ASSERT(anchor != NULL);
  Q_ASSERT(anchor->parentItem() == this);
  
  qreal dx;
  //qreal dy;
  qreal newRadius;
  QPointF lastPoint;
  QPointF point;
  
  newRadius = m_radius;
  lastPoint = sceneEvent->lastScenePos();
  point = sceneEvent->scenePos();
  dx = point.x() - lastPoint.x();
  //dy = point.y() - lastPoint.y();
  
  switch (anchor->privData())
  {
    case ANCHOR_TOP_LEFT:
      newRadius -= dx/2.0;
      if (newRadius > 0)
      {
        m_radius = newRadius;
        setPos(pos().x() + dx, pos().y() + dx);
      }
      break;
      
    case ANCHOR_TOP_RIGHT:
      newRadius += dx/2.0;
      if (newRadius > 0)
      {
        m_radius = newRadius;
        setPos(pos().x() , pos().y() - dx);
      }
      break;
      
    case ANCHOR_BOTTOM_LEFT:
      newRadius -= dx/2.0;
      if (newRadius > 0)
      {
        m_radius = newRadius;
        setPos(pos().x() + dx, pos().y());
      }
      break;
     
    case ANCHOR_BOTTOM_RIGHT:
      newRadius += dx/2.0;
      if (newRadius > 0)
      {
        m_radius = newRadius;
        //setPos(pos().x() , pos().y());
      }
      break; 
      
    default:
      Q_ASSERT(false);
      break;
  } 
  
  prepareGeometryChange();
  buildEnclosedRect();
  updateAnchorPosition();  
  update();
  
  warmThatPosHasChanged();
  emit itemChanged(this, CPropertyDefinition::RADIUS, m_radius);
}


void CGraphicsCircle::unlink(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->removeLinkTo(this);
  }
}


QList<CPropertyDefinition::Type> CGraphicsCircle::getListOfGraphicalProperties()
{
  QList<CPropertyDefinition::Type> list;

  list << CPropertyDefinition::NAME;
  list << CPropertyDefinition::POS;
  list << CPropertyDefinition::RADIUS;
  list << CPropertyDefinition::FONT;
  list << CPropertyDefinition::FONT_COLOR;
  list << CPropertyDefinition::LINE_COLOR;
  list << CPropertyDefinition::BACKGND_COLOR;
  list << CPropertyDefinition::ALIGNMENT_H;
  list << CPropertyDefinition::ALIGNMENT_V;

  return list;
}

QVariant CGraphicsCircle::getGraphicalProperties(CPropertyDefinition::Type type)
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

    case CPropertyDefinition::RADIUS:
      property.setValue(m_radius);
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

void CGraphicsCircle::setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value)
{
  switch (type)
  {
    case CPropertyDefinition::NAME:
      //prepareGeometryChange();
      //m_item->setName(value.toString());
      //update();
      //not more possible
      Q_ASSERT(false);
      break;

    case CPropertyDefinition::POS:
      setPos(value.toPointF());
      break;

    case CPropertyDefinition::RADIUS:
      m_radius = value.toDouble();
      prepareGeometryChange();
      buildEnclosedRect();
      updateAnchorPosition();
      update();
      warmThatPosHasChanged();
      break;

    case CPropertyDefinition::FONT:
      prepareGeometryChange();
      m_font = value.value<QFont>();
      update();
      break;

    case CPropertyDefinition::FONT_COLOR:
      prepareGeometryChange();
      m_fontColor = value.value<QColor>();
      update();
      break;

    case CPropertyDefinition::LINE_COLOR:
      prepareGeometryChange();
      m_lineColor= value.value<QColor>();
      update();
      break;

    case CPropertyDefinition::BACKGND_COLOR:
      prepareGeometryChange();
      m_brush.setColor(value.value<QColor>());
      update();
      break;

    case CPropertyDefinition::ALIGNMENT_H:
      prepareGeometryChange();
      m_alignmentH = CPropertyDefinition::alignementHToInt(
          (CPropertyDefinition::alignHorizontal) value.value<int>());
      update();
      break;

    case CPropertyDefinition::ALIGNMENT_V:
      prepareGeometryChange();
      m_alignmentV = CPropertyDefinition::alignementVToInt(
          (CPropertyDefinition::alignVertical) value.value<int>());
      update();
      break;

    default:
      Q_ASSERT(false);
      break;
  }
}

void CGraphicsCircle::contextMenuEvent ( QGraphicsSceneContextMenuEvent* event )
{
  QMenu menu;
  int result;
  QAction *setName = menu.addAction(tr("change name..."));
  QAction *createNewProcess = menu.addAction(tr("create new process"));
  QAction *openExistingProcess = menu.addAction(tr("display implementation"));
  
  //search the process
  CGraphicsScene* pScene;
  
  pScene= getApp()->getDocument()->searchExistingDiagram(m_item->getName());
  if (pScene == NULL)
  {
    createNewProcess->setEnabled(true);
    openExistingProcess->setEnabled(false);
  }
  else
  {
    createNewProcess->setEnabled(false);
    openExistingProcess->setEnabled(true);    
  }
  
  
  QAction *selectedAction = menu.exec(event->screenPos());
  if (selectedAction == setName)
  {
    CDialogSetName dialog;
    result = dialog.exec();
    if (result == QDialog::Accepted)
    {
      updateProcessName(dialog.getName());
      emit itemChanged(this, CPropertyDefinition::NAME, m_item->getName());
    }
  }
  else if (selectedAction == createNewProcess)
  {
    getApp()->getDocument()->createNewImplementationProcessDiagram(m_item->getName());
  } 
  else if (selectedAction == openExistingProcess)
  {
    getApp()->getDocument()->displayDiagram(pScene);
  }
  
  QGraphicsItem::contextMenuEvent ( event );
}


void CGraphicsCircle::updateProcessName ( const QString& newName )
{
  bool renameOk,startOperation;
  CProcessImplementation* alreadyExist;
  CProcessImplementation* pImplemented;
  
  CProcessInstance* instance = dynamic_cast<CProcessInstance*>(m_item);
  Q_ASSERT(instance != NULL);
  
  renameOk = false;
  startOperation = true;
  pImplemented = instance->getImplementedProcess();
  alreadyExist = getApp()->getDocument()->getItemFactory()->getImplementationProcess(newName);
  
  if ((pImplemented != NULL) && (alreadyExist != NULL))
  {
    QMessageBox::warning(NULL, tr("graphia"), 
                         tr("you try to rename a process to an already existing process with a implementation already existing\n"
                            "This operation is not possible, try to unlink/remove process before"),
                         QMessageBox::Discard);
    startOperation = false;
    return;
  }
  else if (alreadyExist != NULL)
  {
    int result;
    result = QMessageBox::question(NULL, tr("graphia"), 
                                   tr("a implementation process already exists with this name, would you like to rename and link to them\n"
                                      "otherwise, the rename will be cancelled"),
                                   QMessageBox::Yes|QMessageBox::No);
    if (result == QMessageBox::No)
      startOperation = false;
    else
      startOperation = true;
  }
  
  if (startOperation)
  {
    if (pImplemented != NULL)
    {
      renameOk = getApp()->getDocument()->getItemFactory()->renameImplementationProcess(pImplemented, newName);
      if (!renameOk)
      {
        QMessageBox::critical(NULL, tr("graphia"), tr("Unable to rename process"), QMessageBox::Discard);
        return;
      }
    }
    else
    {
      //rename simply the item
      m_item->setName(newName);
      if (alreadyExist != NULL)
      {
        instance->setImplementedProcess(alreadyExist);
      }
      renameOk = true;
    } 
  
    if (renameOk)
    {
      //special case in the case of context diagram the name of the parent if also changed
      CItemContainer* parent;
      parent = instance->getParent();
      if (parent->type() == CProcessContext::CPROCESS_CONTEXT)
      {
        parent->setName(newName);
      }
    }
  } 
}


void CGraphicsCircle::insertItemIntoDomDocument ( QDomDocument& doc, QDomElement& element )
{
  CGraphicsWriter writer(this);
  writer.buildDomDocument(doc, element);
}

void CGraphicsCircle::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent ( event );
  
  CGraphicsScene* theScene = (CGraphicsScene*) scene();
  theScene->checkItemLocation(this);
}


void CGraphicsCircle::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event )
{
  //qDebug("CGraphicsCircle::mouseDoubleClickEvent");
  QGraphicsItem::mouseDoubleClickEvent ( event );
  qreal OFFSET = 1.0;
  QFontMetricsF fontMetrics(m_font);
  qreal lenght  = fontMetrics.width(m_item->getName())+ OFFSET;
  qreal newRadius;

  //qreal height = fontMetrics.height();

  newRadius = (lenght + 2* PADDING)/(2.0 * cos(PI/4));
  if (newRadius > m_radius)
  {
    m_radius = newRadius;
    prepareGeometryChange();
    buildEnclosedRect(); //TODO faire une fonction commune pour toutes ces mises à jour
    updateAnchorPosition();
    update();
    warmThatPosHasChanged();
    emit itemChanged(this, CPropertyDefinition::RADIUS, m_radius);
  }
}
