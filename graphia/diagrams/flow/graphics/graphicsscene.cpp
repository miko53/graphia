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
#include "graphicsscene.h"
#include "graphia.h"
#include "diagrams/generic/graphicsitem.h"
#include <qgraphicssceneevent.h>
#include <QKeyEvent>
#include <QGraphicsView>

CGraphicsScene::CGraphicsScene(CItem* obj, QObject* parent): QGraphicsScene(parent)
{
  m_mode = MODE_IDLE;
  m_item = obj;
  m_itemRect = NULL;
  m_isDisplayed = false;

  QRectF sceneArea = getApp()->getGraphicsConfig()->sceneRect();
  setSceneRect(sceneArea);
  setMouseTrackingInAllViews();
}

CGraphicsScene::~CGraphicsScene()
{
  qDebug("CGraphicsScene::~CGraphicsScene()");
}

void CGraphicsScene::onChangeItem(int id, CToolButton::STATE state)
{
  if (id == CToolItemModel::ITEM_SELECT)
  {
    m_mode = MODE_IDLE;
  }
  else
  {
    m_mode = MODE_CREATE_ITEM;
    m_idToCreate = id;
    m_keepMode = state;
  }
  //qDebug("CGraphicsScene::onChangeItem (id = %i) - (state = %i)", id, state);
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  //qDebug("CGraphicsScene::mousePressEvent");
  
  if (event->button() == Qt::LeftButton)
  {
    switch (m_mode)
    {
      case MODE_IDLE:
      {
        CGraphicsAnchor* anchor;
        anchor = getAnchorAt(event->scenePos());
        if (anchor != NULL)
        {
          //TODO :passe en mode evolution de l'item, passe l'evenement � l'anchor
          m_mode = MODE_IN_ITEM_EVOLUTION;
          QGraphicsItem* gItem;
          gItem = anchor->parentItem();
          Q_ASSERT(gItem->type() == CGraphicsItem::GraphicsItem);    
          m_currentItem = dynamic_cast<CGraphicsItem*> (gItem);
          m_pressedAnchor = anchor;
          setMouseTrackingInAllViews();
        }
        else
        {
          QGraphicsScene::mousePressEvent(event);

          //creation a rect of selection
          if (selectedItems().count() == 0 )
          {
            m_itemRect = new CGraphicsRectSelection();
            m_itemRect->setFirstPoint(event->scenePos());
            addItem(m_itemRect);
          }
        }
      }
        break;
        
      case MODE_CREATE_ITEM:
        createItem(event);
        QGraphicsScene::mousePressEvent(event);
        break;
        
      case MODE_IN_ITEM_CREATION:
        updateItemDuringCreation(event);
        break;
        
      case MODE_IN_ITEM_EVOLUTION:
        Q_ASSERT(false);
        break;
        
      default:
        Q_ASSERT(false);
        break;
    }
  }
  
  //QGraphicsScene::mousePressEvent(event);
}

void CGraphicsScene::createItem(QGraphicsSceneMouseEvent* event)
{
  QPointF currentPos;
  int properties;
  QGraphicsItem* newItem;
  CGraphicsItem* pItem;
  
  //QGraphicsItem* item_to_insert;
  CGraphicsItem* itemAtPos;
  currentPos = event->scenePos();

  newItem = getApp()->getModel()->createItem(m_idToCreate, currentPos, m_item);
  if (newItem == NULL)
  {
    //TODO l'item n'existe pas.... manage it ! en attentant assert.
    Q_ASSERT(false);
    return;
  }

  pItem = dynamic_cast<CGraphicsItem*>(newItem);
  properties = pItem->getProperties();
  
  if (properties & CGraphicsItem::NEED_MORE_STEP)
  {
    m_mode = MODE_IN_ITEM_CREATION;
    m_currentItem = pItem;
    setMouseTrackingInAllViews();
  }
  else
  {
    setModeForNextOperation();
  }

  // if the item is a item used to create a link
  if (properties & CGraphicsItem::IS_LINK_ITEM)
  {
    //search item TODO protection in case of not CGraphicsItem if possible eg for anchor
    itemAtPos = dynamic_cast<CGraphicsItem*> (itemAt(event->scenePos(), QTransform()));
    if ((itemAtPos != NULL) &&
        (itemAtPos->getProperties() & CGraphicsItem::CAN_BE_LINKED))
    {
      //try to associate
      qDebug("Item at Pos -- try association");
      pItem->associate(itemAtPos, CGraphicsItem::BEGIN);
    }
    else
    {
      //no item
      qDebug("noItem at Pos");
    }
  }
  else
  {
    //no link possible
    
  }

  addItem(newItem);
  
}

