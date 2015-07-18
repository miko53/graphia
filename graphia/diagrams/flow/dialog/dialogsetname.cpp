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

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "dialog.h"


CDialogSetName::CDialogSetName(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  setWindowTitle(tr("graphia - modify the name"));
  buildDialog();
}


void CDialogSetName::buildDialog()
{
  QGridLayout* g1 = new QGridLayout();  
  QLabel* lbl = new QLabel(tr("name : "));
  m_editName = new QLineEdit();
  m_editName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  QDialogButtonBox* b1 = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal);
  b1->setCenterButtons(false);
  
  connect(b1, SIGNAL(accepted()), this, SLOT(accept()));
  connect(b1, SIGNAL(rejected()), this, SLOT(reject()));
  
  g1->addWidget(lbl, 0, 0);
  g1->addWidget(m_editName, 0, 1);
  
  QVBoxLayout* h1 = new QVBoxLayout();
  h1->addLayout(g1);
  h1->addSpacing(20);
  h1->addWidget(b1);
  
  setLayout(h1); 
  setMinimumWidth(400);
}

QString CDialogSetName::getName()
{
  return m_editName->text();
}


CDialogSetName::~CDialogSetName()
{

}
