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

#include "recipeparser.h"
#include <QTest>
#include <QBuffer>
#include <memory>

class RecipeParserTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testParseRecipe();
    void testParseRecipe_data();
};

Q_DECLARE_METATYPE(RecipeParser::ParsedRecipe)

void RecipeParserTest::testParseRecipe()
{
    QFETCH(QByteArray, input);
    QFETCH(bool, inputIsFile);
    QFETCH(RecipeParser::ParsedRecipe, result);

    std::unique_ptr<QIODevice> inputDevice;
    if (inputIsFile) {
        QString filename = QFINDTESTDATA("data/"+QString::fromUtf8(input));
        inputDevice.reset(new QFile(filename));
        inputDevice->open(QIODevice::ReadOnly);
    } else {
        inputDevice.reset(new QBuffer(&input));
        inputDevice->open(QIODevice::ReadOnly);
    }

    auto parsedResult = RecipeParser::parseRecipe(inputDevice.get());

    QCOMPARE(parsedResult.title, result.title);
    QCOMPARE(parsedResult.tags.size(), result.tags.size());
    for(int i = 0 ; i < parsedResult.tags.size() ; i++) {
        QCOMPARE(parsedResult.tags.at(i), result.tags.at(i));
    }
    QCOMPARE(parsedResult.ingredients.size(), result.ingredients.size());
    for(int i = 0 ; i < parsedResult.ingredients.size() ; i++) {
        QCOMPARE(parsedResult.ingredients.at(i).amount, result.ingredients.at(i).amount);
        QCOMPARE(parsedResult.ingredients.at(i).unit, result.ingredients.at(i).unit);
        QCOMPARE(parsedResult.ingredients.at(i).ingredient, result.ingredients.at(i).ingredient);
    }
    QCOMPARE(parsedResult.otherMeta.size(), result.otherMeta.size());
    QCOMPARE(parsedResult.otherMeta, result.otherMeta);
}


void RecipeParserTest::testParseRecipe_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<bool>("inputIsFile");
    QTest::addColumn<RecipeParser::ParsedRecipe>("result");

    QTest::newRow("empty") << QByteArray() << false << RecipeParser::ParsedRecipe{};
    QTest::newRow("justtitle") << QByteArray("# This is title\n") << false << RecipeParser::ParsedRecipe{"This is title",{},{},{}};
    QTest::newRow("justags") << QByteArray("# Title\n### Ingredienst\n### Directio \n### Metadata\ntags: horse, cow, sheep") << false << RecipeParser::ParsedRecipe{"Title",{},{"horse","cow","sheep"},{}};
    QTest::newRow("emptytags") << QByteArray("# Title\n### Ingredienst\n### Directio \n### Metadata\ntags: ") << false << RecipeParser::ParsedRecipe{"Title",{},{}, {}};
    QTest::newRow("twoauthors") << QByteArray("# Title\n### Ingredienst\n### Directio \n### Metadata\nauthor: horse\nauthor:cow") << false << RecipeParser::ParsedRecipe{"Title",{},{},{{"author",{"horse","cow"}}}};
    QTest::newRow("emptyauthors") << QByteArray("# Title\n### Ingredienst\n### Directio \n### Metadata\nauthor: \nauthor:") << false << RecipeParser::ParsedRecipe{"Title",{},{},{}};
    QTest::newRow("sometext") << QByteArray("# Title\n### Ingredienst\n\n\n * 1 kg mel\n * 3 l mælk\nunparsablestuff\n### Directio \n do stuff") << false << RecipeParser::ParsedRecipe{"Title",{{"1","kg","mel"},{"3","l","mælk"}},{},{}};
    QTest::newRow("couscousfile") << QByteArray("couscous.recipe.md") << true << RecipeParser::ParsedRecipe{"CousCous",{{"3","dl","kraftig boullion"},{"1.5","dl","cous cous"},{"1","spsk","vineddike"}, {"1","spsk","olivenolie"}},{},{{"author",{"Sune Vuorela <sune@vuorela.dk>"}}}};
    QTest::newRow("two ingredients sections") << QByteArray("twoingredients.recipe.md") << true << RecipeParser::ParsedRecipe{"CousCous",{{"3","dl","kraftig boullion"},{"1.5","dl","cous cous"},{"1","spsk","vineddike"}, {"1","spsk","olivenolie"}},{},{{"author",{"Sune Vuorela <sune@vuorela.dk>"}}}};

}

QTEST_APPLESS_MAIN(RecipeParserTest)

#include "recipeparsertest.moc"
