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

#include "dialoglisttypedico.h"
#include "core/document.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>


CDialogListTypeDico::CDialogListTypeDico(CDocument* doc, QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  m_doc = doc;
  setWindowTitle(tr("graphia - list of data types"));
  buildDialog();
  
}

CDialogListTypeDico::~CDialogListTypeDico()
{

}

void CDialogListTypeDico::buildDialog()
{
  QVBoxLayout* v1 = new QVBoxLayout();
  QLabel* l1 = new QLabel(tr("List of whole data types managed."));
  
  QHBoxLayout* h1 = new QHBoxLayout();
  QPushButton* bAdd = new QPushButton(QIcon(":/create.png"), tr("Add type"));
  m_bDelete = new QPushButton(QIcon(":/erase.png"), tr("Delete type"));
  QPushButton* bDetails = new QPushButton(QIcon(":/properties.png"), tr("Details..."));
  
 /* connect(bDisplay, SIGNAL(clicked(bool)), this, SLOT(onDisplayDiagrams()));
  connect(bClose, SIGNAL(clicked(bool)), this, SLOT(onCloseDiagrams()));
  connect(bDelete, SIGNAL(clicked(bool)), this, SLOT(onDeleteDiagrams()));*/
  
  h1->setAlignment(Qt::AlignCenter);
  h1->addWidget(bAdd);
  h1->addWidget(m_bDelete);
  h1->addWidget(bDetails);
  
  m_table = new QTableWidget();
  
  buildTableWidget(m_table);
  updateTableWidget(m_table);
  
  QPushButton* b1 = new QPushButton(tr("Close")); 
  connect(b1, SIGNAL(clicked(bool)), this, SLOT(accept()));
  
  v1->addWidget(l1);
  v1->addLayout(h1);
  v1->addWidget(m_table);
  v1->addWidget(b1, 0, Qt::AlignRight);
  
  setLayout(v1);
}

void CDialogListTypeDico::buildTableWidget(QTableWidget* table)
{
  table->setColumnCount(2);
  table->setHorizontalHeaderLabels(QString(tr("Name;Definition")).split(";"));
  table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  table->setMinimumWidth(500);
  table->setMinimumHeight(400);
  table->horizontalHeader()->setStretchLastSection(true);
  table->verticalHeader()->setVisible(false);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  connect(table, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onCurrentCellChanged(int,int, int,int)));
}

void CDialogListTypeDico::onCurrentCellChanged(int row, int , int, int)
{
  //qDebug("oncurrentCellChanged (row=%i)", row);
  CClass* selectedClass;
  QList<CClass*>* listType = m_doc->getItemFactory()->getTypeFactory()->getList();
  selectedClass = (*listType)[row];
  if (selectedClass->isReadOnly())
  {
    m_bDelete->setEnabled(false);
  }
  else
  {
    m_bDelete->setEnabled(true);
  }
}

void CDialogListTypeDico::updateTableWidget(QTableWidget* table)
{
  m_table->clearContents();
  
  QList<CClass*>* listType;
  listType = m_doc->getItemFactory()->getTypeFactory()->getList();
  CClass* currentClass;
  
  int nbItems = listType->count();
  table->setRowCount(nbItems);

  int i = 0;
  for(i = 0; i < listType->size(); i++)
  {
    currentClass = (*listType)[i];
    if (currentClass != NULL)
    {
      table->setItem(i,0,new QTableWidgetItem(currentClass->getName()));
      table->setItem(i,1,new QTableWidgetItem(currentClass->getDefinition()));
    }
    else
    {
      qDebug("currentClass == NULL");
    }
  }
  
  table->resizeRowsToContents();
  table->resizeColumnsToContents();
  table->horizontalHeader()->setStretchLastSection(true);  
}

