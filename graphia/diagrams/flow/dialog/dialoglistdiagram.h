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


#ifndef DIALOGLISTDIAGRAM_H
#define DIALOGLISTDIAGRAM_H
#include <QDialog>
#include <QCheckBox>

class CMainWidget;
class CGraphicsScene;
class QTableWidget;
class CDocument;

class CContainerCheckBoxAndDiagram: public QWidget
{
public:
  CContainerCheckBoxAndDiagram(CGraphicsScene* diagram, QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~CContainerCheckBoxAndDiagram();

  QCheckBox* getCheckBox() { return m_cb;}
  CGraphicsScene* getDiagram() { return m_diagram; }
  
protected:
  CGraphicsScene* m_diagram;
  QCheckBox* m_cb;
  
private:
};


class CDialogListDiagram : public QDialog
{
  Q_OBJECT
  
public:
  explicit CDialogListDiagram(CDocument* doc, CMainWidget* mainWidget, QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~CDialogListDiagram();

protected slots:
  void onCloseDiagrams();
  void onDeleteDiagrams();
  void onDisplayDiagrams();
  void onSectionClicked(int index);
  
protected:
  void closeDiagram(CGraphicsScene* diagram);
  void deleteDiagram(CGraphicsScene* diagram);
  void displayDiagram(CGraphicsScene* diagram);
  
protected:
  CDocument* m_doc;
  CMainWidget* m_mainWidget;
  QList<CContainerCheckBoxAndDiagram*> m_listContainer;
  QTableWidget* m_table;
  
private:
  void buildDialog();
  void buildTableWidget(QTableWidget* table);
  void updateTableWidget(QTableWidget* table);
};

#endif // DIALOGLISTDIAGRAM_H
