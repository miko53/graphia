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

#ifndef ITEMCONTAINER_H
#define ITEMCONTAINER_H

#include "item.h"


class CItemContainer : public CItem
{
public:
  CItemContainer(CItemContainer* parent = 0);
  virtual ~CItemContainer();
  
  virtual CItem* createItem(int type) = 0;
  virtual void removeItem(CItem* item) = 0;
  virtual bool renameObject ( CItem* m_item, const QString& name ) = 0;
  
  virtual bool insertItem(CItem* item) = 0;
  virtual CFlow* searchFlow(int ID) = 0;
  virtual CItem* searchItem(int type, int ID) = 0;
  
protected:

private:

  
};

#endif // ITEMCONTAINER_H
