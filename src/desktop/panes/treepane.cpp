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
#include "treepane.h"
#include <QTreeView>
#include <QHBoxLayout>

TreePane::TreePane(QWidget* parent) : QWidget(parent){
    m_view = new QTreeView(this);
    m_view->setHeaderHidden(true);
    setLayout(new QHBoxLayout());
    layout()->addWidget(m_view);
    connect(m_view,&QTreeView::clicked, this, [this](const QModelIndex& idx) {
        QString data = idx.data(Qt::UserRole + 1).toString();
        if (!data.isEmpty()) {
            Q_EMIT this->fileSelected(data);
        }
    });
}

TreePane::~TreePane() {
    //
}

void TreePane::setModel(const std::shared_ptr<QAbstractItemModel>& model)
{
    m_model = model;
    m_view->setModel(model.get());
}



