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
#include "metadatapane.h"
#include "recipeparser.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QFile>

MetaDataPane::MetaDataPane(QWidget* parent) : PaneBase(parent)
{
    m_model = std::make_unique<QStandardItemModel>();
    auto layout = std::make_unique<QVBoxLayout>();
    auto treeview = std::make_unique<QTreeView>();
    treeview->setModel(m_model.get());
    treeview->setHeaderHidden(true);
    layout->addWidget(treeview.release());
    setLayout(layout.release());
}

MetaDataPane::~MetaDataPane()
{
    // for smart pointers
}


void MetaDataPane::openPath(const QString& path)
{
    m_model->clear();

    if (path.isEmpty() || !QFile::exists(path)) {
        return;
    }

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        return;
    }
    auto parsedRecipe = RecipeParser::parseRecipe(&f);
    f.close();


    if (parsedRecipe.title.isEmpty() && parsedRecipe.tags.isEmpty() && parsedRecipe.ingredients.isEmpty()) {
        return;
    }

    auto title = std::make_unique<QStandardItem>(parsedRecipe.title);
    title->setEditable(false);
    m_model->appendRow(title.release());

    auto ingredients = std::make_unique<QStandardItem>(QString("ingredients (%1)").arg(parsedRecipe.ingredients.size()));
    ingredients->setEditable(false);
    for(auto ingredient : std::as_const(parsedRecipe.ingredients)) {
        auto ing = std::make_unique<QStandardItem>(ingredient.ingredient);
        ing->setEditable(false);
        ingredients->appendRow(ing.release());
    }
    m_model->appendRow(ingredients.release());

    auto tags  = std::make_unique<QStandardItem>(QString("tags (%1)").arg(parsedRecipe.tags.size()));
    tags->setEditable(false);
    for(auto tag : std::as_const(parsedRecipe.tags)) {
        auto tagItem = std::make_unique<QStandardItem>(tag);
        tagItem->setEditable(false);
        tags->appendRow(tagItem.release());
    }

    m_model->appendRow(tags.release());
    auto meta = std::make_unique<QStandardItem>(QString("meta (%1)").arg(parsedRecipe.otherMeta.size()));
    meta->setEditable(false);
    for(auto it = parsedRecipe.otherMeta.constBegin(), end = parsedRecipe.otherMeta.constEnd(); it != end; it++) {
        auto child = std::make_unique<QStandardItem>(it.key());
        child->setEditable(false);
        const auto values = it.value();
        for(const auto& value : values)
        {
            auto valueItem = std::make_unique<QStandardItem>(value);
            valueItem->setEditable(false);
            child->appendRow(valueItem.release());
        }
        meta->appendRow(child.release());
    }
    m_model->appendRow(meta.release());

}

