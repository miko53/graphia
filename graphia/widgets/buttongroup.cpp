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
#include "buttongroup.h"

CButtonGroup::CButtonGroup(QObject* parent): QButtonGroup(parent)
{
  connect(this, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClicked(int)));
}


CButtonGroup::~CButtonGroup()
{

}

void CButtonGroup::addButton(CToolButton* button, int id)
{
  QButtonGroup::addButton(button, id);
}


void CButtonGroup::OnButtonClicked(int id)
{
  CToolButton::STATE state = ((CToolButton *) button(id))->getState();

  emit buttonClicked(id, state);
}
