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

#include "scanner.h"
#include <QDirIterator>
#include <future>
#include <QThread>
#include <QStandardItemModel>

Scanner::Scanner(QObject* parent) : QObject(parent), m_running(false)
{
    connect(this, &Scanner::dataUpdatedInternal, this, &Scanner::dataUpdated, Qt::QueuedConnection);
}

Scanner::~Scanner()
{
}

void Scanner::doUpdate()
{
    if (m_running) {
        return;
    }
    if (!m_rootPath.isEmpty()) {
        m_running = true;
        std::async(&Scanner::parseThingsInDifferentThread,this,m_rootPath,QThread::currentThread());
    }
}

void Scanner::parseThingsInDifferentThread(const QString& path, QThread* resultThread) {
    
    QDirIterator it(path, QStringList() << "*.recipe.md", QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
    auto fstree = std::make_unique<QStandardItemModel>();
    QMap<QString, QVector<QPair<QString,QString>>> tags;
    QMap<QString, QVector<QPair<QString,QString>>> ingredients;
    QVector<QPair<QString, QString>> titles;
    
    while (it.hasNext()) {
        it.next();
        QFileInfo fi = it.fileInfo();
        QFile f(fi.absoluteFilePath());
        f.open(QIODevice::ReadOnly);
        auto parsed = RecipeParser::parseRecipe(&f);
        auto pair = qMakePair(parsed.title, fi.absoluteFilePath());
        titles.push_back(pair);
        for(const auto& it : qAsConst(parsed.tags)) {
            tags[it].push_back(pair);
        }
        for(const auto& it : qAsConst(parsed.ingredients)) {
            ingredients[it.ingredient].push_back(pair);
        }
    }
    
    auto parseding = std::make_unique<QStandardItemModel>();
    for(auto it = ingredients.constBegin(),end = ingredients.constEnd(); it!=end; it++) {
        auto line = std::make_unique<QStandardItem>(it.key());
        for( const auto& recipe : qAsConst(it.value())) {
            auto child = std::make_unique<QStandardItem>(recipe.first);
            child->setData(recipe.second);
            line->appendRow(child.release());
        }
        parseding->appendRow(line.release());
    }
    auto parsedtags = std::make_unique<QStandardItemModel>();
    for(auto it = tags.constBegin(),end = tags.constEnd(); it!=end; it++) {
        auto line = std::make_unique<QStandardItem>(it.key());
        for( const auto& recipe : qAsConst(it.value())) {
            auto child = std::make_unique<QStandardItem>(recipe.first);
            child->setData(recipe.second);
            line->appendRow(child.release());
        }
        parsedtags->appendRow(line.release());
    }
    std::sort(titles.begin(), titles.end(), [] (auto left, auto right) {
        return left.first < right.first;
    });
    auto titlelist = std::make_unique<QStandardItemModel>();
    QHash<QString,QString> titlemap;
    for(auto title : qAsConst(titles)) {
        titlemap[title.second] = title.first;
        auto line = std::make_unique<QStandardItem>(title.first);
        line->setData(title.second);
        titlelist->appendRow(line.release());
    }
    
    parseding->moveToThread(resultThread);
    parsedtags->moveToThread(resultThread);
    fstree->moveToThread(resultThread);
    titlelist->moveToThread(resultThread);
    std::unique_lock<std::mutex> lock(m_mutex);
    m_parsedIngredients = std::move(parseding);
    m_parsedTags = std::move(parsedtags);
    m_parsedFileNameTitleMap = titlemap;
    titlemap = {}; // ensure to decrease refcount
    m_titleList = std::move(titlelist);
    emit dataUpdatedInternal();
    m_running = false;
}

std::shared_ptr<QAbstractItemModel> Scanner::parsedIngredients() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_parsedIngredients;
}

std::shared_ptr<QAbstractItemModel> Scanner::parsedTags() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_parsedTags;
}

std::shared_ptr<QAbstractItemModel> Scanner::parsedTitleList() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_titleList;
}

void Scanner::setRootPath(const QString& path)
{
    m_rootPath = path;
    doUpdate();
}

QHash<QString, QString> Scanner::parsedFileNameTitleMap() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_parsedFileNameTitleMap;
}



