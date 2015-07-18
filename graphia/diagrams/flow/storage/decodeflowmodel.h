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
#ifndef DECODEFLOWMODEL_H
#define DECODEFLOWMODEL_H

#include "decodeflowdiagram.h"
#include "diagrams/flow/model/itemfactory.h"

class CDecodeFlowModel: public CDecodeXmlFile
{
public:
  CDecodeFlowModel(CItemFactory* pItemFactory);
  virtual bool decode ( const QDomElement& item );

protected:
  CProcessImplementation* readImplementationProcessFirstPass (const QDomElement& element);

protected:
  CItemFactory* m_pItemFactory;
};


class CDecodeFlowList: public CDecodeXmlFile
{
public:
  typedef enum { FIRST_PASS, SECOND_PASS} passType;
  CDecodeFlowList(CItemContainer* pImplProcess, CDataFactory* dataFactory, passType passDecoding);
  virtual bool decode(const QDomElement& item);
  
protected:
  CFlow* readFlowFirstPass(const QDomElement& data);
  bool readFlowSecondPass(const QDomElement& data);

protected:
  CItemContainer* m_implProcess;
  CDataFactory* m_dataFactory;
  passType m_passType;  
};


class CDecodeProcessInstanceList: public CDecodeXmlFile
{
public:
  CDecodeProcessInstanceList(CProcessImplementation* pImplProcess);
  virtual bool decode(const QDomElement& item);
  
protected:
  CProcessInstance* readProcessInstance(const QDomElement& data);

protected:
  CProcessImplementation* m_implProcess;
};


class CDecodeInputOutputListOfProcessInstance: public CDecodeXmlFile //for processinstance
{
public:
    CDecodeInputOutputListOfProcessInstance(CItemContainer* pImplProcess, CProcessInstance* process, bool isInput);
    virtual bool decode(const QDomElement& item);
    
protected:

protected:
  CItemContainer* m_implProcess;
  CProcessInstance* m_processInstance;
  bool m_isInput;
};

class CDecodeStoreList: public CDecodeXmlFile
{
public:
  CDecodeStoreList(CItemContainer* pImplProcess);
  virtual bool decode(const QDomElement& item);
  
protected:
  CStore* readStore(const QDomElement& store);
  
protected:
  CItemContainer* m_implProcess;
};


class CDecodeInputOutputListOfStore: public CDecodeXmlFile
{
public:
  CDecodeInputOutputListOfStore(CItemContainer* pImplProcess, CStore* pStore, bool isInput);
  virtual bool decode(const QDomElement& item);
  
protected:
  
protected:
  CItemContainer* m_implProcess;
  CStore* m_store;
  bool m_isInput;
};

class CDecodeInputOutputList: public CDecodeXmlFile
{
public:
  CDecodeInputOutputList(CProcessImplementation* impl, CItemFactory* factory, bool isInput);
  virtual bool decode(const QDomElement& item);
    
protected:
  CItem* readItemData(const QDomElement& item);
  
protected:
  CProcessImplementation* m_implProcess;
  CItemFactory* m_itemFactory;
  bool m_isInput;
};


#endif /* DECODEFLOWMODEL_H */


