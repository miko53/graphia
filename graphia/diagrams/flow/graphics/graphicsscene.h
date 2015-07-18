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
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include "widgets/toolbutton.h"
#include "diagrams/generic/toolitemmodel.h"
#include "diagrams/generic/graphicsitem.h"
#include "graphicsanchor.h"
#include "graphicsrectselection.h"
#include "diagrams/flow/model/item.h"

class CGraphicsScene : public QGraphicsScene
{
  Q_OBJECT
  
public:
  CGraphicsScene(CItem* obj = NULL, QObject* parent = 0);
  virtual ~CGraphicsScene();

//accessor
  void setItem(CItem* obj) { m_item = obj; }
  CItem* getItem(void) { return m_item; }
  void setType(int type) { m_type = type; }
  int getType(void) { return m_type; }
  
//  QString getName() {  Q_ASSERT(m_item != NULL); return m_item->getName(); }
  QString getTitle() {  return m_title; }
  void setTitle(const QString& title) {  m_title = title; } 
  
  void setDisplayState(bool isDisplayed) { m_isDisplayed = isDisplayed; } 
  bool isDisplayed() { return m_isDisplayed; }
  
  void appendAuthorization(CToolItemModel::ItemListId id) { m_authorization.append(id); };
  QVector<CToolItemModel::ItemListId>* getAutho() { return &m_authorization;}
  void resetSceneMode() { m_mode = MODE_IDLE; }

  CGraphicsItem* searchItemAndAssociateAt(const QPointF& point, CGraphicsItem::LinkOrder type); //TODO put in private?
  CGraphicsItem* searchItem(const QPointF& point);
  void removeGraphicalItem(QGraphicsItem* item);
  void checkItemLocation ( QGraphicsItem* item);

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
  virtual void keyPressEvent(QKeyEvent* event);

  void setMouseTrackingInAllViews(void);
  void setModeForNextOperation();
    
public slots:
  void onChangeItem(int id, CToolButton::STATE state);

signals:
  void itemInserted();    
  
protected:
  QVector<CToolItemModel::ItemListId> m_authorization;
  CItem*  m_item;
  QString m_title;
  int m_type;
  bool m_isDisplayed;
  
  typedef enum
  {
    MODE_IDLE,
    MODE_CREATE_ITEM,
    MODE_IN_ITEM_CREATION, 
    MODE_IN_ITEM_EVOLUTION, //when an anchor is pressed
  } SceneMode;

  SceneMode m_mode;
  int m_idToCreate;
  CToolButton::STATE m_keepMode;
  CGraphicsItem* m_currentItem;
  CGraphicsAnchor* m_pressedAnchor;
  
  CGraphicsRectSelection* m_itemRect;
  QPointF m_posWhenSelect;

private:
  void createItem(QGraphicsSceneMouseEvent* event);
  void updateItemDuringCreation(QGraphicsSceneMouseEvent* event);

  CGraphicsAnchor* getAnchorAt(const QPointF& point);
  void notifyClickToItem(const QPointF& point);
};

#endif // GRAPHICSSCENE_H
