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
#include <mutex>
#include "recipeparser.h"
#include <memory>
#include <QHash>
#include <future>

class QAbstractItemModel;
/**
 * File system scanner to index the recipes in the recipe collection.
 *
 * As a implementation detail, a worker thread is used for
 * the file system access and parsing and stuff.
 */
class Scanner : public QObject
{
    Q_OBJECT
public:
    Scanner(QObject* parent = nullptr);
    /**
     * \return tree model of parsed tags
     */
    std::shared_ptr<QAbstractItemModel> parsedTags() const;
    /**
     * \return tree model of parsed ingredients
     */
    std::shared_ptr<QAbstractItemModel> parsedIngredients() const;
    /**
     * \return list model of parsed title
     */
    std::shared_ptr<QAbstractItemModel> parsedTitleList() const;
    /**
     * \return map from file path to recipe title
     */
    QHash<QString,QString> parsedFileNameTitleMap() const;
    /**
     * Sets the (new) root path and triggers a re-indexing of everything.
     */
    void setRootPath(const QString& path);
    ~Scanner();
public Q_SLOTS:
    /**
     * Re-indexes with current root path
     */
    void doUpdate();
Q_SIGNALS:
    /**
     * Emitted update is done (and new models and maps and such can be read)
     */
    void dataUpdated();
    /**
     * \internal - thread switching for dataUpdated.
     */
    void dataUpdatedInternal(QPrivateSignal p = QPrivateSignal());
private:
    mutable std::mutex m_mutex;
    std::shared_ptr<QAbstractItemModel> m_parsedTags;
    std::shared_ptr<QAbstractItemModel> m_parsedIngredients;
    std::shared_ptr<QAbstractItemModel> m_titleList;
    QHash<QString,QString> m_parsedFileNameTitleMap;
    /**
     * Function to be run in a different thread.
     * \param path Root path to start indexing from
     * \param targetThread The QThread to move created QObjects to (main thread) before switching back.
     */
    void parseThingsInDifferentThread(const QString& path, QThread* targetThread);
    QString m_rootPath;
    std::atomic_bool m_running;
    std::future<void> m_future;
};
