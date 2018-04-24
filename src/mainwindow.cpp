/*
 * Copyright (c) 2018 Sune Vuorela <sune@vuorela.dk>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mainwindow.h"
#include <memory>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QToolBar>
#include <QDesktopServices>
#include "fspane.h"
#include "mainpane.h"
#include "metadatapane.h"
#include "rawviewpane.h"
#include "activedocument.h"
#include "scanner.h"
#include <QFileDialog>
#include "treepane.h"
#include "listpane.h"

auto mkdock(const QString& title) { return std::make_unique<QDockWidget>(title);}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    m_activeDocument = std::make_unique<ActiveDocument>();
    m_scanner = std::make_unique<Scanner>();
    {
        auto mainpane = std::make_unique<MainPane>();
        m_activeDocument->registerListener(mainpane.get());
    
        setCentralWidget(mainpane.release());
    }
    {
        auto rawviewpane = std::make_unique<RawViewPane>();
        m_activeDocument->registerListener(rawviewpane.get());
        auto rawviewdock = mkdock("Raw view");
        rawviewdock->setWidget(rawviewpane.release());
        addDockWidget(Qt::RightDockWidgetArea, rawviewdock.release());
    }
    {
        auto fspane = std::make_unique<FsPane>();
        connect(fspane.get(), &FsPane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        fspane->setRootPath("/home/sune/recipes/");
        m_scanner->setRootPath("/home/sune/recipes/");
        auto fsdock = mkdock("Recipes");
        m_fsPane = fspane.get();
        fsdock->setWidget(fspane.release());
        addDockWidget(Qt::LeftDockWidgetArea, fsdock.release());
        
    }
    {
        auto metadatapane = std::make_unique<MetaDataPane>();
        m_activeDocument->registerListener(metadatapane.get());
        auto metadatadock = mkdock("Metadata");
        metadatadock->setWidget(metadatapane.release());
        addDockWidget(Qt::LeftDockWidgetArea, metadatadock.release());
    }
    {
        auto ingredientspane = std::make_unique<TreePane>();
        TreePane* rawingredientspane = ingredientspane.get();
        connect(ingredientspane.get(), &TreePane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        auto tagspane = std::make_unique<TreePane>();
        TreePane* rawtagspane = tagspane.get();
        connect(tagspane.get(), &TreePane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        auto titlelist = std::make_unique<ListPane>();
        ListPane* rawtitlelist = titlelist.get();
        connect(titlelist.get(), &ListPane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        
        connect(m_scanner.get(), &Scanner::dataUpdated, this, [rawingredientspane,rawtagspane,rawtitlelist,this]() {
            rawingredientspane->setModel(m_scanner->parsedIngredients());
            rawtagspane->setModel(m_scanner->parsedTags());
            rawtitlelist->setModel(m_scanner->parsedTitleList());
            m_fsPane->setFileNameTitleMap(m_scanner->parsedFileNameTitleMap());
        });
        connect(this, &MainWindow::clear, this, [rawingredientspane,rawtagspane, rawtitlelist,this]() {
            rawingredientspane->setModel(nullptr);
            rawtagspane->setModel(nullptr);
            rawtitlelist->setModel(nullptr);
            m_fsPane->setFileNameTitleMap({});
        });
        
        {
            auto tagsdock = mkdock("Tags");
            tagsdock->setWidget(tagspane.release());
            addDockWidget(Qt::BottomDockWidgetArea,tagsdock.release());
        }
        
        {
            auto ingredientsdock = mkdock("Ingredients");
            ingredientsdock->setWidget(ingredientspane.release());
            addDockWidget(Qt::BottomDockWidgetArea,ingredientsdock.release());
        }
        
        {
            auto titlesdock = mkdock("Title");
            titlesdock->setWidget(titlelist.release());
            addDockWidget(Qt::BottomDockWidgetArea,titlesdock.release());
        }
    }
    auto toolbar = std::make_unique<QToolBar>("Main Toolbar");
    toolbar->addAction(QIcon::fromTheme("document-open-folder"),"open",this, &MainWindow::openFolder);
    toolbar->addAction(QIcon::fromTheme("edit-entry"),"edit current", this, &MainWindow::editActiveRecipe);
    toolbar->addAction(QIcon::fromTheme("view-refresh"),"reload", m_scanner.get(), &Scanner::doUpdate);
    addToolBar(Qt::TopToolBarArea, toolbar.release());
}

MainWindow::~MainWindow()
{
    //for smart pointers
}

void MainWindow::editActiveRecipe()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_activeDocument->currentPath()));
}

void MainWindow::openFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this,"Open Folder",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (folder.isEmpty()) {
        return;
    }
    clear();
    
    m_activeDocument->openPath(QString());
    m_fsPane->setRootPath(folder);
    m_scanner->setRootPath(folder);
}

