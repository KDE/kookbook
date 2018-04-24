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
#include "rawviewpane.h"
#include <memory>
#include <QFile>
#include <QTextBrowser>
#include <QHBoxLayout>

RawViewPane::RawViewPane(QWidget* parent) : PaneBase(parent)
{
    auto rawView = std::make_unique<QTextBrowser>();
    auto layout = std::make_unique<QHBoxLayout>();
    m_rawView = rawView.get();
    layout->addWidget(rawView.release());
    setLayout(layout.release());
}


void RawViewPane::openPath(const QString& path)
{
    QFile f(path);
    bool isOpen = f.open(QIODevice::ReadOnly);
    if (isOpen) {
        QString content = QString::fromUtf8(f.readAll());
        m_rawView->setPlainText(content);
    }
}
