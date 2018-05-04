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
#include <QWidget>
#include <memory>

class QListView;
class QAbstractItemModel;
class QSortFilterProxyModel;

/**
 * General pane for filtered lists. Technically wraps a QListView
 */
class ListPane : public QWidget
{
    Q_OBJECT
public:
    ListPane(QWidget* parent = nullptr);
    /**
     * Sets the model for the list view
     * \param model The model
     */
    void setModel(const std::shared_ptr<QAbstractItemModel>& model);
    ~ListPane();
Q_SIGNALS:
    /**
     * Emitted when a file is selected
     * \param path The selected path
     */
    void fileSelected(const QString& path);
private Q_SLOTS:
    /**
     * The string to filter on
     * \param filter The filter string
     */
    void setFilterString(const QString& filter);
private:
    std::shared_ptr<QAbstractItemModel> m_model;
    std::unique_ptr<QSortFilterProxyModel> m_filter;
    QListView* m_view;
};