void CGraphicsScene::updateItemDuringCreation(QGraphicsSceneMouseEvent* event)
{
  //search if an item can be linked
  //in this case, try a association with the item_to_insert
  //send a clickevent to the item ?
  QPointF point = event->scenePos();
  CGraphicsItem* item;

  if (m_currentItem->getProperties() & CGraphicsItem::IS_LINK_ITEM)
  {
    item = searchItemAndAssociateAt(point, CGraphicsItem::END);
    if (item != NULL)
    {
      //item founded and associate, the creation is ended
      qDebug("item found");
      setModeForNextOperation();
      setMouseTrackingInAllViews();//TODO remove all tracking except in constructor ; unuseless
    }
    else
      notifyClickToItem(point);
  }
  else
  {
    notifyClickToItem(point);
  }
}

void CGraphicsScene::notifyClickToItem(const QPointF& point)
{
  //notify item about this click
  bool bEnded;
  qDebug("item not found or can't be associated");
  bEnded = m_currentItem->onLeftClickEvent(point);
  if (bEnded == true)
  {
    setModeForNextOperation();
    setMouseTrackingInAllViews();
  }
}

CGraphicsItem* CGraphicsScene::searchItem(const QPointF& point)
{
  QList<QGraphicsItem *> listItems;
  listItems = items(point, Qt::IntersectsItemShape, Qt::AscendingOrder);

  foreach(QGraphicsItem* item, listItems)
  {
    if ((item->type() == CGraphicsItem::GraphicsItem) &&
        (dynamic_cast<CGraphicsItem*> (item)->getProperties() & CGraphicsItem::CAN_BE_LINKED))
    {      
      return dynamic_cast<CGraphicsItem*> (item);      
    }
  }
  
  return NULL;
}


CGraphicsItem* CGraphicsScene::searchItemAndAssociateAt(const QPointF& point, CGraphicsItem::LinkOrder type)
{
  QList<QGraphicsItem *> listItems;
  listItems = items(point, Qt::IntersectsItemShape, Qt::AscendingOrder);

  foreach(QGraphicsItem* item, listItems)
  { 
    if ((item != (QGraphicsItem*) m_currentItem) && 
        (item->type() == CGraphicsItem::GraphicsItem) &&
        (dynamic_cast<CGraphicsItem*> (item)->getProperties() & CGraphicsItem::CAN_BE_LINKED) &&
        (m_currentItem->associate( dynamic_cast<CGraphicsItem*> (item), type) == true))
    {
      return dynamic_cast<CGraphicsItem*> (item);
    }
    else
    {
      //qDebug("(%p;%p) or associate failed", item, m_currentItem);
    }
  }
  return NULL;
}


void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  //  static int nbMove=0;
  //qDebug("CGraphicsScene::mouseMoveEvent %i", m_mode);
  
  switch (m_mode)
  {
    case MODE_IDLE:
      QGraphicsScene::mouseMoveEvent(event);

      if (m_itemRect != NULL)
      {
        m_itemRect->setMoveablePoint(event->scenePos());
        setSelectionArea(m_itemRect->path(), QTransform());
      }
      
      break;
    case MODE_CREATE_ITEM:
      //Q_ASSERT(false); //Normallement ne doit jamais arrive...
      QGraphicsScene::mouseMoveEvent(event);
      break;
      
    case MODE_IN_ITEM_CREATION:
      Q_ASSERT(m_currentItem != NULL);
      m_currentItem->updateCurrentPoint(event->scenePos());
      //QGraphicsScene::mouseMoveEvent(event); pas d'appel a la classe de base pour eviter les moves en meme temps que les creations d'item
      break;
      
    case MODE_IN_ITEM_EVOLUTION:
      Q_ASSERT(m_currentItem != NULL);
      m_currentItem->onMouseEventOnAnchor(event, m_pressedAnchor);
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
}


