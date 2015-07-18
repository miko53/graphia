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
#include "mainwindow_p.h"
#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include "dialognewdiagram.h"
#include "diagrams/flow/dialog/dialoglistdiagram.h"
#include "diagrams/flow/dialog/dialoglisttypedico.h"
#include "mainwidget.h"
#include <QDesktopWidget>
#include <QFileDialog>


CMainWindowPrivate::CMainWindowPrivate(CMainWindow *mainWindow, CApplication* app) :
    m_mainWindow(mainWindow), m_app(app)
{
  m_mainWidget = NULL;
  m_untitledDocumentNumber = 0;
  buildName();
  
  connect(getDoc(), SIGNAL(documentModified(bool)),
          this, SLOT(onDocumentModified(bool)));
}

CMainWindowPrivate::~CMainWindowPrivate()
{
	//TODO: remove all action
}

void CMainWindowPrivate::buildName()
{
  m_hasUntitledName = true;
  m_displayName = tr("untitled-%1").arg(m_untitledDocumentNumber);
  m_fullFileName = m_displayName;
  m_untitledDocumentNumber++;
}


CMainWidget* CMainWindowPrivate::createMainWidget()
{
  Q_ASSERT(m_mainWidget == NULL);
  m_mainWidget = new CMainWidget(m_mainWindow);
  return m_mainWidget;
}


void CMainWindowPrivate::setupAction()
{
  setupActionFile();
  setupActionEdit();
  setupActionDisplay();
  setupActionDiagram();
  setupActionWindow();
  setupActionHelp();
}

void CMainWindowPrivate::setupActionFile()
{
  QAction* action;

  action = new QAction(tr("&New"), m_mainWindow);
  action->setShortcuts(QKeySequence::New);
  action->setIcon(QIcon(":/new_file.png"));
  action->setStatusTip(tr("Create new shema"));
  connect(action, SIGNAL(triggered()), this, SLOT(onNewFile()));
  m_hashAction.insert("new", action);

  action = new QAction(tr("&Open"), m_mainWindow);
  action->setShortcuts(QKeySequence::Open);
  action->setIcon(QIcon(":/open_file.png"));
  action->setStatusTip(tr("Open a shema"));
  connect(action, SIGNAL(triggered()), this, SLOT(open()));
  m_hashAction.insert("open", action);

  action = new QAction(tr("&Save"), m_mainWindow);
  action->setShortcuts(QKeySequence::Save);
  action->setIcon(QIcon(":/save.png"));
  action->setStatusTip(tr("Save the current shema"));
  connect(action, SIGNAL(triggered()), this, SLOT(save()));
  m_hashAction.insert("save", action);

  action = new QAction(tr("Save as"), m_mainWindow);
  action->setShortcuts(QKeySequence::SaveAs);
  action->setIcon(QIcon(":/save_as.png"));
  action->setStatusTip(tr("Save the current shema under another name"));
  connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));
  m_hashAction.insert("save_as", action);

  action = new QAction(tr("Print"), m_mainWindow);
  action->setShortcuts(QKeySequence::Print);
  action->setIcon(QIcon(":/print.png"));
  action->setStatusTip(tr("Print the diagram"));
//  connect(action, SIGNAL(triggered()), this, SLOT());
  m_hashAction.insert("print", action);

  action = new QAction(tr("Export to PDF"), m_mainWindow);
  action->setIcon(QIcon(":/export.png"));
  action->setStatusTip(tr("Export the diagram as PDF file"));
//  connect(action, SIGNAL(triggered()), this, SLOT());
  m_hashAction.insert("export_pdf", action);

  action = new QAction(tr("Export to PNG"), m_mainWindow);
  action->setIcon(QIcon(":/picture.png"));
  action->setStatusTip(tr("Export the diagram as png picture"));
//  connect(action, SIGNAL(triggered()), this, SLOT());
  m_hashAction.insert("export_png", action);
  
  
	action = new QAction(tr("&Quit"), m_mainWindow);
	action->setShortcut(Qt::ControlModifier+Qt::Key_Q);
	action->setIcon(QIcon(":/exit.png"));
	action->setStatusTip(tr("Quit the application"));
	connect(action, SIGNAL(triggered()), m_mainWindow, SLOT(close()));
	m_hashAction.insert("quit", action);
}

