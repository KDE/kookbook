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
#include <memory>

class QAbstractItemModel;
class QSortFilterProxyModel;

class Scanner;

/** Adapts the kookbook scanner to QtQuick
 */
class ScannerAdapter : public QObject
{
    Q_OBJECT

public:
    /** Model containing the title list */
    Q_PROPERTY(QObject* titleList READ titleList NOTIFY titleListChanged)
    /** Filter property for title list */
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    /** Cookbook root path*/
    Q_PROPERTY(QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)
    ScannerAdapter(QObject* parent = nullptr);
    ~ScannerAdapter();
    QObject* titleList() const;
    QString filter() const;
    void setFilter(const QString& filter);
    QString rootPath() const;
    void setRootPath(const QString& rootPath);
    /** Re-parses recipe collection in current path*/
    Q_INVOKABLE void refresh();
Q_SIGNALS:
    void titleListChanged();
    void filterChanged();
    void rootPathChanged();
private:
    std::unique_ptr<Scanner> m_scanner;
    std::shared_ptr<QAbstractItemModel> m_titleListSource;
    QString m_filterFixedString;
    std::unique_ptr<QSortFilterProxyModel> m_titleList;

};
