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
#include "toolitem.h"


CToolItem::CToolItem(const QString& name, int id, const QIcon& icon1, const QIcon& icon2, void* graphicItem)
{
  m_name = name;
  m_id = id;
  m_icon1 = icon1;
  m_icon2 = icon2;
  m_graphicItem = graphicItem;
  m_isDefault = false;
}


CToolItem::~CToolItem()
{

}
