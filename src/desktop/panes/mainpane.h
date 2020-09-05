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
#include "panebase.h"
#include <memory>

class QTextDocument;
class QTextBrowser;

/**
 * The main view for rendering the recipes.
 */
class MainPane : public PaneBase
{
    Q_OBJECT
public:
    MainPane(QWidget* parent = nullptr) ;
    ~MainPane();
Q_SIGNALS:
    /**
     * Requests a simple notification
     * \param msg The message to notify
     */
    void notifySimple(const QString& msg);
public Q_SLOTS:
    void openPath(const QString& path) override;
    /**
     * Starts printing the viewed document
     */
    void print();
    /**
     * Starts print preview (and printing)
     */
    void printPreview();
private:
    std::unique_ptr<QTextDocument> m_document;
    QTextBrowser *m_textView;
};
