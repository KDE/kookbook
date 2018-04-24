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
#include "activedocument.h"
#include <QFileSystemWatcher>
#include "panebase.h"
#include <QDebug>

ActiveDocument::ActiveDocument(QObject* parent)
{
    m_watcher = std::make_unique<QFileSystemWatcher>();
    connect(m_watcher.get(), &QFileSystemWatcher::fileChanged, this, &ActiveDocument::checkedReload);
}

ActiveDocument::~ActiveDocument()
{
    // for smart pointers
}

void ActiveDocument::registerListener(ActiveDocumentListener* pane)
{
    m_panes.append(pane);
}

void ActiveDocument::openPath(const QString& file)
{
    if (!m_currentPath.isEmpty()) {
        m_watcher->removePath(m_currentPath);
    }
        
    m_currentPath = file;
    
    if (!m_currentPath.isEmpty()) {
        m_watcher->addPath(m_currentPath);
    }
    reload();
}

void ActiveDocument::reload()
{
    for(auto listener : qAsConst(m_panes)) {
        listener->openPath(m_currentPath);
    }
}

void ActiveDocument::checkedReload(const QString& path) 
{
    if (path == m_currentPath) {
        reload();
    }
}

QString ActiveDocument::currentPath() const
{
    return m_currentPath;
}




