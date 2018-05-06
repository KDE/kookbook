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

#include "ingredientsextractor.h"
#include <QTest>

class IngredientsExtractorTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testThings();
    void testThings_data();
};

Q_DECLARE_METATYPE(IngredientsExtractor::Ingredient);


void IngredientsExtractorTest::testThings()
{
    QFETCH(QString, input);
    QFETCH(IngredientsExtractor::Ingredient, result);

    auto parsedResult = IngredientsExtractor::parseLine(input);
    QCOMPARE(parsedResult.amount,result.amount);
    QCOMPARE(parsedResult.unit,result.unit);
    QCOMPARE(parsedResult.ingredient,result.ingredient);

}


void IngredientsExtractorTest::testThings_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<IngredientsExtractor::Ingredient>("result");

    QTest::newRow("simple") << " * 1.5 kg mel" << IngredientsExtractor::Ingredient{"1.5","kg","mel"};
    QTest::newRow("simple almost") << "* 1.5 kg mel" << IngredientsExtractor::Ingredient{"1.5","kg","mel"};
    QTest::newRow("simple extra spaces") << "* 1.5  kg   mel" << IngredientsExtractor::Ingredient{"1.5","kg","mel"};
    QTest::newRow("half baked") << "* 1.5 kg " << IngredientsExtractor::Ingredient{};
    QTest::newRow("with comment") << " * 1.5 kg mel, alternativt grahamsmel" << IngredientsExtractor::Ingredient{"1.5","kg","mel"};
    QTest::newRow("with comment, no parsable ingredient") << " * mel, alternativt grahamsmel" << IngredientsExtractor::Ingredient{};
    QTest::newRow("just comment") << " , foo" << IngredientsExtractor::Ingredient{};
    QTest::newRow("empty") << QString() << IngredientsExtractor::Ingredient{};
    QTest::newRow("almost empty") << " * " << IngredientsExtractor::Ingredient{};
}



QTEST_APPLESS_MAIN(IngredientsExtractorTest)

#include "ingredientsextractortest.moc"
