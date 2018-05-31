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
#include "fspane.h"
#include <memory>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QIdentityProxyModel>

#include <QDebug>

class OneColumnProxy : public QIdentityProxyModel
{
public:
    OneColumnProxy(QObject* parent = nullptr) : QIdentityProxyModel(parent) {}
    int columnCount(const QModelIndex& ) const override {
        return 1;
    }
};

class FileNameTitleMapper : public QIdentityProxyModel
{
public:
    FileNameTitleMapper(QObject* parent = nullptr) : QIdentityProxyModel(parent) {}
    void setFileNameTitleMap(QHash<QString,QString> map) {
        beginResetModel();
        m_map = map;
        endResetModel();
    }
    QHash<QString,QString> m_map;
    QVariant data(const QModelIndex& idx, int role) const override {
        if(role == Qt::DisplayRole) {
            QVariant d = QIdentityProxyModel::data(idx,QFileSystemModel::FilePathRole);
            QString str = d.toString();
            auto it = m_map.constFind(str);
            if (it != m_map.constEnd()) {
                return it.value();
            }
        }
        return QIdentityProxyModel::data(idx,role);
    }
};

FsPane::FsPane(QWidget* parent) : QWidget(parent)
{
    auto layout = std::make_unique<QVBoxLayout>();
    
    auto tree = std::make_unique<QTreeView>();
    m_dirModel = std::make_unique<QFileSystemModel>();
    m_dirModel->setNameFilters(QStringList() << QStringLiteral("*.recipe.md"));
    m_dirModel->setNameFilterDisables(false);
    
    m_filenamemapper = std::make_unique<FileNameTitleMapper>();
    m_filenamemapper->setSourceModel(m_dirModel.get());
    
    m_proxy = std::make_unique<OneColumnProxy>();
    m_proxy->setSourceModel(m_filenamemapper.get());
    
    
    tree->setModel(m_proxy.get());
    tree->setHeaderHidden(true);
    
    connect(tree.get(), &QTreeView::clicked, [this] (const QModelIndex& idx)
    {
        auto var = idx.data(QFileSystemModel::FilePathRole);
        emit this->fileSelected(var.toString());
    });

    m_tree = tree.get();
    layout->addWidget(tree.release());
    
    setLayout(layout.release());
}

FsPane::~FsPane()
{
    // for smart pointers
}

void FsPane::setRootPath(const QString& string)
{
    auto path = m_dirModel->setRootPath(string);

    path = m_filenamemapper->mapFromSource(path);
    path = m_proxy->mapFromSource(path);
    if (path.isValid()) {
        m_tree->setModel(m_proxy.get());
        m_tree->setRootIndex(path);
        m_rootPath = string;
    } else {
        m_tree->setModel(nullptr);
        m_rootPath = QString();
    }
}

void FsPane::setFileNameTitleMap(QHash<QString, QString> titlemap)
{
    m_filenamemapper->setFileNameTitleMap(titlemap);
    setRootPath(m_rootPath);
}
