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
#include <QAbstractListModel>
#include <QDir>
#include <memory>
#include <QVector>
#include <QFileInfo>

class QSortFilterProxyModel;
class DirModelModel;
/**
 * Backend class for FileOpen dialog. Provides a filterable model of current directory
 * and ability to change current directory
 */
class DirModel : public QObject
{
    Q_OBJECT

public:
    enum Mode {
        Folders = QDir::Dirs,
        Files = QDir::Files,
        Drives = QDir::Drives,
        AllEntries = QDir::AllEntries
    };
    Q_ENUM(Mode)
    DirModel(QObject* parent = nullptr);
    ~DirModel();
    /** The potentially filtered model */
    Q_PROPERTY(QObject* filteredModel READ filteredModel CONSTANT)
    /** Filter string for the filtered model */
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    /** Current path */
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    /** The current mode (available files types)*/
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    // TODO Q_PROPERTY(QStringList filterList ....) to be able to match e.g. *.pdf or whatever
    /** changes directory one level up */
    Q_INVOKABLE void cdUp();
    /** changes to directory */
    Q_INVOKABLE void cd(const QString& directory);
    QObject* filteredModel() const;
    QString filter() const;
    void setFilter(const QString& newFilter);
    QString path() const;
    void setPath(const QString& newPath);
    Mode mode() const;
    void setMode(Mode mode);

Q_SIGNALS:
    void filterChanged();
    void pathChanged();
    void modeChanged();
private:
    std::unique_ptr<QSortFilterProxyModel> m_filterModel;
    std::unique_ptr<DirModelModel> m_sourceModel;
    QString m_filter;
};

/**
 * QAIM backend for DirModel
 * When using a filtered model in \ref DirModel, the following roles are available
 *  "display" - file name
 *  "icon" - xdg spec'ed icon for current file if applicaple
 *  "fullpath" - full path to file
 *  "type" - "file" or "folder"
 */
class DirModelModel : public QAbstractListModel {
    Q_OBJECT
public:
    DirModelModel(QObject* parent = nullptr);
    ~DirModelModel();
    void cdUp();
    void cd(const QString& directory);
    void setPath(const QString& newPath);
    QString path() const;
    // QAIM api
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & parent) const override;
    enum MyRoles {
        FullPath = Qt::UserRole + 1,
        Type
    };
    void setMode(DirModel::Mode newMode);
    DirModel::Mode mode() const;


private:
    void refresh();
    QDir m_path;
    DirModel::Mode m_mode;
    QVector<QFileInfo> m_files;
};
