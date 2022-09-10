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

#include "scanneradapter.h"
#include "scanner.h"
#include <QSortFilterProxyModel>

#include <QFile>
#include <QSettings>
#include <QStandardPaths>

ScannerAdapter::ScannerAdapter(QObject* parent) : m_scanner(std::make_unique<Scanner>()), m_titleList(std::make_unique<QSortFilterProxyModel>())
{
    m_titleList->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(m_scanner.get(), &Scanner::dataUpdated, this, [&]() {
        auto titleList = m_scanner->parsedTitleList();
        m_titleList->setSourceModel(titleList.get());
        m_titleListSource = titleList;
    });
    QSettings s;
    s.beginGroup("General");
    m_scanner->setRootPath(s.value("location", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/recipes/").toString());
}

ScannerAdapter::~ScannerAdapter()
{
}

QObject * ScannerAdapter::titleList() const {
    return m_titleList.get();
}

void ScannerAdapter::setFilter(const QString& filter)
{
    if (filter != m_filterFixedString) {
        m_titleList->setFilterFixedString(filter);
        m_filterFixedString = filter;
        Q_EMIT filterChanged();
    }
}

void ScannerAdapter::setRootPath(const QString& rootPath)
{
    if (m_scanner->rootPath() != rootPath)
    {
        m_scanner->setRootPath(rootPath);
        Q_EMIT rootPathChanged();
        QSettings s;
        s.beginGroup("General");
        if (QFile::exists(rootPath)) {
            s.setValue("location", rootPath);
        }
    }
}

QString ScannerAdapter::filter() const
{
    return m_filterFixedString;
}

QString ScannerAdapter::rootPath() const
{
    return m_scanner->rootPath();
}

void ScannerAdapter::refresh()
{
    m_scanner->doUpdate();
}
