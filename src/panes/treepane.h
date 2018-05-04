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

class QTreeView;
class QAbstractItemModel;
/**
 * Generic pane wrapping a QTreeView
 */
class TreePane : public QWidget
{
    Q_OBJECT
public:
    TreePane(QWidget* parent = nullptr);
    /**
     * Sets the model to the given object.
     * \param model The model
     */
    void setModel(const std::shared_ptr<QAbstractItemModel>& model);
    ~TreePane();
Q_SIGNALS:
    /**
     * Emitted when a file is selected in this pane.
     *
     * \param path The selected file
     */
    void fileSelected(const QString& path);
private:
    std::shared_ptr<QAbstractItemModel> m_model;
    QTreeView* m_view;
};
