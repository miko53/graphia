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


#ifndef DIALOGCHOOSEDATA_H
#define DIALOGCHOOSEDATA_H

#include <QDialog>
#include <QTableView>
#include <QSortFilterProxyModel>
#include "diagrams/flow/model/data.h"
#include "diagrams/flow/model/typefactory.h"
#include "dataflowmodel.h"

class CDialogChooseData : public QDialog
{
  Q_OBJECT
  
public:
  explicit CDialogChooseData(CDataFactory* dataFactory, CTypeFactory* typeFactory, QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~CDialogChooseData();

  CData* getChosenData() { return m_dataTaken;}
   
protected:
  CData* m_dataTaken;  
  QTableView* m_table;
  CDataFlowModel* m_model;
  QSortFilterProxyModel* m_proxyModel;
  CTypeFactory* m_typeFactory;

protected slots:
  void changeRegExp(const QString& edit);
  void applyModification();
  void onClickAddButton();

private:
  void buildDialog();
  void buildTableWidget(QTableView* table);

  
};

#endif // DIALOGCHOOSEDATA_H