void CMainWindowPrivate::setupActionEdit()
{
  QAction* action;

  action = new QAction(tr("&Undo"), m_mainWindow);
  action->setShortcuts(QKeySequence::Undo);
  action->setIcon(QIcon(":/undo.png"));
  action->setStatusTip(tr("undo"));
  action->setEnabled(false);//TODO
  //connect(action, SIGNAL(triggered()), this, )
  m_hashAction.insert("undo", action);

  action = new QAction(tr("&Cut"), m_mainWindow);
  action->setShortcuts(QKeySequence::Cut);
  action->setIcon(QIcon(":/cut.png"));
  action->setStatusTip(tr("cut"));
  action->setEnabled(false);//TODO
  //connect(action, SIGNAL(triggered()), this, )
  m_hashAction.insert("cut", action);
  
  action = new QAction(tr("&Copy"), m_mainWindow);
  action->setShortcuts(QKeySequence::Copy);
  action->setIcon(QIcon(":/copy.png"));
  action->setStatusTip(tr("copy"));
  action->setEnabled(false);//TODO
  //connect(action, SIGNAL(triggered()), this, )
  m_hashAction.insert("copy", action);

  action = new QAction(tr("&Paste"), m_mainWindow);
	action->setShortcuts(QKeySequence::Paste);
	action->setIcon(QIcon(":/paste.png"));
	action->setStatusTip(tr("paste"));
  action->setEnabled(false);//TODO
  //connect(action, SIGNAL(triggered()), this, )
  m_hashAction.insert("paste", action);
}

void CMainWindowPrivate::setupActionDisplay()
{
  QAction* action;

  action = new QAction(tr("real size"), m_mainWindow);
  //action->setShortcuts(QKeySequence::Undo);
  action->setIcon(QIcon(":/zoom_11.png"));
  action->setStatusTip(tr("set diagram at its real size"));
  action->setEnabled(false);
  connect(action, SIGNAL(triggered()), this, SLOT(onZoomNormal()));
  m_hashAction.insert("zoom_11", action);
  
  action = new QAction(tr("zoom in"), m_mainWindow);
  //action->setShortcuts(QKeySequence::Undo);
  action->setIcon(QIcon(":/zoom_in.png"));
  action->setStatusTip(tr("zoom in on diagram"));
  action->setEnabled(false);
  connect(action, SIGNAL(triggered()), this, SLOT(onZoomIn()));
  m_hashAction.insert("zoom_in", action);

  action = new QAction(tr("zoom out"), m_mainWindow);
  //action->setShortcuts(QKeySequence::Undo);
  action->setIcon(QIcon(":/zoom_out.png"));
  action->setStatusTip(tr("zoom out on diagram"));
  action->setEnabled(false);
  connect(action, SIGNAL(triggered()), this, SLOT(onZoomOut()));
  m_hashAction.insert("zoom_out", action);
  
}


void CMainWindowPrivate::setupActionDiagram()
{
  QAction* action;
  
  action = new QAction(tr("&Add a new diagram"), m_mainWindow);
  action->setShortcut(Qt::ControlModifier+Qt::Key_A);
  action->setIcon(QIcon(":/create.png"));
  action->setStatusTip(tr("add a new diagram to the current document"));
  connect(action, SIGNAL(triggered()), this, SLOT(addNewDiagram()));
  m_hashAction.insert("add_new_diagram", action);

  action = new QAction(tr("Display &list of diagram"), m_mainWindow);
  action->setShortcut(Qt::ControlModifier+Qt::Key_L);
  action->setIcon(QIcon(":/form.png"));
  action->setStatusTip(tr("display the list of diagram of the document"));
  connect(action, SIGNAL(triggered()), this, SLOT(displayListOfDiagram()));
  m_hashAction.insert("display_diagram_list", action);
  
  action = new QAction(tr("Display list of data &types"), m_mainWindow);
  action->setShortcut(Qt::ControlModifier+Qt::Key_T);
  action->setIcon(QIcon(":/form.png"));
  action->setStatusTip(tr("display the list of data types of the document"));
  connect(action, SIGNAL(triggered()), this, SLOT(displayListOfDataTypes()));
  m_hashAction.insert("display_data_list", action);

  action = new QAction(tr("Display data dictionary"), m_mainWindow);
  action->setShortcut(Qt::ControlModifier+Qt::Key_G);
  //action->setIcon(QIcon(":/form.png"));
  action->setStatusTip(tr("display the global data dictionary of the document"));
  //connect(action, SIGNAL(triggered()), this, SLOT(displayListOfDataTypes()));
  m_hashAction.insert("display_data_dico", action);
}


