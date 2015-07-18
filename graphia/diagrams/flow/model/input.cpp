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
#include "input.h"
#include "flow.h"

CInput::CInput(CItemContainer* parent): CItem(parent)
{
  m_flow = NULL;
  m_dataFlow = NULL;
  m_name = QString("input_%1").arg(m_id);
  qDebug("creation of CInput '%p'", this);  
}

CInput::~CInput()
{
  qDebug("deletion of CInput '%p'", this);
}

QString CInput::getName()
{
  return QString("input_%1").arg(m_id);
}

void CInput::setName ( const QString& newName )
{
  Q_UNUSED(newName);
  //name if not changeable
  return;
}

bool CInput::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_ASSERT(item->type() == CFlow::CFLOW);
  bool bAssociationOK = false;
  CFlow* flow = dynamic_cast<CFlow*>(item);
  
  
  if ((order == CGraphicsItem::BEGIN) && 
      (m_flow == NULL))        //TODO aujourd'hui ne gere q'un flow, ajouter une liste pour gerer la connection de plusieurs flows a un connecteur
  {
    m_flow = flow;
    flow->setInput(this);
    bAssociationOK = true;
  }
  
  return bAssociationOK;  
}

bool CInput::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  bool bOK = false;
  Q_ASSERT(item->type() == CFlow::CFLOW);
  CFlow* flow = dynamic_cast<CFlow*> (item);
 
  if (order == CGraphicsItem::BEGIN)
  {
    m_flow = NULL;
    flow->setInput(NULL);
    bOK = true;
  }
  else
  {
    bOK = false;
  }
  
  
  return bOK;
}


