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


#include "dialoglistdiagram.h"
#include "core/document.h"
#include "core/mainwidget.h"
#include "graphia.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>


CContainerCheckBoxAndDiagram::CContainerCheckBoxAndDiagram(CGraphicsScene* diagram, QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  m_diagram = diagram;
  m_cb = new QCheckBox();
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(m_cb);
  setLayout(layout);
}

CContainerCheckBoxAndDiagram::~CContainerCheckBoxAndDiagram()
{
}



CDialogListDiagram::CDialogListDiagram(CDocument* doc,  CMainWidget* mainWidget, QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  Q_ASSERT(doc != NULL);
  m_doc = doc;
  m_mainWidget = mainWidget;
  
  setWindowTitle(tr("graphia - diagram list"));
  buildDialog();
}

CDialogListDiagram::~CDialogListDiagram()
{
  foreach(CContainerCheckBoxAndDiagram* container, m_listContainer)
  {
    delete container;
  }
  m_listContainer.clear();
}

void CDialogListDiagram::buildDialog()
{
  QVBoxLayout* v1 = new QVBoxLayout();
  QLabel* l1 = new QLabel(tr("List of whole diagrams managed."));
  
  QHBoxLayout* h1 = new QHBoxLayout();
  QPushButton* bDisplay = new QPushButton(QIcon(":/display_diagram.png"), tr("Display"));
  QPushButton* bClose = new QPushButton(QIcon(":/close.png"), tr("Close"));
  QPushButton* bDelete = new QPushButton(QIcon(":/erase.png"), tr("Delete"));
  
  connect(bDisplay, SIGNAL(clicked(bool)), this, SLOT(onDisplayDiagrams()));
  connect(bClose, SIGNAL(clicked(bool)), this, SLOT(onCloseDiagrams()));
  connect(bDelete, SIGNAL(clicked(bool)), this, SLOT(onDeleteDiagrams()));
  
  h1->setAlignment(Qt::AlignCenter);
  h1->addWidget(bDisplay);
  h1->addWidget(bClose);
  h1->addWidget(bDelete);
  
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

void CDialogListDiagram::buildTableWidget(QTableWidget* table)
{
  table->setColumnCount(4);
  table->setHorizontalHeaderLabels(QString(tr(";Type;Diagram Name;Process")).split(";"));
  table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  table->setMinimumWidth(350);
  table->horizontalHeader()->setStretchLastSection(true);
  
  connect(table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onSectionClicked(int)));
  m_table->horizontalHeaderItem(0)->setIcon(QIcon(":/square.png"));
  table->horizontalHeaderItem(0)->setToolTip(tr("Click here to select all"));
  table->verticalHeader()->setVisible(false);  
  
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  //table->setSelectionMode(QAbstractItemView::NoSelection);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void CDialogListDiagram::updateTableWidget(QTableWidget* table)
{
  QString nameOfTypeOfDiagram;
  
  m_table->clearContents();
  
  int nbItems = m_doc->getNbOfDiagram();
  table->setRowCount(nbItems);

  for(int i = 0; i < nbItems; i++)
  {
    CContainerCheckBoxAndDiagram* container = new CContainerCheckBoxAndDiagram(m_doc->getAt(i));
    
    m_listContainer.append(container);
    
    table->setCellWidget(i, 0, container);
        
    switch (m_doc->getAt(i)->getType()) //TODO create a class DiagramType
    {
      case 0:
        nameOfTypeOfDiagram = tr("Context Diagram");
        break;
        
      case 1:
        nameOfTypeOfDiagram = tr("Process Diagram");
        break;
        
      default:
        Q_ASSERT(false);
        break;
    }
    table->setItem(i,1,new QTableWidgetItem(nameOfTypeOfDiagram));
    table->setItem(i,2,new QTableWidgetItem(m_doc->getAt(i)->getTitle()));
    table->setItem(i,3,new QTableWidgetItem(m_doc->getAt(i)->getItem()->getName()));
  }
  
  table->resizeRowsToContents();
  table->resizeColumnsToContents();
  table->horizontalHeader()->setStretchLastSection(true);

}

void CDialogListDiagram::onSectionClicked(int index)
{
  static Qt::CheckState state = Qt::Unchecked;
  
  if (index == 0)
  {
    if (state == Qt::Checked)
    {
      state = Qt::Unchecked;
      m_table->horizontalHeaderItem(0)->setIcon(QIcon(":/square.png"));
      m_table->horizontalHeaderItem(0)->setToolTip(tr("Click here to select all"));
    }
    else
    {
      state = Qt::Checked;
      m_table->horizontalHeaderItem(0)->setIcon(QIcon(":/checked.png"));
      m_table->horizontalHeaderItem(0)->setToolTip(tr("Click here to unselect all"));
    }
    
    foreach(CContainerCheckBoxAndDiagram* checkBox, m_listContainer)
    {
      checkBox->getCheckBox()->setCheckState(state);
    }
  }
}


void CDialogListDiagram::onDisplayDiagrams()
{
  int index;

  for(index = 0; index < m_listContainer.count(); index++)
  {
    if ((m_listContainer.at(index)->getCheckBox()->checkState() == Qt::Checked) &&
         m_listContainer.at(index)->getDiagram()->isDisplayed() == false)
    {
      m_mainWidget->displayDiagram(m_listContainer.at(index)->getDiagram());
    }
  }
}

void CDialogListDiagram::onCloseDiagrams()
{
  int index;
  
  for(index = 0; index < m_listContainer.count(); index++)
  {
    if ((m_listContainer.at(index)->getCheckBox()->checkState() == Qt::Checked) &&
         m_listContainer.at(index)->getDiagram()->isDisplayed() == true)
    {
      m_mainWidget->closeDiagram(m_listContainer.at(index)->getDiagram());
    }
  }  
}


void CDialogListDiagram::onDeleteDiagrams()
{
  QMessageBox msg(QMessageBox::Warning, tr("Delete Diagram confirmation"), tr("The selected diagrams will be deleted\nAll references to theses diagrammes in another diagrams will be deleted\nAre you sure ?"),
    QMessageBox::Yes|QMessageBox::No);
  int action;
  
  action = msg.exec();
  if (action == QMessageBox::Yes)
  {
    int index;
    for (index = 0; index < m_listContainer.count(); index++)
    {
      if ((m_listContainer.at(index)->getCheckBox()->checkState() == Qt::Checked))
      {
        m_doc->removeDiagram(m_listContainer.at(index)->getDiagram());
      }
    }
    
    foreach(CContainerCheckBoxAndDiagram* b, m_listContainer)
    {
      delete b;
    }
    
    m_listContainer.clear();
    updateTableWidget(m_table);
  }
}