void CMainWindowPrivate::setupActionWindow()
{
  QAction* action;

  action = new QAction(tr("&Display tool box"), m_mainWindow);
  action->setShortcut(Qt::ControlModifier+Qt::Key_D);
  action->setStatusTip(tr("display the tool box"));
	connect(action, SIGNAL(triggered()), this, SLOT(displayToolBox()));
  m_hashAction.insert("display_tool_box", action);

  action = new QAction(tr("Display &properties"), m_mainWindow);
  action->setShortcut(Qt::ControlModifier+Qt::Key_P);
  action->setStatusTip(tr("display the property window"));
  connect(action, SIGNAL(triggered()), this, SLOT(displayPropertyBox()));
  m_hashAction.insert("display_property_box", action);
}

void CMainWindowPrivate::setupActionHelp()
{
  QAction* action;
  
  action = new QAction(tr("&About graphia"), m_mainWindow);
  action->setStatusTip(tr("about graphia"));
	connect(action, SIGNAL(triggered()), this, SLOT(displayAboutWnd()));
  m_hashAction.insert("about_graphia", action);
}


void CMainWindowPrivate::setupMenu()
{
  setupMenuFile();
  setupMenuEdit();
  setupMenuDisplay();
  setupMenuDiagram();
  setupMenuWindow();
  setupMenuHelp();
}

void CMainWindowPrivate::setupMenuFile()
{
	QMenu* menu;

	menu = m_mainWindow->menuBar()->addMenu(tr("&File"));
  menu->addAction(m_hashAction["new"]);
  menu->addAction(m_hashAction["open"]);
  menu->addAction(m_hashAction["save"]);
  menu->addAction(m_hashAction["save_as"]);
  menu->addSeparator();
  menu->addAction(m_hashAction["print"]);
  menu->addAction(m_hashAction["export_pdf"]);
  menu->addAction(m_hashAction["export_png"]);
  menu->addSeparator();  
  menu->addAction(m_hashAction["quit"]);
}

void CMainWindowPrivate::setupMenuEdit()
{
  QMenu* menu;

  menu = m_mainWindow->menuBar()->addMenu(tr("&Edit"));
  menu->addAction(m_hashAction["undo"]);
  menu->addSeparator();
  menu->addAction(m_hashAction["cut"]);
  menu->addAction(m_hashAction["copy"]);
  menu->addAction(m_hashAction["paste"]);
  
}

void CMainWindowPrivate::setupMenuDisplay()
{
  QMenu* menu;
  menu = m_mainWindow->menuBar()->addMenu(tr("D&isplay"));
  menu->addAction(m_hashAction["zoom_11"]);
  menu->addSeparator();
  menu->addAction(m_hashAction["zoom_in"]);
  menu->addAction(m_hashAction["zoom_out"]);
}



void CMainWindowPrivate::setupMenuDiagram()
{
  QMenu* menu;

  menu = m_mainWindow->menuBar()->addMenu(tr("&Diagram"));
  menu->addAction(m_hashAction["add_new_diagram"]);
  menu->addSeparator();
  menu->addAction(m_hashAction["display_diagram_list"]);
  menu->addAction(m_hashAction["display_data_list"]);
  menu->addAction(m_hashAction["display_data_dico"]);
  menu->addSeparator();

}



void CMainWindowPrivate::setupMenuWindow()
{
  QMenu* menu;

	menu = m_mainWindow->menuBar()->addMenu(tr("&Window"));
  menu->addAction(m_hashAction["display_tool_box"]);
  menu->addAction(m_hashAction["display_property_box"]);
}

void CMainWindowPrivate::setupMenuHelp()
{
  QMenu* menu;

  menu = m_mainWindow->menuBar()->addMenu(tr("&Help"));
  menu->addAction(m_hashAction["about_graphia"]);
}

