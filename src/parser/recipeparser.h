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
#include "ingredientsextractor.h"
#include <QVector>
#include <QMap>

class QIODevice;

/**
 * Extract information from a recipe. See format documentation for details.
 */
namespace RecipeParser
{
    /**
     * A parsed recipe with metadata extracted
     */
    struct ParsedRecipe
    {
        /** The recipe title*/
        QString title;
        /** A list of ingredients */
        QVector<IngredientsExtractor::Ingredient> ingredients;
        /** The tags extracted from the metadata*/
        QVector<QString> tags;
        /** Less formalized bits of metadata. At a more formal point, it might
         deserve to be treated like tags */
        QMap<QString,QVector<QString>> otherMeta; // we want them ordered
    };
    /**
     * Parses a recipe
     * \param input open QIODevice to read the recipe from
     * \return Parsed Recipe.
     */
    ParsedRecipe parseRecipe(QIODevice* input);
};
