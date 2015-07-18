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

#ifndef INPUT_H
#define INPUT_H

#include "item.h"
#include "data.h"

class CFlow;

class CInput : public CItem
{
public:
  CInput(CItemContainer* parent = 0);
  virtual ~CInput();
  
  enum { CINPUT = 5 };
  virtual int type() { return CINPUT;}

  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);    

  virtual QString getName();
  virtual void setName ( const QString& newName );
  
  CFlow* getFlow() { return m_flow;}
  void setFlow(CFlow* flow) { m_flow = flow; }
  
  void setDataFlow(CData* data) { m_dataFlow = data; }
  CData* getDataFlow(void) { return m_dataFlow; }
  
protected:
  CFlow* m_flow;
  CData* m_dataFlow;
};

#endif // INPUT_H
