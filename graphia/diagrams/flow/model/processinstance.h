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
#ifndef PROCESS_INSTANCE_H
#define PROCESS_INSTANCE_H

#include "item.h"

class CFlow;
class CInput;
class COutput;
class CProcessImplementation;

class CConnector 
{
public:
  CInput* m_input;
  CFlow* m_flow;
};

class CProcessInstance : public CItem
{
public:
  CProcessInstance(CProcessImplementation* parent = 0);
  virtual ~CProcessInstance();
  
  enum { CPROCESS_INSTANCE = 1 };
  virtual int type() { return CPROCESS_INSTANCE; }

  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);

  CProcessImplementation* getImplementedProcess() { return m_implementation; }
  void setImplementedProcess(CProcessImplementation* p);
  
  virtual QString getName();
  virtual void setName ( const QString& newName );
  
  void removeAllInputs(void);
  void removeAllOutputs(void); 
  void removeInput(CFlow* in);
  void removeOutput(CFlow* out);
  
  bool nameHasChanged() { return m_nameChanged;}
  void setNameHasChanged(bool b) { m_nameChanged = b;}
  
  int getInputCount() { return m_inputs.count(); }
  CConnector* getInputConnectorAt(int i) { return m_inputs.at(i); }
  
  int getOutputCount() { return m_outputs.count(); }
  CConnector* getOutputConnectorAt(int i) { return m_outputs.at(i); }

  void addInput( CFlow* input, CInput* in );
  void addOutput( CFlow* output, COutput* out );
  
  
protected:
  void addInput(CFlow* input);
  void addOutput(CFlow* output);
  
  
protected:
  CProcessImplementation* m_implementation;
  QList<CConnector*> m_inputs;
  QList<CConnector*> m_outputs;
  bool m_nameChanged;

private:
};

#endif /* PROCESS_INSTANCE_H */
