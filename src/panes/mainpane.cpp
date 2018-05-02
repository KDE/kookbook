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
#include "mainpane.h"
#include <QHBoxLayout>
#include <memory>
#include <QFile>
#include <QTextEdit>
#include <QTextDocument>

extern "C"
{
#include <mkdio.h>
}

MainPane::MainPane(QWidget* parent) : PaneBase(parent)
{
    m_document = std::make_unique<QTextDocument>();
    auto layout = std::make_unique<QHBoxLayout>();
    auto textView = std::make_unique<QTextEdit>();
    textView->setReadOnly(true);
    textView->setDocument(m_document.get());
    
    layout->addWidget(textView.release());
    setLayout(layout.release());
    openPath(QString());
}

void MainPane::openPath(const QString& path)
{
    QByteArray data;
    if(QFile::exists(path)) {
        QFile f(path);
        bool success = f.open(QIODevice::ReadOnly);
        if(!success) {
            return;
        }
        data = f.readAll();
    } else {
        QFile f(":/docs/intro.md");
        f.open(QIODevice::ReadOnly);
        data = f.readAll();
    }

    MMIOT* handle = mkd_string(data,data.size(), 0);

    int success = mkd_compile(handle, 0);
    if (!success ) {
        return;
    }

    char* html;
    int length = mkd_document(handle, &html);

    m_document->setHtml(QString::fromUtf8(html,length));

    mkd_cleanup(handle);

}

MainPane::~MainPane()
{
    // for smart pointers
}
