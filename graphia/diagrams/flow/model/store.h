/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#ifndef STORE_H
#define STORE_H

#include "item.h"

class CFlow;
class CProcessImplementation;

class CStore : public CItem
{
public:
  CStore(CItemContainer* parent = NULL);
  virtual ~CStore();

  enum { CSTORE = 4 };
  virtual int type() { return CSTORE; }
  
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
  
  void addInput(CFlow* input);
  void addOutput(CFlow* output);
  
  void removeInput(CFlow* in);
  void removeOutput(CFlow* out);

  void removeAllInputs(void);
  void removeAllOutputs(void); 

  virtual QString getName();
  virtual void setName ( const QString& newName );
  bool nameHasChanged() { return m_nameChanged;}
  void setNameHasChanged(bool b) { m_nameChanged = b; }

  int getInputCount() { return m_inputs.count(); }
  CFlow* getInputFlowAt(int index) { return m_inputs.at(index); }
  
  int getOutputCount() { return m_outputs.count(); }
  CFlow* getOutputFlowAt(int index) { return m_outputs.at(index); }
  
protected:
  QList<CFlow*> m_inputs;
  QList<CFlow*> m_outputs;
  bool m_nameChanged;

protected:

};

#endif // STORE_H
