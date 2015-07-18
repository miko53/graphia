/*
    <one line to give the program's name and a brief idea of what it does.>
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

#include "store.h"
#include "flow.h"

CStore::CStore(CItemContainer* parent) : CItem(parent)
{
  m_nameChanged = false;
  qDebug("creation of CStore '%p'", this);
}

CStore::~CStore()
{
  qDebug("deletion of CStore '%p'", this);
}

QString CStore::getName()
{
  if (m_nameChanged == false)
    return QString("store_%1").arg(m_id);
  else
    return CItem::getName();
}

void CStore::setName ( const QString& newName )
{
  m_nameChanged = true;
  CItem::setName ( newName );
}


bool CStore::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_ASSERT(item->type() == CFlow::CFLOW);
  bool bAssociationOK = false;
  CFlow* flow = dynamic_cast<CFlow*>(item);
  CItem* itemAtOutputFlow;
  CItem* itemAtInputFlow;
  
  switch (order)
  {
    case CGraphicsItem::BEGIN:
      itemAtOutputFlow = flow->getOutput();
      if (itemAtOutputFlow == this)
        bAssociationOK = false;
      else
      {
        //add the flow as output
        addOutput(flow);
        flow->setInput(this);
        bAssociationOK = true;
      }
    break;
    
    case CGraphicsItem::END:
      itemAtInputFlow = flow->getInput();
      if (itemAtInputFlow == this)
        bAssociationOK = false;
      else
      {
        //add the flow as input
        addInput(flow);
        flow->setOutput(this);
        bAssociationOK = true;
      }
    break;
    
    default:
    Q_ASSERT(false);  
    break;
  }
  
  return bAssociationOK;
}

void CStore::addInput(CFlow* input)
{ 
  m_inputs.append(input);
}

void CStore::addOutput(CFlow* output)
{
  m_outputs.append(output);
}


bool CStore::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  qDebug("CStore::dissociate");
  
  Q_ASSERT(item->type() == CFlow::CFLOW);
  CFlow* flow = dynamic_cast<CFlow*> (item);
  bool bOK = true;

  switch (order)
  {
    case CGraphicsItem::BEGIN:
      removeOutput(flow);
      flow->setInput(NULL);
      break;

    case CGraphicsItem::END:
      removeInput(flow);
      flow->setOutput(NULL);
      break;

    default:
      Q_ASSERT(false);
      return false;
      break;
  }

  return bOK;
}

void CStore::removeInput(CFlow* in)
{
  bool bOk;
  bOk = m_inputs.removeOne(in);
  Q_ASSERT(bOk == true);
}

void CStore::removeOutput(CFlow* out)
{
  bool bOk;
  bOk = m_outputs.removeOne(out);
  Q_ASSERT(bOk == true);
}


void CStore::removeAllInputs(void)
{
  foreach(CFlow* f, m_inputs)
  {
    //set all connected flow to NULL
    Q_ASSERT(f->getOutput() == this);
    f->setOutput(NULL);
  }
}

void CStore::removeAllOutputs(void)
{
  foreach(CFlow* f, m_outputs)
  {
    //set all connected flow to NULL
    Q_ASSERT(f->getInput() == this);
    f->setInput(NULL);
  }
}
