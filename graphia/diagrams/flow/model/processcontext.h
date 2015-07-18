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

#ifndef PROCESSCONTEXT_H
#define PROCESSCONTEXT_H

#include "itemcontainer.h"
#include "terminatorfactory.h"
#include "flowfactory.h"
#include "storefactory.h"

class CFlow;
class CStore;
class CGraphicsScene;
class CProcessInstance;

class CProcessContext : public CItemContainer
{
public:
  CProcessContext(CItemContainer* parent = 0);  
  virtual ~CProcessContext();
    
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
    
  enum { CPROCESS_CONTEXT = 7 };
  virtual int type() { return CPROCESS_CONTEXT; }

  virtual CItem* createItem(int type);      
  virtual void removeItem(CItem* item);
  //virtual void removeAll(void);
  
  CProcessInstance* processInstance() { return m_instanceOfContext; }
  void setProcessInstance(CProcessInstance* p) { m_instanceOfContext = p; }
  
  CGraphicsScene* scene() { return m_scene; }
  void setScene(CGraphicsScene* scene) { m_scene = scene; }
  
  virtual QString getName();
  virtual void setName ( const QString& newName );
  virtual bool renameObject ( CItem* m_item, const QString& name );
  
  bool nameHasChanged() { return m_nameChanged; }
  int getFLowcount() { return m_flowFactory.getCount(); }
  CFlow* getFlowAt ( int index ) { return m_flowFactory.getAt(index); }
  int getStoreCount() { return m_storeFactory.getCount(); }
  CStore* getStoreAt ( int index ) { return m_storeFactory.getAt(index); }
  int getTerminatorCount()  { return m_terminatorFactory.getCount(); }
  CTerminator* getTerminatorAt ( int index ) { return m_terminatorFactory.getAt(index); }
  void setNameHasChanged(bool b) { m_nameChanged = b;}
  
  virtual bool insertItem ( CItem* item );
  virtual CFlow* searchFlow ( int ID ) { return m_flowFactory.search(ID); }
  virtual CItem* searchItem ( int type, int ID );
  
protected:
  
protected:
  CTerminatorFactory m_terminatorFactory;
  CFlowFactory m_flowFactory;
  CStoreFactory m_storeFactory;
  CGraphicsScene* m_scene;
  CProcessInstance* m_instanceOfContext;
  bool m_nameChanged;
  
private:
  
};

#endif // PROCESSCONTEXT_H
