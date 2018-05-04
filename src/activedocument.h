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
#pragma once
#include <QObject>
#include <QVector>
#include <memory>

class QFileSystemWatcher;
class ActiveDocumentListener;

/**
 * Broker for handling opening a document and
 * informing components interested in the active document.
 *
 * Does also cause a reload whenever the file changes.
 */
class ActiveDocument : public QObject
{
    Q_OBJECT
public:
    ActiveDocument(QObject* parent = nullptr);
    ~ActiveDocument();
    void registerListener(ActiveDocumentListener* pane);
    QString currentPath() const;

public Q_SLOTS:
    void openPath(const QString& file);
private:
    QString m_currentPath;
    QVector<ActiveDocumentListener*> m_panes;
    void reload();
    std::unique_ptr<QFileSystemWatcher> m_watcher;
private Q_SLOTS:
    void checkedReload(const QString& path); // needs to check if path and currentPath is the same
};
