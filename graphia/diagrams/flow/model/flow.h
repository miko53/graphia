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
#ifndef FLOW_H
#define FLOW_H

#include "item.h"
#include "data.h"

class CFlow : public CItem
{
public:
  CFlow(CItemContainer* parent = NULL);
  virtual ~CFlow();
    
  enum { CFLOW = 2 };
  int type() { return CFLOW; }
  
  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);

  CItem* getInput() { return m_input; }
  CItem* getOutput() { return m_output;}
  
  void setInput(CItem* input) { m_input = input; }
  void setOutput(CItem* output) { m_output = output; }
  
  void setDataFlow(CData* data) { m_dataFlow = data; }
  CData* getDataFlow(void) { return m_dataFlow; }
  
  virtual QString getName();
  virtual void setName ( const QString& newName );
  
protected:
  CItem* m_input;
  CItem* m_output;
  CData* m_dataFlow;

private:

};

#endif /* FLOW_H */
