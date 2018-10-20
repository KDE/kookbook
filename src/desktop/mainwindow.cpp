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
#include <QProcess>
#include "treepane.h"
#include "listpane.h"
#include "ingredientsparserpane.h"
#include <QStatusBar>
#include <QSettings>
#include <QAbstractItemModel>

auto mkdock(const QString& title) {
    auto dock = std::make_unique<QDockWidget>(title);
    dock->setObjectName(title);
    return std::move(dock);
}

// QDesktopServices::openUrl just gives whatever is available to open it
// in the markdown case, it is quite likely to be a viewer, so just try to
// find some random editors, and if none of them are found
// go for QDesktopServices::openUrl and hope the best
static void openFile(const QString& file)
{
    QStringList exes = {"kwrite", "kate", "gedit", "notepad++", "atom", "gvim"};
    QString foundExe;
    for(const auto& exe : qAsConst(exes)) {
        foundExe = QStandardPaths::findExecutable(exe);
        if (!foundExe.isEmpty()) {
            break;
        }
    }
    if (!foundExe.isEmpty()) {
        QProcess::startDetached(foundExe, QStringList() << file);
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    }
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    m_activeDocument = std::make_unique<ActiveDocument>();
    m_scanner = std::make_unique<Scanner>();
    QDockWidget* fsdockptr = nullptr; // we need a handle to be able to tabify with it later.
    QVector<QDockWidget*> developerDocks; // want to hide them at the end.
    {
        auto mainpane = std::make_unique<MainPane>();
        m_activeDocument->registerListener(mainpane.get());
        m_mainPane = mainpane.get();
        setCentralWidget(mainpane.release());
        connect(m_mainPane, &MainPane::notifySimple, this, &MainWindow::notifyStatusBar);
    }
    {
        auto rawviewpane = std::make_unique<RawViewPane>();
        m_activeDocument->registerListener(rawviewpane.get());
        auto rawviewdock = mkdock("Raw view");
        developerDocks << rawviewdock.get();
        rawviewdock->setWidget(rawviewpane.release());
        addDockWidget(Qt::RightDockWidgetArea, rawviewdock.release());
    }
    {
        auto fspane = std::make_unique<FsPane>();
        connect(fspane.get(), &FsPane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        auto fsdock = mkdock("Recipes");
        m_fsPane = fspane.get();
        fsdock->setWidget(fspane.release());
        fsdockptr = fsdock.get();
        addDockWidget(Qt::LeftDockWidgetArea, fsdock.release());

    }
    {
        auto metadatapane = std::make_unique<MetaDataPane>();
        m_activeDocument->registerListener(metadatapane.get());
        auto metadatadock = mkdock("Metadata");
        metadatadock->setWidget(metadatapane.release());
        developerDocks << metadatadock.get();
        addDockWidget(Qt::LeftDockWidgetArea, metadatadock.release());
    }
    {
        auto ingredientsparserpane = std::make_unique<IngredientsParserPane>();
        auto ingredientsparserdock = mkdock("Ingredients line");
        ingredientsparserdock->setWidget(ingredientsparserpane.release());
        developerDocks << ingredientsparserdock.get();
        addDockWidget(Qt::RightDockWidgetArea, ingredientsparserdock.release());
    }
    {
        auto ingredientspane = std::make_unique<TreePane>();
        connect(ingredientspane.get(), &TreePane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        auto tagspane = std::make_unique<TreePane>();
        connect(tagspane.get(), &TreePane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });
        auto titlelist = std::make_unique<ListPane>();
        connect(titlelist.get(), &ListPane::fileSelected, m_activeDocument.get(), [this](const QString& path) {
            m_activeDocument->openPath(path);
        });

        connect(m_scanner.get(), &Scanner::dataUpdated, this, [rawingredientspane = ingredientspane.get(),rawtagspane = tagspane.get(),rawtitlelist = titlelist.get(),this]() {
            rawingredientspane->setModel(m_scanner->parsedIngredients());
            rawtagspane->setModel(m_scanner->parsedTags());
            auto titlelist = m_scanner->parsedTitleList();
            rawtitlelist->setModel(titlelist);
            notifyStatusBar(QString("Found %1 recipes").arg(titlelist->rowCount()));
            m_fsPane->setFileNameTitleMap(m_scanner->parsedFileNameTitleMap());
        });
        connect(this, &MainWindow::clear, this, [rawingredientspane = ingredientspane.get(), rawtagspane = tagspane.get(), rawtitlelist = titlelist.get(),this]() {
            rawingredientspane->setModel(nullptr);
            rawtagspane->setModel(nullptr);
            rawtitlelist->setModel(nullptr);
            m_fsPane->setFileNameTitleMap({});
        });

        {
            auto titlesdock = mkdock("Title");
            titlesdock->setWidget(titlelist.release());
            auto titlesdockptr = titlesdock.get();
            addDockWidget(Qt::LeftDockWidgetArea,titlesdock.release());
            tabifyDockWidget(titlesdockptr, fsdockptr);
        }
        {
            auto tagsdock = mkdock("Tags");
            tagsdock->setWidget(tagspane.release());
            auto tagsdockptr = tagsdock.get();
            addDockWidget(Qt::LeftDockWidgetArea,tagsdock.release());
            auto ingredientsdock = mkdock("Ingredients");
            auto ingredientsdockptr = ingredientsdock.get();
            ingredientsdock->setWidget(ingredientspane.release());
            addDockWidget(Qt::LeftDockWidgetArea,ingredientsdock.release());
            tabifyDockWidget(ingredientsdockptr,tagsdockptr);
        }

    }
    for(auto dock : qAsConst(developerDocks)) {
        dock->hide();
    }
    auto toolbar = std::make_unique<QToolBar>("Main Toolbar");
    toolbar->setObjectName("Main Toolbar");
    {
        auto action = toolbar->addAction(QIcon::fromTheme("document-open-folder"),"Open collection",this, &MainWindow::openFolder);
        action->setShortcut(QKeySequence(QKeySequence::Open));
    }
    {
        auto action = toolbar->addAction(QIcon::fromTheme("view-refresh"),"Reload collection", m_scanner.get(), &Scanner::doUpdate);
        action->setShortcut(QKeySequence(QKeySequence::Refresh));
    }
    toolbar->addSeparator();
    {
        auto action = toolbar->addAction(QIcon::fromTheme("document-new"), "New recipe", this, &MainWindow::newRecipe);
        action->setShortcut(QKeySequence(QKeySequence::New));
    }
    {
        auto action = toolbar->addAction(QIcon::fromTheme("edit-entry"),"Edit current recipe", this, &MainWindow::editActiveRecipe);
        action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    }
    {
        auto action = toolbar->addAction(QIcon::fromTheme("document-print"),"Print current recipe", m_mainPane, &MainPane::print);
        action->setShortcut(QKeySequence(QKeySequence::Print));
    }
    toolbar->addAction(QIcon::fromTheme("document-print-preview"),"Print preview current recipe", m_mainPane, &MainPane::printPreview);
    addToolBar(Qt::TopToolBarArea, toolbar.release());
    QSettings s;
    s.beginGroup("General");
    setCurrentFolder(s.value("location", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/recipes/").toString());
    restoreState(s.value("windowState", QByteArray()).toByteArray());
    statusBar()->show();
}

MainWindow::~MainWindow()
{
    QSettings s;
    s.beginGroup("General");
    if (QFile::exists(m_currentFolder)) {
        s.setValue("location", m_currentFolder);
    }
    s.setValue("windowState", saveState());
    s.sync();
}

void MainWindow::editActiveRecipe()
{
    auto path = m_activeDocument->currentPath();
    if (QFile::exists(path)) {
        openFile(path);
    }
}

void MainWindow::openFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this,"Open Folder",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (folder.isEmpty()) {
        notifyStatusBar("Cancelled");
        return;
    }
    setCurrentFolder(folder);

}

void MainWindow::newRecipe()
{
    QString file = QFileDialog::getSaveFileName(this, "Create Recipe", m_currentFolder, "Recipes (*.recipe.md)");
    if (!file.endsWith(".recipe.md")) {
        notifyStatusBar("Cancelled");
        return;
    }
    if (!QFile::exists(file)) {
        QFile::copy(":/docs/template.md", file);
        QFile::setPermissions(file,QFileDevice::WriteOwner | QFile::permissions(file));
    }
    m_activeDocument->openPath(file);
    openFile(file);
}

void MainWindow::notifyStatusBar(const QString& message)
{
    statusBar()->showMessage(message, 5000);
}


void MainWindow::setCurrentFolder(const QString& folder)
{
    m_currentFolder = folder;

    clear();
    m_activeDocument->openPath(QString());
    m_fsPane->setRootPath(folder);
    notifyStatusBar(QString("Parsing %1").arg(folder));
    m_scanner->setRootPath(folder);

}
