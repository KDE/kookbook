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
#include "listpane.h"
#include <QListView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <memory>
#include <QSortFilterProxyModel>
#include <QAction>

ListPane::ListPane(QWidget* parent) : QWidget(parent){
    m_view = new QListView(this);
    m_filter = std::make_unique<QSortFilterProxyModel>();
    m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    setLayout(new QVBoxLayout());

    auto edit = std::make_unique<QLineEdit>();
    edit->setPlaceholderText("Search");
    auto action = edit->addAction(QIcon::fromTheme("edit-clear"),QLineEdit::TrailingPosition);
    connect(action, &QAction::triggered, edit.get(), &QLineEdit::clear);
    connect(edit.get(), &QLineEdit::textChanged, this, &ListPane::setFilterString);

    layout()->addWidget(edit.release());
    layout()->addWidget(m_view);
    connect(m_view,&QListView::clicked, this, [this](const QModelIndex& idx) {
        QString data = idx.data(Qt::UserRole + 1).toString();
        if (!data.isEmpty()) {
            emit this->fileSelected(data);
        }
    });
    m_view->setModel(m_filter.get());
}

ListPane::~ListPane()
{
    // for smart pointers
}


void ListPane::setModel(const std::shared_ptr<QAbstractItemModel>& model)
{
    m_model = model;
    m_filter->setSourceModel(model.get());
}

void ListPane::setFilterString(const QString& filter) {
    m_filter->setFilterFixedString(filter);
}
