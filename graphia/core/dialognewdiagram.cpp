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

#include "dialognewdiagram.h"
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
 
CDialogNewDiagram::CDialogNewDiagram(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  setWindowTitle(tr("graphia - add a new diagram"));
  buildDialog();
  updateCombo();
}


void CDialogNewDiagram::buildDialog()
{
  QGridLayout* g1 = new QGridLayout();
  
  QLabel* labelName = new QLabel(tr("Diagram name :"));
  m_editName = new QLineEdit();
  m_editName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  QLabel* labelprocessName = new QLabel(tr("Process name :"));
  m_editProcessName = new QLineEdit();
  m_editProcessName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  QLabel* labelType = new QLabel(tr("Type of diagram :"));
  m_comboType = new QComboBox();
  m_comboType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  g1->addWidget(labelName, 0, 0);
  g1->addWidget(m_editName, 0, 1);
  g1->addWidget(labelprocessName, 1, 0);
  g1->addWidget(m_editProcessName, 1, 1);  
  g1->addWidget(labelType, 2, 0);
  g1->addWidget(m_comboType, 2, 1);
  
  QDialogButtonBox* b1 = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Vertical);
  b1->setCenterButtons(true);
  connect(b1, SIGNAL(accepted()), this, SLOT(accept()));
  connect(b1, SIGNAL(rejected()), this, SLOT(reject()));
  
  QHBoxLayout* h1 = new QHBoxLayout();
  h1->addLayout(g1);
  h1->addSpacing(20);
  h1->addWidget(b1);
  
  setLayout(h1); 
}

void CDialogNewDiagram::updateCombo()
{
  m_comboType->addItem(tr("Context Diagram"), 0);
  m_comboType->addItem(tr("Process Diagram"), 1);
}

int CDialogNewDiagram::getType()
{
  int currentIndex;
  currentIndex= m_comboType->currentIndex();
  
  return m_comboType->itemData(currentIndex).toInt();
}


CDialogNewDiagram::~CDialogNewDiagram()
{

}

