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

#ifndef PROCESS_IMPLEMENTATION_H
#define PROCESS_IMPLEMENTATION_H

#include "itemcontainer.h"
#include "flowfactory.h"
#include "processinstancefactory.h"
#include "storefactory.h"
#include "inputfactory.h"
#include "outputfactory.h"

class CFlow;
class CStore;
class CProcessInstance;
class CInput;
class COutput;
class CGraphicsScene;

class CProcessImplementation: public CItemContainer
{
public:
  CProcessImplementation(CProcessImplementation* parent = 0);
  virtual ~CProcessImplementation();
  
  enum { CPROCESS_IMPL = 3 };
  virtual int type() { return CPROCESS_IMPL; }
  
  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);
  
  enum { SPEC, COMPOSED };
  int typeProcess() { return m_typeProcess; }
  
  CGraphicsScene* scene() { return m_scene; }
  void setScene(CGraphicsScene* scene) { m_scene = scene; }
  
  virtual void setName ( const QString& newName );
  
  
  virtual CItem* createItem(int type);
  virtual void removeItem(CItem* item);
  virtual bool renameObject ( CItem* m_item, const QString& name );
  
  void addInstance(CProcessInstance* process);

  int getFLowcount() { return m_flowFactory.getCount(); }
  CFlow* getFlowAt(int index) { return m_flowFactory.getAt(index); }
  
  int getProcessInstanceCount() { return m_processInstFactory.getCount(); }
  CProcessInstance* getProcessInstanceAt ( int index ) { return m_processInstFactory.getAt(index); }
  
  int getStoreCount() { return m_storeFactory.getCount(); }
  CStore* getStoreAt(int index) { return m_storeFactory.getAt(index);}
  
  int getInputCount() { return m_inputFactory.getCount(); }
  CInput* getInputAt(int index) { return m_inputFactory.getAt(index);}

  int getOutputCount() { return m_outputFactory.getCount(); }
  COutput* getOutputAt(int index) { return m_outputFactory.getAt(index);}
  
  
  virtual CFlow* searchFlow(int ID) { return m_flowFactory.search(ID); }
  virtual CItem* searchItem(int type, int ID);
  virtual bool insertItem(CItem* item);
  
protected:
  CFlowFactory m_flowFactory;
  CProcessInstanceFactory m_processInstFactory;
  CStoreFactory m_storeFactory;
  CInputFactory m_inputFactory;
  COutputFactory m_outputFactory;

  QList<CProcessInstance*> m_InstanciatedProcess; //list of process which references this implementation

  CGraphicsScene* m_scene;

  int m_typeProcess;
  bool m_actif;
  void* m_spec;
  
private:
  void removeAll(void);

};


#endif /* PROCESS_IMPLEMENTATION */