void CMainWindowPrivate::setupToolBar()
{
  QToolBar* fileToolBar;
  fileToolBar = m_mainWindow->addToolBar(tr("&File"));
  fileToolBar->addAction(m_hashAction["new"]);
  fileToolBar->addAction(m_hashAction["open"]);
  fileToolBar->addAction(m_hashAction["save"]);
  fileToolBar->addAction(m_hashAction["save_as"]);
  fileToolBar->addSeparator();
  fileToolBar->addAction(m_hashAction["quit"]);

  QToolBar* diagram;
  diagram = m_mainWindow->addToolBar(tr("Diagram"));
  diagram->addAction(m_hashAction["add_new_diagram"]);
  diagram->addAction(m_hashAction["display_diagram_list"]);

  QToolBar* edit;
  edit = m_mainWindow->addToolBar(tr("&Edit"));
  edit->addAction(m_hashAction["undo"]);
  edit->addAction(m_hashAction["cut"]);
  edit->addAction(m_hashAction["copy"]);
  edit->addAction(m_hashAction["paste"]);

  QToolBar* zoom;
  zoom = m_mainWindow->addToolBar(tr("Zoom"));
  zoom->addAction(m_hashAction["zoom_in"]);
  zoom->addAction(m_hashAction["zoom_out"]);
}

void CMainWindowPrivate::setupToolBox()
{
  m_dockOfToolBox = new QDockWidget(tr("Tool box"), m_mainWindow);
  m_toolBox = new CToolBox(m_app->getModel());
  m_dockOfToolBox->setWidget(m_toolBox);
  m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_dockOfToolBox);
}

void CMainWindowPrivate::setupPropertyBox()
{
  m_dockOfPropertyBrowser = new QDockWidget(tr("Property"), m_mainWindow);
  m_propertyBrowser = new CPropertyBrowser();
  m_dockOfPropertyBrowser->setWidget(m_propertyBrowser);
  m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, m_dockOfPropertyBrowser);
}

void CMainWindowPrivate::setupStatusBar()
{
  m_mainWindow->statusBar()->showMessage(tr("Ready"));
}

//File Menu

void CMainWindowPrivate::onNewFile()
{
  CDocument* doc = getDoc();
  
  if (doc->needToBeSaved())
  {
    QMessageBox msgBox(QMessageBox::Warning,
                       tr("Document has been modified"),
                       tr("The document has not been modified since the last backup.\n Do you want to create a new document anyway?"), 
                       QMessageBox::Yes | QMessageBox::No);
    int result = msgBox.exec();
    if (result == QMessageBox::Yes)
    {
      createNewFile();
    }
    else
    {
      //do nothing.
    }
  }
  else
  {
    createNewFile();
  }
}

void CMainWindowPrivate::createNewFile()
{
  buildName();
    
  CDocument* doc = getDoc();
  doc->createNewDocument();  
  m_mainWindow->setWindowTitle(m_displayName);
}

void CMainWindowPrivate::readFile()
{
  CDocument* doc = getDoc();
  
  QString filename = QFileDialog::getOpenFileName( 
      NULL, 
      tr("Open Document"), 
      QDir::currentPath(), 
      tr("Graphia files (*.graphia);;All files (*)") );

  if( !filename.isNull() )
  {
    //qDebug( filename.toAscii() );
    bool result;
    result = doc->read(filename);
    if (result)
    {
      setDocumentTitle(filename);
    }
    else 
    {
      QMessageBox::warning(NULL, tr("graphia - opening error"), 
                      tr("It is not possible to open the diagram, due to an decoding error.\n"
                      "Check if file is corrupted, if not, you should probably create a bug.")
                      );
    }
  }
}


void CMainWindowPrivate::open()
{
  CDocument* doc = getDoc();
  
  if (doc->needToBeSaved())
  {
    QMessageBox msgBox(QMessageBox::Warning,
                       tr("Document has been modified"),
                       tr("The document has not been modified since the last backup\n. Do you want to open document anyway?"), 
                       QMessageBox::Yes | QMessageBox::No);
    int result = msgBox.exec();
    if (result == QMessageBox::Yes)
    {
      readFile();
    }
    else
    {
      //do nothing.
    }
  }
  else
  {
    readFile();
  }
}


