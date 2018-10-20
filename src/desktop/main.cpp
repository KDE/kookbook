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
#include "mainwindow.h"
#include <QApplication>
#include <QResource>
#include <QFile>
#include <QStandardPaths>


int main(int argc, char** argv)
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts,true); // we load QtWE
    QApplication app(argc, argv);

    QString themeFilePath = QStandardPaths::locate(QStandardPaths::AppDataLocation, QLatin1String("icons/breeze/breeze-icons.rcc"));
    if (QFile::exists(themeFilePath)) {
        bool ok = QResource::registerResource(themeFilePath, "/icons/breeze");
        if (ok) {
            QIcon::setThemeName("breeze");
        }
    }

    app.setWindowIcon(QIcon(":/appicon.svg"));
    app.setApplicationDisplayName("Kookbook");
    app.setOrganizationName("kde.org");
    app.setOrganizationDomain("kde.org");
    app.setApplicationName("Kookbook");

    MainWindow mw;

    mw.show();
    return app.exec();

}