void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  switch (m_mode)
  {
    case MODE_IDLE:
      //remove the rectangle of selection
      if (m_itemRect != NULL)
      {
        removeItem(m_itemRect);
        delete m_itemRect;
        m_itemRect = NULL;
      }
      break;
      
    case MODE_CREATE_ITEM:
      break;
      
    case MODE_IN_ITEM_CREATION:
      break;
      
    case MODE_IN_ITEM_EVOLUTION:
      m_mode = MODE_IDLE; //in this case, no another mode of return is possible
      setMouseTrackingInAllViews();
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  //qDebug("CGraphicsScene::mouseReleaseEvent");
  QGraphicsScene::mouseReleaseEvent(event);
}

void CGraphicsScene::setMouseTrackingInAllViews(void)
{
  QList<QGraphicsView*> list_views = views();
  foreach(QGraphicsView* view, list_views)
  {
    view->setMouseTracking(true); //tracking always
  }
}

void CGraphicsScene::setModeForNextOperation()
{
  if (m_keepMode == CToolButton::STATE_2)
  {
    m_mode = MODE_CREATE_ITEM;
    m_currentItem = NULL;
  }
  else
  {
    m_mode = MODE_IDLE;
    //qDebug("signal itemInserted\n");
    emit itemInserted();
  }
}

void CGraphicsScene::keyPressEvent(QKeyEvent* event)
{
  //qDebug("CGraphicsScene::keyPressEvent 0x%x", event->key());
  //qDebug("selectedItems().count() = %i", selectedItems().count());
  //qDebug("focusItem() = 0x%x", focusItem());

  QGraphicsScene::keyPressEvent(event);
  //qDebug("event->isAccepted() %i", event->isAccepted());
  
  //if a item has the focus send all key data on this
  //management of the pressed key here
  switch(event->key())
  {
    case Qt::Key_Escape:
      //if in create step, return to idle mode
      switch (m_mode)
      {
        case MODE_IN_ITEM_CREATION:
          setModeForNextOperation();
          //warn child object about key press
          //QGraphicsScene::keyPressEvent(event);
          break;

        default:
          //no action
          break;
      }
      break;

    case Qt::Key_Delete:
      //if (!focusItem())
      if (event->isAccepted() == false)
      {
        QList<QGraphicsItem *> listSelectedItems;
        listSelectedItems = selectedItems();

        foreach(QGraphicsItem* item, listSelectedItems)
        {
          removeGraphicalItem(item);
        }

      }
      //else
      //  QGraphicsScene::keyPressEvent(event);
    break;

    default:
      //QGraphicsScene::keyPressEvent(event);
      break;
  }

}

CGraphicsAnchor* CGraphicsScene::getAnchorAt(const QPointF& point)
{
  QList<QGraphicsItem*> listOfItems;
  listOfItems = items(point, Qt::IntersectsItemShape, Qt::AscendingOrder);

  foreach(QGraphicsItem* item, listOfItems)
  {
    if (item->type() == CGraphicsAnchor::GraphicsAnchor)
      return (CGraphicsAnchor*) item;
  }

  return NULL;
}

void CGraphicsScene::removeGraphicalItem(QGraphicsItem* item)
{
  Q_ASSERT(item != NULL);
  
  CGraphicsItem* cItem;
  cItem = dynamic_cast<CGraphicsItem*>(item);
  
  if ((cItem != NULL) && (cItem->getProperties() & CGraphicsItem::IS_REMOVABLE))
  {
    //firstly remove from the scene
    removeItem(item);

    //now clear it, request to the model
    getApp()->getModel()->removeItem(dynamic_cast<CGraphicsItem*>(item));
  }
  else
  {
    qDebug("item not removable (%p,%p), keep it\n", item, cItem);
  }
}


void CGraphicsScene::checkItemLocation ( QGraphicsItem* item )
{
  QRectF itemRect = item->boundingRect();
  QPointF topLeft = sceneRect().topLeft();
  qreal sceneWidth = sceneRect().width();
  qreal sceneHeight = sceneRect().height();

  
  if (item->x() < topLeft.x())
    item->setPos(topLeft.x(), item->y());
  
  if ((item->x() + itemRect.width()) > sceneWidth)
  {
    qreal newPosX = sceneWidth - itemRect.width();
    item->setPos(newPosX, item->y());
  }  
  
  if (item->y() < topLeft.y())
    item->setPos(item->x(), topLeft.y());
  
  if ((item->y() + itemRect.height()) > sceneHeight)
  {
    qreal newPosY = sceneHeight - itemRect.height();
    item->setPos(item->x(), newPosY);
  }
}