void CMainWindowPrivate::addNewDiagram()
{
  CDialogNewDiagram newDiagramDialog;  
  int result;
  
  result = newDiagramDialog.exec();
  if (result == QDialog::Accepted)
  {
    //qDebug("accepted (%i, \"%s\")", newDiagramDialog.getType(), newDiagramDialog.getDiagramName().toLocal8Bit().data());
    getDoc()->addNewDiagram(newDiagramDialog.getType(), 
                            newDiagramDialog.getDiagramName(),
                            newDiagramDialog.getProcessName());
  }
  else
  {
    //qDebug("rejected");
  }
}

void CMainWindowPrivate::saveAs()
{
  CDocument* doc = getDoc();

  QString filename = QFileDialog::getSaveFileName(NULL, 
                                                  tr("Save Document"),
                                                  m_fullFileName,
                                                  tr("Graphia document (*.graphia);;All files (*)") );
  if (!filename.isNull())
  {
    //qDebug( filename.toAscii() );
    bool saveOk;
    saveOk = doc->save(filename);
    if (saveOk)
    {
      setDocumentTitle(filename);
    }
  }
}

void CMainWindowPrivate::setDocumentTitle(const QString& filename)
{
  QFileInfo fileInfo(filename);
  m_fullFileName = filename;
  m_hasUntitledName = false;
  m_displayName = fileInfo.baseName();
  m_mainWindow->setWindowTitle(m_displayName);
}


void CMainWindowPrivate::save()
{
  if (m_hasUntitledName)
  {
    saveAs();
  }
  else
  {
    CDocument* doc = getDoc();
    doc->save(m_fullFileName);
  }
}


void CMainWindowPrivate::displayListOfDiagram()
{
  CDialogListDiagram listDiagramDialog(getDoc(), m_mainWidget);

  listDiagramDialog.exec();  
}

void CMainWindowPrivate::displayListOfDataTypes()
{
  CDialogListTypeDico listDataTypeDialog(getDoc());
  listDataTypeDialog.exec();
}


void CMainWindowPrivate::displayToolBox()
{
  m_dockOfToolBox->show();
}

void CMainWindowPrivate::displayPropertyBox()
{
  m_dockOfPropertyBrowser->show();
}

void CMainWindowPrivate::displayAboutWnd()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(tr("About graphia"));
  msgBox.setIconPixmap(QIcon(":/flow_block.png").pixmap(32));
  msgBox.setText(tr("Flow diagram application "
                    "version " GRAPHIA_VERSION "<br/>"
                    "build of " __DATE__ " at "  __TIME__ "<br/>"
                    "(c) Mickael Sergent 2011-2014 under GPL v3 license<br/>"
                    "Icons used come from Aha-Soft(c): <a href=\"http://www.small-icons.com/\">http://www.small-icons.com/ </a> under Creative Commons v3.0 license<br/>"));
  msgBox.setTextFormat(Qt::RichText);
  msgBox.exec();
}

void CMainWindowPrivate::onDocumentModified( bool isModified )
{
  //qDebug("onDocumentModified");
  //CDocument* doc = getDoc();
  if (isModified == true)
  {
    m_mainWindow->setWindowTitle(m_displayName + "*");
  }
  else
  {
    m_mainWindow->setWindowTitle(m_displayName);    
  }
}


void CMainWindowPrivate::onDisplayDiagram ( CGraphicsScene* scene )
{
  if (scene != NULL)
  {
    m_hashAction["zoom_11"]->setEnabled(true);
    m_hashAction["zoom_in"]->setEnabled(true);
    m_hashAction["zoom_out"]->setEnabled(true);
  }
  else
  {
    m_hashAction["zoom_11"]->setEnabled(false);
    m_hashAction["zoom_in"]->setEnabled(false);
    m_hashAction["zoom_out"]->setEnabled(false);
  }
}

void CMainWindowPrivate::onZoomIn()
{
  m_mainWidget->zoom(CMainWidget::ZOOM_IN);
}

void CMainWindowPrivate::onZoomNormal()
{
  m_mainWidget->zoom(CMainWidget::ZOOM_11);
}

void CMainWindowPrivate::onZoomOut()
{
  m_mainWidget->zoom(CMainWidget::ZOOM_OUT);
}
