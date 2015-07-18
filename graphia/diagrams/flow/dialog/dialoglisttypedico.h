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

#ifndef DIALOGLISTDATATYPE_H
#define DIALOGLISTDATATYPE_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
class CDocument;


class CDialogListTypeDico : public QDialog
{
  Q_OBJECT
public:
  explicit CDialogListTypeDico(CDocument* doc, QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~CDialogListTypeDico();
  
protected: 
  QPushButton* m_bDelete;
  QTableWidget* m_table;
  CDocument* m_doc;

protected slots:
  void onCurrentCellChanged(int row, int, int, int);
  
private:
  void buildDialog();
  void buildTableWidget(QTableWidget* table);
  void updateTableWidget(QTableWidget* table);
};

#endif // DIALOGLISTDATADICO_H
