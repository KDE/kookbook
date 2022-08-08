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
#include <QIODevice>
#include <QDebug>

enum class State {
    Unknown,
    ParsingMetaData,
    ParsingIngredients
};

RecipeParser::ParsedRecipe RecipeParser::parseRecipe(QIODevice* input)
{
    if (!input) {
        qDebug() << "no input";
        return {};
    }
    if (!input->isReadable()) {
        return {};
    }

    RecipeParser::ParsedRecipe result;
    auto state = State::Unknown;
    while (!input->atEnd()) {
        QByteArray line = input->readLine(2000);
        if (line.startsWith("# ")) {
            result.title = QString::fromUtf8(line.right(line.length()-2)).trimmed();
            continue;
        }
        if (line.startsWith("### Ingred")) {
            state = State::ParsingIngredients;
            continue;
        }
        if (line.startsWith("### Direc")) {
            state = State::Unknown;
            continue;
        }
        if (line.startsWith("### Meta")) {
            state = State::ParsingMetaData;
            continue;
        }
        switch(state)
        {
            case State::ParsingIngredients:
            {
                auto ingredient = IngredientsExtractor::parseLine(QString::fromUtf8(line));
                if (!ingredient.unit.isEmpty() && !ingredient.ingredient.isEmpty()) {
                    result.ingredients.push_back(std::move(ingredient));
                }
                continue;
            }
            case State::ParsingMetaData:
            {
                int sep = line.indexOf(':');
                if (sep != -1) {
                    QString key = QString::fromUtf8(line.left(sep)).trimmed();
                    QString value = QString::fromUtf8(line.mid(sep+1)).trimmed();
                    if (!value.isEmpty())
                    {
                        if (key == QLatin1String("tags")) {
                            QStringList tags = value.split(',',Qt::SkipEmptyParts);
                            for(QString tag : qAsConst(tags)) {
                                result.tags.push_back(tag.trimmed());
                            }
                        } else {
                            result.otherMeta[key].push_back(value);
                        }
                    }
                }
                continue;
            }
            case State::Unknown:
                continue;
        }

    }

    return result;
}
