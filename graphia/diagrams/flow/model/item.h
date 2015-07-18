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
#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include "diagrams/generic/graphicsitem.h"

class CItemContainer;

class CItem
{
public:
  virtual ~CItem();

  virtual QString getName();
  virtual void setName(const QString& newName);
  
  virtual int type() = 0;
  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order) = 0;
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order) = 0;

  CItemContainer* getParent() { return m_parent; }
  void setParent(CItemContainer* parent) { m_parent = parent; }
  void setID(int id) { m_id = id; }
  int getID(void) { return m_id; }
  
protected:
  CItem(CItemContainer* parent = NULL);
  
  CItemContainer* m_parent;
  int m_id;
  QString m_name;

private:

};


#endif /* ITEM_H */

