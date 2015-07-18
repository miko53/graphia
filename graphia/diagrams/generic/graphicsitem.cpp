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
#include "diagrams/generic/graphicsitem.h"
#include "diagrams/flow/model/item.h"

CGraphicsItem::CGraphicsItem( CItem* associatedObj) :  QObject()
{
  m_item = associatedObj;
}

int CGraphicsItem::getID()
{
  if (m_item != NULL)
    return m_item->getID();
  else
    return -1;
}

