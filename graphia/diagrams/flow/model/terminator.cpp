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

#include "terminator.h"
#include "processimplementation.h"
#include "flow.h"

CTerminator::CTerminator(CItemContainer* parent): CItem(parent)
{
  m_nameChanged = false;
  m_inOrOut = NULL;
  m_type = NONE;
  m_dataFlow = NULL;
  qDebug("creation of CTerminator '%p'", this);  
}

CTerminator::~CTerminator()
{
  qDebug("deletion of CTerminator '%p'", this);
}


QString CTerminator::getName()
{
  if (m_nameChanged == false)
    return QString("terminator_%1").arg(m_id);
  else
    return CItem::getName();
}

void CTerminator::setName ( const QString& newName )
{
  CItem::setName ( newName );
  m_nameChanged = true;
}


bool CTerminator::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_ASSERT(item->type() == CFlow::CFLOW);
  bool bAssociationOK = false;
  CFlow* flow = dynamic_cast<CFlow*>(item);
  
  if (m_type == NONE)
  {
    bAssociationOK = true;
    switch (order)
    {
      case CGraphicsItem::BEGIN:
        m_inOrOut = flow;
        m_type = OUTPUT;
        flow->setInput(this);
      break;
      
      case CGraphicsItem::END:
        m_inOrOut = flow;
        m_type = INPUT;
        flow->setOutput(this);
      break;
      
      default:
      Q_ASSERT(false);  
      break;
    }
  }
  else 
    bAssociationOK = false;
    
  return bAssociationOK;
}

bool CTerminator::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_ASSERT(item->type() == CFlow::CFLOW);
  bool bOK = true;
  
  if (m_type != NONE)
  {
    switch (order)
    {
      case CGraphicsItem::BEGIN:
        m_inOrOut->setInput(NULL);
        break;
        
      case CGraphicsItem::END:
        m_inOrOut->setOutput(NULL);
        break;
        
      default:
        Q_ASSERT(false);
        return false;
        break;
    }
    m_type = NONE;
    m_inOrOut = NULL;
  }
  return bOK;
}


void CTerminator::removeAttachedFlow(void)
{
  if (m_inOrOut != NULL)
  {
    switch (m_type)
    {
      case INPUT:
        Q_ASSERT(m_inOrOut->getOutput() == this);
        m_inOrOut->setOutput(NULL);
        m_inOrOut = NULL;
        break;
        
      case OUTPUT:
        Q_ASSERT(m_inOrOut->getInput() == this);
        m_inOrOut->setInput(NULL);    
        m_inOrOut = NULL;
        break;
        
      case NONE:
      default:
        Q_ASSERT(false);
        break;
    }
  }
}
