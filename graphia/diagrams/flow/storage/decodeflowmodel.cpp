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

#include "decodeflowmodel.h"
#include <qtextstream.h>

CDecodeFlowModel::CDecodeFlowModel(CItemFactory* pItemFactory)
{
  m_pItemFactory = pItemFactory;
}


bool CDecodeFlowModel::decode(const QDomElement& item)
{
  CProcessImplementation* p = readImplementationProcessFirstPass(item);
  bool bCorrectlyDecoded;
  
  if (p != NULL)
  {
    m_pItemFactory->getProcessFactory()->insertImplementationProcess(p, p->getID());
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded; 
}

CProcessImplementation* CDecodeFlowModel::readImplementationProcessFirstPass(const QDomElement& element)
{
  CProcessImplementation* process = NULL;  
  QDomElement id = element.firstChildElement("id");
  QDomElement name = element.firstChildElement("name");
  bool bOk = false;

  //faire en plusieurs passe,  premiere pour prendre les noms ensuite pour faire les liens entre les objets
  
  if (!id.isNull() && !name.isNull())
  {
    process = new CProcessImplementation();
    qDebug("name = %s\n", name.text().toLocal8Bit().data());
    process->setName(name.text());
    process->setID(id.text().toInt(&bOk));
  }
  
  if ((process != NULL) && (bOk == false))
  {
    delete process;
    process = NULL;
    return process;
  }

  //flow list
  QDomElement flowList = element.firstChildElement("flow-list");
  if (!flowList.isNull())
  {
    CDecodeFlowList decodeFlowList(process, m_pItemFactory->getDataFactory(), CDecodeFlowList::FIRST_PASS);
    bOk = decodeFlowList.searchInXmlList(flowList, "flow", &decodeFlowList);
  }
  else
  {
    delete process;
    process = NULL;
    bOk = false;
    return process;
  }
  
  if (bOk == true)
  {    
    QDomElement processInstanceList = element.firstChildElement("process-instance-list");
    if (!processInstanceList.isNull())
    {
      CDecodeProcessInstanceList decodeProcessInstanceList(process);
      bOk = decodeProcessInstanceList.searchInXmlList(processInstanceList, "process", &decodeProcessInstanceList);
    }
    else
    {
      delete process;
      process = NULL;
      bOk = false;
      return process;
    }
  }

  //store list
  if (bOk == true)
  {
    QDomElement storeList = element.firstChildElement("store-list");
    if (!storeList.isNull())
    {
      CDecodeStoreList decodeStoreList(process);
      bOk = decodeStoreList.searchInXmlList(storeList, "store", &decodeStoreList);
    }
    else
    {
      delete process;
      process = NULL;
      bOk = false;
      return process;
    }
  }

  //qDebug("read input list (previous status = %d)", bOk);
  //input list
  if (bOk == true)
  {
    QDomElement inputList = element.firstChildElement("input-list");
    if (!inputList.isNull())
    {
      CDecodeInputOutputList decodeInputList(process, m_pItemFactory, true);
      bOk = decodeInputList.searchInXmlList(inputList, "input", &decodeInputList);
    }
    else
    {
      delete process;
      process = NULL;
      bOk = false;
      return process;
    }
  }

  //qDebug("read output list (previous status = %d)", bOk);
  //output list
  if (bOk == true)
  {
    QDomElement outputList = element.firstChildElement("output-list");
    if (!outputList.isNull())
    {
      CDecodeInputOutputList decodeOutputList(process, m_pItemFactory, false);
      bOk = decodeOutputList.searchInXmlList(outputList, "output", &decodeOutputList);
    }
    else
    {
      delete process;
      process = NULL;
      bOk = false;
      return process;
    }
  }
  
  //qDebug("read 2nd pass (previous status = %d)", bOk);
  //now do the second pass for terminate the creation of the process.
  if (bOk == true)
  {
    CDecodeFlowList decodeFlowList(process, m_pItemFactory->getDataFactory(), CDecodeFlowList::SECOND_PASS);
    bOk = decodeFlowList.searchInXmlList(flowList, "flow", &decodeFlowList);
    if (!bOk)
    {
      delete process;
      process = NULL;
      return process;
    }
  }

  return process;
}


CDecodeFlowList::CDecodeFlowList( CItemContainer* pImplProcess, CDataFactory* dataFactory, CDecodeFlowList::passType passDecoding )
{
  m_implProcess = pImplProcess;
  m_dataFactory = dataFactory;
  m_passType = passDecoding;
}

bool CDecodeFlowList::decode(const QDomElement& item)
{
  CFlow* f;
  bool bCorrectlyDecoded;
  
  if (m_passType == FIRST_PASS)
  {
    f = readFlowFirstPass(item);
    if (f != NULL)
    {
      m_implProcess->insertItem(f);
      bCorrectlyDecoded = true;
    }
    else
      bCorrectlyDecoded = false;
  }
  else
  {
    bCorrectlyDecoded = readFlowSecondPass(item);
  }
  
  return bCorrectlyDecoded; 
}



CFlow* CDecodeFlowList::readFlowFirstPass(const QDomElement& data)
{
  CFlow* f = NULL;
  bool bOk = true;
  QDomElement id = data.firstChildElement("id");
  QDomElement name = data.firstChildElement("name");
  
  //qDebug("id = %s, name = %s", id.text().toLocal8Bit().data(), name.text().toLocal8Bit().data());
  if (!id.isNull() &&  !name.isNull())
  {
    //qDebug("flow is ok");
    f = new CFlow();
    f->setID(id.text().toInt(&bOk));
    f->setName(name.text());
    if (!bOk)
    {
      delete f;
      f = NULL;
    }
      
    /* another parameter will be updated in a 2nd pass */
  }

  return f;
}



bool CDecodeFlowList::readFlowSecondPass ( const QDomElement& data )
{
  CFlow* f = NULL;
  bool bOk = true;
  QDomElement id = data.firstChildElement("id");
  QDomElement input = data.firstChildElement("input");
  QDomElement output = data.firstChildElement("output");
  QDomElement dataId = data.firstChildElement("data-id");

  int Id;
  int type;
  int linkedId;
  
  Q_ASSERT(!id.isNull()); //it is not possible because it is the 2nd pass
  
  Id = id.text().toInt();
  
  //searcher le flow
  //recuperer les infos d'input/output
  //faire les liens avec le flow
  f = m_implProcess->searchFlow(Id);
  Q_ASSERT(f != NULL); // flow exists in the 2nd pass.
  
  
  if (!input.isNull())
  {
     type = input.attribute("type").toInt(&bOk);
     if (bOk == true)
     {
       linkedId = input.attribute("id").toInt(&bOk);
     }
     if (bOk == true)
     {
       CItem* item;
       //qDebug("type = %d linkedId = %d\n", type, linkedId);
       item = m_implProcess->searchItem(type, linkedId);
       Q_ASSERT(item != NULL); //not found the item is not possible in the 2nd pass.
       f->setInput(item);
     }
  }
  
  if (!output.isNull() && (bOk == true))
  {
     type = output.attribute("type").toInt(&bOk);
     if (bOk == true)
     {
       linkedId = output.attribute("id").toInt(&bOk);
     }
     if (bOk == true)
     {
       CItem* item;
       item = m_implProcess->searchItem(type, linkedId);
       Q_ASSERT(item != NULL); //not found the item is not possible in the 2nd pass.
       f->setOutput(item);
     }
  }
  
  
  if (!dataId.isNull() && (bOk == true))
  {
    linkedId = dataId.text().toInt(&bOk);
    if (bOk)
    {
      CData* data;
      data = m_dataFactory->search(linkedId);
      if (data != NULL)
      {
        f->setDataFlow(data);
      }
      else
      {
        qDebug("Data flow not found !");
        bOk = false;
      }
    }
  }  
  
  return bOk;
}


CDecodeProcessInstanceList::CDecodeProcessInstanceList(CProcessImplementation* pImplProcess)
{
  m_implProcess = pImplProcess;
}


bool CDecodeProcessInstanceList::decode(const QDomElement& item)
{
  CProcessInstance* p;
  bool bCorrectlyDecoded;
  
  p = readProcessInstance(item);
  if (p != NULL)
  {
    m_implProcess->insertItem(p);
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded; 
}


CProcessInstance* CDecodeProcessInstanceList::readProcessInstance(const QDomElement& data)
{
  CProcessInstance* processInstance = NULL; 
  bool bOk = true;
  QDomElement id = data.firstChildElement("id");
  QDomElement name = data.firstChildElement("name");
  QDomElement hasChanged = data.firstChildElement("hasChanged");
  
  
  if (!id.isNull() && !name.isNull() && !hasChanged.isNull())
  {
    processInstance = new CProcessInstance();
    
    processInstance->setID(id.text().toInt());
    processInstance->setName(name.text());
    processInstance->setNameHasChanged(hasChanged.text().toInt());
    
    
    //read now input flow list
    QDomElement inputList = data.firstChildElement("input-list");
    CDecodeInputOutputListOfProcessInstance decodeInput(m_implProcess, processInstance, true);
    
    bOk = decodeInput.searchInXmlList(inputList, "input", &decodeInput);
    
    if (bOk)
    {
      //read now output flow list
      QDomElement outputList = data.firstChildElement("output-list");
      CDecodeInputOutputListOfProcessInstance decodeOutput(m_implProcess, processInstance, false);
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


CDecodeInputOutputListOfProcessInstance::CDecodeInputOutputListOfProcessInstance( CItemContainer* pImplProcess, CProcessInstance* process, bool isInput )
{
  m_implProcess = pImplProcess;
  m_processInstance = process;
  m_isInput = isInput;
}


bool CDecodeInputOutputListOfProcessInstance::decode(const QDomElement& item)
{
  CFlow* f;
  bool bCorrectlyDecoded;
 
  int flowId;
  flowId = item.attribute("flow-id").toInt(&bCorrectlyDecoded); 
  if (bCorrectlyDecoded == true)
  {
    f = m_implProcess->searchFlow(flowId);
    if (f != NULL)
    {
      if (m_isInput)
      {
        m_processInstance->addInput(f, NULL);//TODO to add the CINput here
      }
      else
      {
        m_processInstance->addOutput(f, NULL);//TODO to add the COUTput here et remove le NULL qui ne peut pas être ajouté à ce stade.
      }
    }
    else
      bCorrectlyDecoded = false;
  }

  return bCorrectlyDecoded; 
}



CDecodeStoreList::CDecodeStoreList(CItemContainer* pImplProcess)
{
  m_implProcess = pImplProcess;
}

bool CDecodeStoreList::decode(const QDomElement& item)
{
  CStore* s;
  bool bCorrectlyDecoded;
  
  s = readStore(item);
  if (s != NULL)
  {
    m_implProcess->insertItem(s);
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded;  
}


CStore* CDecodeStoreList::readStore(const QDomElement& store)
{
  bool bOk = true;
  CStore* s = NULL;
  
  QDomElement id = store.firstChildElement("id");
  QDomElement name = store.firstChildElement("name");
  QDomElement hasChanged = store.firstChildElement("hasChanged");
  
  if (!id.isNull() && !name.isNull() && !hasChanged.isNull())
  {
    s = new CStore();
    //qDebug("store ID = %d", id.text().toInt(&bOk));
    if (bOk)
    {
      s->setID(id.text().toInt(&bOk));
      s->setName(name.text());
      s->setNameHasChanged(hasChanged.text().toInt(&bOk));
      
      CDecodeInputOutputListOfStore decodeInputOfStore(m_implProcess, s, true);
      QDomElement inputList = store.firstChildElement("input-list");
      bOk = decodeInputOfStore.searchInXmlList(inputList, "input", &decodeInputOfStore);
      
      if (bOk)
      {
        CDecodeInputOutputListOfStore decodeOutputOfStore(m_implProcess, s, false);
        QDomElement outputList = store.firstChildElement("output-list");
        bOk = decodeOutputOfStore.searchInXmlList(inputList, "output", &decodeInputOfStore);
      }
      
      if (!bOk)
      {
        qDebug("delete Store 1");
        delete s;
        s = NULL;
      }
    }
    else
    {qDebug("delete Store 2");
      delete s;
      s = NULL;
    }

  }

  return s;
}

CDecodeInputOutputListOfStore::CDecodeInputOutputListOfStore( CItemContainer* pImplProcess, CStore* pStore, bool isInput )
{
  m_implProcess = pImplProcess;
  m_store = pStore;
  m_isInput = isInput;
}

bool CDecodeInputOutputListOfStore::decode(const QDomElement& item)
{
  CFlow* f;
  bool bCorrectlyDecoded;
 
  int flowId;
  flowId = item.attribute("flow-id").toInt(&bCorrectlyDecoded);
  
  if (bCorrectlyDecoded == true)
  {
    f = m_implProcess->searchFlow(flowId);
    if (f != NULL)
    {
      if (m_isInput)
      {
        m_store->addInput(f);
      }
      else
      {
        m_store->addOutput(f);
      }
    }
    else
      bCorrectlyDecoded = false;
  }

  return bCorrectlyDecoded; 
}



CDecodeInputOutputList::CDecodeInputOutputList(CProcessImplementation* impl, CItemFactory* factory, bool isInput)
{
  m_implProcess = impl;
  m_isInput = isInput;
  m_itemFactory = factory;
}


bool CDecodeInputOutputList::decode(const QDomElement& item)
{
  CItem* pItem = NULL;
  bool bCorrectlyDecoded = false;
 
  pItem = readItemData(item);
  if (pItem != NULL)
  {
    m_implProcess->insertItem(pItem);
    bCorrectlyDecoded = true;
  }

  return bCorrectlyDecoded; 
}


CItem* CDecodeInputOutputList::readItemData(const QDomElement& item)
{
  CItem* pItem = NULL;
  CInput* in = NULL;
  COutput* out = NULL;
  bool correctlyDecoded = false;
  QDomElement id = item.firstChildElement("id");
  QDomElement name = item.firstChildElement("name");
  QDomElement flowId = item.firstChildElement("flow");
  QDomElement dataId = item.firstChildElement("data");
  int ID;
  ID = id.text().toInt(&correctlyDecoded);

  if (!id.isNull() && !name.isNull() && (correctlyDecoded == true))
  {
    if (m_isInput)
    {
      pItem = new CInput();
      in = (CInput*) pItem;
    }
    else
    {
      pItem = new COutput();
      out = (COutput*) pItem;
    }
    pItem->setID(ID);
    pItem->setName(name.text());    
  }
  
  if (!flowId.isNull())
  {
    ID = flowId.text().toInt(&correctlyDecoded);
    if (correctlyDecoded == true)
    {
      CFlow* f;
      f = m_implProcess->searchFlow(ID);
      if (f == NULL)
      {
	delete pItem;
	pItem = NULL;
	return pItem;
      }
      else
      {
	if (m_isInput)
	  in->setFlow(f);
	else
	  out->setFlow(f);	  
      }
    }
    else
    {
	delete pItem;
	pItem = NULL;
	return pItem;	
    }
  }
  
  if (!dataId.isNull())
  {
    ID = dataId.text().toInt(&correctlyDecoded);
    if (correctlyDecoded == true)
    {
      CData* d;
      d = m_itemFactory->getDataFactory()->search(ID);
      if (d == NULL)
      {
	delete pItem;
	pItem = NULL;
	return pItem;	
      }
      else
      {
	if (m_isInput)
	  in->setDataFlow(d);
	else
	  out->setDataFlow(d);
      }
    }
    else
    {
	delete pItem;
	pItem = NULL;
	return pItem;	
    }
  }
  
  return pItem;
}

