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

#include "dirmodel.h"
#include <QSortFilterProxyModel>
#include <QDirIterator>

DirModel::DirModel(QObject* parent) : QObject(parent), m_filterModel{std::make_unique<QSortFilterProxyModel>()}, m_sourceModel{std::make_unique<DirModelModel>()}{
    m_filterModel->setSourceModel(m_sourceModel.get());
}

void DirModel::setPath(const QString& newPath)
{
    QString oldPath = m_sourceModel->path();
    if (oldPath != newPath) {
        m_sourceModel->setPath(newPath);
        Q_EMIT pathChanged();
    }
}

QString DirModel::filter() const
{
    return m_filter;
}

QString DirModel::dirName() const
{
    return m_sourceModel->dirName();
}


QObject* DirModel::filteredModel() const
{
    return m_filterModel.get();
}

QString DirModel::path() const
{
    return m_sourceModel->path();
}

void DirModel::setFilter(const QString& newFilter)
{
    if (m_filter != newFilter) {
        m_filter = newFilter;
        m_filterModel->setFilterFixedString(newFilter);
        Q_EMIT filterChanged();
    }
}

void DirModel::cd(const QString& path)
{
    m_sourceModel->cd(path);
    Q_EMIT pathChanged();
}

void DirModel::cdUp()
{
    m_sourceModel->cdUp();
    Q_EMIT pathChanged();
}

DirModel::Mode DirModel::mode() const
{
    return m_sourceModel->mode();
}

void DirModel::setMode(DirModel::Mode mode)
{
    if (mode != m_sourceModel->mode()) {
        m_sourceModel->setMode(mode);
        Q_EMIT modeChanged();
    }

}




DirModel::~DirModel()
{
}

DirModelModel::DirModelModel(QObject* parent) : QAbstractListModel(parent), m_mode(DirModel::AllEntries)
{
}

DirModelModel::~DirModelModel()
{
}


void DirModelModel::cd(const QString& directory)
{
    beginResetModel();
    m_path.cd(directory);
    refresh();
    endResetModel();
}

void DirModelModel::cdUp()
{
    beginResetModel();
    m_path.cdUp();
    refresh();
    endResetModel();
}

void DirModelModel::setPath(const QString& newPath)
{
    if (m_path.currentPath() != newPath) {
        beginResetModel();
        m_path.setPath(newPath);
        refresh();
        endResetModel();
    }
}

QString DirModelModel::path() const
{
    return m_path.currentPath();
}

QVariant DirModelModel::data(const QModelIndex& index, int role) const
{
    switch(role) {
        case Qt::DisplayRole: {
            return m_files[index.row()].fileName();
        }
        case Qt::DecorationRole: {
            if(m_files[index.row()].isFile()) {
                return {};
            }
            if(m_files[index.row()].isDir()) {
                return "folder";
            }
            return QVariant{};
        }
        case FullPath: {
            return m_files[index.row()].absoluteFilePath();
        }
        case Type: {
            if(m_files[index.row()].isFile()) {
                return "file";
            }
            if(m_files[index.row()].isDir()) {
                return "folder";
            }
            return QVariant{};
        }

    }
    return QVariant{};
}

int DirModelModel::rowCount(const QModelIndex& parent) const
{
    return m_files.count();
}

QHash<int, QByteArray> DirModelModel::roleNames() const
{
    QHash<int, QByteArray> rv;
    rv[Qt::DisplayRole] = "display";
    rv[Qt::DecorationRole] = "icon";
    rv[FullPath] = "fullpath";
    rv[Type] = "type";
    return rv;
}

void DirModelModel::refresh()
{
    m_files.clear();
    m_files = m_path.entryInfoList(QDir::NoDotAndDotDot | static_cast<QDir::Filters>(m_mode), QDir::Name | QDir::DirsFirst).toVector();
}

DirModel::Mode DirModelModel::mode() const
{
    return m_mode;
}

void DirModelModel::setMode(DirModel::Mode newMode)
{
    if (newMode != m_mode) {
        beginResetModel();
        m_mode = newMode;
        refresh();
        endResetModel();
    }
}

QString DirModelModel::dirName() const
{
    return m_path.dirName();
}
