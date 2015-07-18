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
#include "toolgroup.h"


CToolGroup::CToolGroup(const QString& name)
{
  m_name = name;
}

CToolGroup::~CToolGroup()
{

}

void CToolGroup::insertToolItem(CToolItem* toolItem)
{
  m_listToolItem.append(toolItem);
}

CToolItem* CToolGroup::getAt(int i)
{
  Q_ASSERT(i < m_listToolItem.size());
  return m_listToolItem[i];
}

int CToolGroup::getNbOfItems()
{
  return m_listToolItem.size();
}
