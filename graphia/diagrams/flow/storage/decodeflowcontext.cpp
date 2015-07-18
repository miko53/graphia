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
#include "decodeflowcontext.h"


CDecodeFlowContext::CDecodeFlowContext ( CItemFactory* factory )
{
  m_pItemFactory = factory;
}

bool CDecodeFlowContext::decode ( const QDomElement& item )
{
  CProcessContext* p = NULL;
  p = readCProcessContext(item);
  bool bCorrectlyDecoded;
  
  if (p != NULL)
  {
    m_pItemFactory->getContexProcessFactory()->insertContextProcess(p, p->getID());
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded; 
}


CProcessContext* CDecodeFlowContext::readCProcessContext ( const QDomElement& element )
{
  CProcessContext* process = NULL;
  bool bCorrectlyDecoded = false;
  QDomElement id = element.firstChildElement("id");
  QDomElement name = element.firstChildElement("name");
  QDomElement hasChanged = element.firstChildElement("hasChanged");
  
  if (!id.isNull() && !name.isNull() && !hasChanged.isNull())
  {
    process = new CProcessContext();
    process->setID(id.text().toInt());
    process->setName(name.text());
    process->setNameHasChanged(hasChanged.text().toInt());
    bCorrectlyDecoded = true;
  }

  //   "flow-list"
  QDomElement flowList = element.firstChildElement("flow-list");
  if (!flowList.isNull())
  {
    CDecodeFlowList decodeFlowList(process, m_pItemFactory->getDataFactory(), CDecodeFlowList::FIRST_PASS);
    bCorrectlyDecoded = decodeFlowList.searchInXmlList(flowList, "flow", &decodeFlowList);
  }
  else
  {
    delete process;
    process = NULL;
    bCorrectlyDecoded = false;
  }
  

  //store list
  if (bCorrectlyDecoded == true)
  {
    QDomElement storeList = element.firstChildElement("store-list");
    if (!storeList.isNull())
    {
      CDecodeStoreList decodeStoreList(process);
      bCorrectlyDecoded = decodeStoreList.searchInXmlList(storeList, "store", &decodeStoreList);
    }
    else
    {
      delete process;
      process = NULL;
      bCorrectlyDecoded = false;
    }
  } 
  
  if (bCorrectlyDecoded == true)
  {
    QDomElement terminatorList = element.firstChildElement("terminator-list");
    if (!terminatorList.isNull())
    {
      CDecodeTerminatorList decodeTerminatorList(process, m_pItemFactory->getDataFactory());
      bCorrectlyDecoded = decodeTerminatorList.searchInXmlList(terminatorList, "terminator", &decodeTerminatorList);
    }
    else
    {
      delete process;
      process = NULL;
      bCorrectlyDecoded = false;
    }
  }
  
  if (bCorrectlyDecoded)
  {
    QDomElement mainProcess = element.firstChildElement("process");
    CProcessInstance* uniqueInstance = NULL;
    uniqueInstance = readInstanceOfContextProcess(mainProcess, process);
    if (uniqueInstance != NULL)
    {
      process->setProcessInstance(uniqueInstance);
    }
    else
      bCorrectlyDecoded = false;
  }
  
  //flow 2nd pass
  if (bCorrectlyDecoded == true)
  {
    CDecodeFlowList decodeFlowList(process, m_pItemFactory->getDataFactory(), CDecodeFlowList::SECOND_PASS);
    bCorrectlyDecoded = decodeFlowList.searchInXmlList(flowList, "flow", &decodeFlowList);
    if (!bCorrectlyDecoded)
    {
      delete process;
      process = NULL;
      bCorrectlyDecoded = false;
    }
    
  } 
  
  
  if ((bCorrectlyDecoded == false) && (process != NULL))
  {
    delete process;
    process = NULL;
  }
  
  return process;
}

CProcessInstance* CDecodeFlowContext::readInstanceOfContextProcess ( const QDomElement& data, CProcessContext* processContext )
{
  CProcessInstance* processInstance = NULL; 
  bool bOk = true;
  QDomElement id = data.firstChildElement("id");
  QDomElement name = data.firstChildElement("name");
  QDomElement hasChanged = data.firstChildElement("hasChanged");
  QDomElement implements = data.firstChildElement("implements");
  int implementsID;
  CProcessImplementation* implementsProcess = NULL;  
  
  if (!id.isNull() && !name.isNull() && !hasChanged.isNull() && !implements.isNull())
  {
    processInstance = new CProcessInstance();
    processInstance->setID(id.text().toInt());
    processInstance->setName(name.text());
    processInstance->setNameHasChanged(hasChanged.text().toInt());
 
    implementsID = implements.attribute("id").toInt();
    implementsProcess = m_pItemFactory->getProcessFactory()->getImplementationProcess(implementsID);
    if (implementsProcess != NULL) //shall not be null because exist always for a context diagram ToCheck
      processInstance->setImplementedProcess(implementsProcess);
    else
    {
      delete processInstance;
      processInstance = NULL;
    }

    //read input flow list
    QDomElement inputList = data.firstChildElement("input-list");
    CDecodeInputOutputListOfProcessInstance decodeInput(processContext, processInstance, true);
    
    bOk = decodeInput.searchInXmlList(inputList, "input", &decodeInput);
    if (bOk)
    {
      //read output flow list
      QDomElement outputList = data.firstChildElement("output-list");
      CDecodeInputOutputListOfProcessInstance decodeOutput(processContext, processInstance, false);
      bOk = decodeOutput.searchInXmlList(outputList, "output", &decodeOutput);
    }
    
    if (!bOk)
    {
      delete processInstance;
      processInstance = NULL;
    }
    
  }
  return processInstance;
}




CDecodeTerminatorList::CDecodeTerminatorList ( CProcessContext* p, CDataFactory* dataFactory )
{
  m_ctxProcess = p;
  m_dataFactory = dataFactory;
}


bool CDecodeTerminatorList::decode ( const QDomElement& item )
{
  CTerminator* t;
  bool bCorrectlyDecoded;
  
  t = readTerminator(item);
  if (t != NULL)
  {
    m_ctxProcess->insertItem(t);
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded;  
}

CTerminator* CDecodeTerminatorList::readTerminator ( QDomElement terminator )
{
  CTerminator* t = NULL;
  
  QDomElement id = terminator.firstChildElement("id");
  QDomElement name = terminator.firstChildElement("name");
  QDomElement hasChanged = terminator.firstChildElement("hasChanged");
  QDomElement type = terminator.firstChildElement("type");

  if (!id.isNull() && !name.isNull() && !hasChanged.isNull() && !type.isNull())
  {
    bool bCorrectlyDecoded;
    int Id;
    Id = id.text().toInt(&bCorrectlyDecoded);
    
    if (bCorrectlyDecoded)
    {
      t = new CTerminator();
      t->setName(name.text());
      t->setID(Id);
      t->setNameHasChanged(hasChanged.text().toInt());
      t->setType(type.text().toInt());
      
      QDomElement flowID = terminator.firstChildElement("flow");
      if (!flowID.isNull())
      {
        QString sIdOfFlow;
        int idOfFlow;
        flowID.attribute("id", sIdOfFlow);
        idOfFlow = sIdOfFlow.toInt();
        CFlow* f = m_ctxProcess->searchFlow(idOfFlow);
        if (f != NULL)
        {
          t->setInOrOutFlow(f);
        }
        else
        {
          //not possible 
          qDebug("flow not found");
          delete t;
          t = NULL;
        }
      }
      
      QDomElement dataID = terminator.firstChildElement("data-id");
      if ((t != NULL) && (!dataID.isNull()))
      {
        int idOfData = dataID.text().toInt();
        CData* theData;
        theData = m_dataFactory->search(idOfData);
        if (theData != NULL)
        {
          t->setDataFlow(theData);
        }
        else
        {
          qDebug("data not found");
          delete t;
          t = NULL;
        }
      }
    }
  }
  
  return t;
}


