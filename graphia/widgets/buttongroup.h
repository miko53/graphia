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
#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QButtonGroup>
#include "toolbutton.h"


class CButtonGroup : public QButtonGroup
{
  Q_OBJECT
  public:    
    CButtonGroup(QObject* parent = 0);
    void addButton(CToolButton * button, int id);
    virtual ~CButtonGroup();

signals:
    void buttonClicked(int id, CToolButton::STATE state);

public slots:
    void OnButtonClicked(int id);

};

#endif // BUTTONGROUP_H
