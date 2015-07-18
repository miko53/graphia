
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
#include "flow.h"

CFlow::CFlow(CItemContainer* parent) : CItem(parent)
{
  m_input = NULL;
  m_output = NULL;
  m_dataFlow = NULL;
  
  qDebug("creation of CFlow(Arrow) '%p'", this);
}

CFlow::~CFlow()
{
  qDebug("deletion of CFlow '%p'", this);
}

QString CFlow::getName()
{
  return QString("arrow_%1").arg(m_id);
}

void CFlow::setName ( const QString& newName )
{
  Q_UNUSED(newName);
  //name is not changeable
  return;
}


bool CFlow::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);Q_UNUSED(order);
  Q_ASSERT(false);
  return false;
}

bool CFlow::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);Q_UNUSED(order);
  qDebug("CFlow::dissociate");
  return false;
}
