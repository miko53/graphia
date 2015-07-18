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
#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include "toolitemmodel.h"
#include "property/propertydefinition.h"

class CGraphicsAnchor;
class QDomDocument;
class QDomElement;
class CItem;

class CGraphicsItem : public QObject
{
  Q_OBJECT

public:
  CGraphicsItem(CItem* associatedObj);
  virtual ~CGraphicsItem() {}

  enum { GraphicsItem = QGraphicsItem::UserType + 1 };
  
  typedef enum
  {
    CAN_BE_LINKED = 0x01,
    NEED_MORE_STEP = 0x02,
    IS_LINK_ITEM = 0x04,
    IS_REMOVABLE = 0x08
  } itemProperties;
  
  typedef enum
  {
    BEGIN = 1,
    END = 2,
  } LinkOrder;

  CItem* getAssociatedItem() { return m_item;}
  void setAssociatedItem(CItem* obj) { m_item = obj; }
  
  int getID(); 
  //Normally not necessary ?void setID(int id) { m_id = id; }
  
  virtual void setProperties(int p) { m_itemProperties |= p; }
  virtual void resetProperties(int p) { m_itemProperties &= ~p; }
  
  /**
   * This function returns the properties of the graphic item,
   *  these properties are used by graphicsscene to adapt its behavior to the item
   *  i.e. if item can be linked to another or if it needs more steps to be inserted.
  **/
  virtual int getProperties() { return m_itemProperties; }

  /**
   * This fonction is called by graphics scene when a association is possible
   * when a true is returned the association is possible and the scene terminates the insertion
   * when a false is returned the association is failed or not possible and the scene continues the insertion
   **/
  virtual bool associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order) = 0;

  /**
   * This function is used by the graphic scene to indicate that the user modify the object being inserted
  **/
  virtual void updateCurrentPoint(const QPointF& point) = 0;

  /**
   * This function is used by graphic scene to indicate that the user click on left button during inserting
   * of a item that need more than one step to be inserted
  **/
  virtual bool onLeftClickEvent(const QPointF& point) = 0;

  /**
   * This function is called by a graphical item to link the graphical object to an another one
  **/
  virtual void linkTo(CGraphicsItem* item) = 0;

  /**
   * This function returns the intersection point calculated according to ref point given
   **/
  virtual bool getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint)  = 0;

  /**
   * This fonctions indicates that linked item has changed of position
   **/
  virtual void onPositionHasChanged(void)  = 0;
  
  /** 
   * TODO
   **/
  virtual void onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor) = 0;


  /**
   * This function is called to unlink the graphical item of another (especially when the graphical item is removed from the scene
   **/
  virtual void unlink(void) = 0;

  /**
   * This function requests to remove the link between the objet and the graphical item given in parameter
   **/
  virtual void removeLinkTo(CGraphicsItem* item) = 0;

  /**
   * This function returns the list of properties which can be browser into the property browser widget
   **/
  virtual QList<CPropertyDefinition::Type> getListOfGraphicalProperties(void) = 0;
  virtual QVariant getGraphicalProperties(CPropertyDefinition::Type type) = 0;
  virtual void setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value) = 0;

  /**
   * TODO
   **/
  virtual void insertItemIntoDomDocument(QDomDocument& doc, QDomElement& element) = 0;

  /**
   * TODO
   **/
  virtual void setAndUpdateGraphicalData(void) = 0;
  /**
   * This signal informs that a propertie of the item has changed
   **/
  signals:
  void itemChanged(CGraphicsItem *item, CPropertyDefinition::Type param, QVariant value);

protected:
  CItem* m_item;
  int m_itemProperties;
  int m_id;
  
private:
  
};

#endif // GRAPHICSITEM_H
