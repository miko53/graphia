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

#ifndef TERMINATOR_H
#define TERMINATOR_H

#include "item.h"
#include "data.h"

class CTerminator : public CItem
{
public:
  CTerminator(CItemContainer* parent = 0);
  virtual ~CTerminator();
  
  virtual bool dissociate(CItem* item, CGraphicsItem::LinkOrder order);
  virtual bool associateWith(CItem* item, CGraphicsItem::LinkOrder order);
  
  enum { CTERMINATOR = 8 };
  virtual int type() { return CTERMINATOR; }

  void removeAttachedFlow(void);

  void setDataFlow(CData* data) { m_dataFlow = data; }
  CData* getDataFlow(void) { return m_dataFlow; }
  
  virtual QString getName();
  virtual void setName ( const QString& newName );
  bool nameHasChanged() { return m_nameChanged; }
  void setNameHasChanged(bool b) { m_nameChanged = b; }
  int getType() { return m_type; }
  void setType(int type) { m_type = type; }
  CFlow* getInOrOutFlow() { return m_inOrOut; }
  void setInOrOutFlow(CFlow* f) { m_inOrOut = f;}
  
protected:
  CFlow* m_inOrOut;
  enum { INPUT, OUTPUT, NONE };
  int m_type;
  CData* m_dataFlow;
  bool m_nameChanged;
  
private:
};

#endif // TERMINATOR_H
