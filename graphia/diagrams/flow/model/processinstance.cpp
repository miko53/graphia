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
#include "processinstance.h"
#include "flow.h"
#include "store.h"
#include "processimplementation.h"
#include "processcontext.h"

CProcessInstance::CProcessInstance(CProcessImplementation* parent) : CItem(parent)
{
  m_implementation = NULL;
  m_nameChanged = false,
  qDebug("creation of CProcessInstance '%p'", this);
}

CProcessInstance::~CProcessInstance()
{
  qDebug("deletion of CProcessInstance '%p'", this);
}

QString CProcessInstance::getName()
{
  if (m_implementation != NULL)
  {
    return m_implementation->getName();
  }
  else
  {
    if (m_nameChanged == false)
    {
      return QString("Process_%1").arg(m_id);
    }
    else
      return CItem::getName();
  }
}

void CProcessInstance::setName ( const QString& newName )
{
  CItem::setName ( newName );
  m_nameChanged = true;
}


bool CProcessInstance::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
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


void CProcessInstance::addInput ( CFlow* input, CInput* in )
{
  addInput(input);
}


void CProcessInstance::addOutput ( CFlow* output, COutput* out )
{
  addOutput(output);
}


void CProcessInstance::addInput(CFlow* input)
{
  CConnector* c = new CConnector();
  
  c->m_flow = input;
  c->m_input = NULL;
  
  m_inputs.append(c);
}

void CProcessInstance::addOutput(CFlow* output)
{
  CConnector* c = new CConnector();
  
  c->m_flow = output;
  c->m_input = NULL;
  
  m_outputs.append(c);
}

bool CProcessInstance::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  qDebug("CProcessInstance::dissociate");
//  CProcessInstance* process;
  Q_ASSERT(item->type() == CFlow::CFLOW);
  CFlow* flow = dynamic_cast<CFlow*> (item);
  bool bOK = true;
  
  //TODO eviter de retirer si on est dans le meme process (dans le graphique en cours de modif)
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
      bOK = false;
      break;
  }
  
  return bOK;
}


void CProcessInstance::removeInput(CFlow* in)
{
  bool found = false;
  bool bOk;
  CConnector* c;
  //search the connector
  foreach(c, m_inputs)
  {
    if (c->m_flow == in)
    {
      found = true;
      break;
    }
  }
  
  if (found)
  {
    bOk = m_inputs.removeOne(c);
    Q_ASSERT(bOk == true);
    Q_ASSERT(c->m_flow == in);
    delete c;
  }
  
  Q_ASSERT(found == true);
}

void CProcessInstance::removeOutput(CFlow* out)
{
  bool found = false;
  bool bOk;
  CConnector* c;
  //search the connector
  foreach(c, m_outputs)
  {
    if (c->m_flow == out)
    {
      found = true;
      break;
    }
  }
  
  if (found)
  {
    bOk = m_outputs.removeOne(c);
    Q_ASSERT(bOk == true);
    Q_ASSERT(c->m_flow == out);
    delete c;
  }
  
  Q_ASSERT(found == true);
}

void CProcessInstance::removeAllInputs(void)
{
  foreach(CConnector* c, m_inputs)
  {
    //set all connected flow to NULL
    Q_ASSERT(c->m_flow->getOutput() == this);
    c->m_flow->setOutput(NULL);
    delete c;
  }
}

void CProcessInstance::removeAllOutputs(void)
{
  foreach(CConnector* c, m_outputs)
  {
    //set all connected flow to NULL
    Q_ASSERT(c->m_flow->getInput() == this);
    c->m_flow->setInput(NULL);
    delete c;
  }
}


void CProcessInstance::setImplementedProcess(CProcessImplementation* p)
{
  m_implementation = p;
  if (p != NULL)
  {
    p->addInstance(this);
  }
}

