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

#include "ingredientsparserpane.h"
#include "ingredientsextractor.h"
#include <memory>
#include <QLineEdit>
#include <QFormLayout>
#include <QAction>
#include <QLabel>

IngredientsParserPane::IngredientsParserPane ( QWidget* parent )
{
    auto edit = std::make_unique<QLineEdit>();
    edit->setPlaceholderText("Search");
    auto action = edit->addAction(QIcon::fromTheme("edit-clear"),QLineEdit::TrailingPosition);
    connect(action, &QAction::triggered, edit.get(), &QLineEdit::clear);
    connect(edit.get(), &QLineEdit::textChanged, this, &IngredientsParserPane::setLine);

    auto amountLabel  = std::make_unique<QLabel>();
    m_amountLabel = amountLabel.get();
    auto ingredientLabel = std::make_unique<QLabel>();
    m_ingredientLabel = ingredientLabel.get();
    auto unitLabel = std::make_unique<QLabel>();
    m_unitLabel = unitLabel.get();

    auto form = std::make_unique<QFormLayout>();
    form->addRow("Amount:", amountLabel.release());
    form->addRow("Unit:", unitLabel.release());
    form->addRow("Ingredient:", ingredientLabel.release());


    auto vbox = std::make_unique<QVBoxLayout>();
    vbox->addWidget(new QLabel(" * 200 ml maple syrup, the good stuff"));
    vbox->addWidget(edit.release());
    vbox->addLayout(form.release());
    vbox->addStretch();
    setLayout(vbox.release());
}

void IngredientsParserPane::setLine(const QString& line) {
    auto ingredients = IngredientsExtractor::parseLine(line);
    m_amountLabel->setText(ingredients.amount);
    m_ingredientLabel->setText(ingredients.ingredient);
    m_unitLabel->setText(ingredients.unit);
}
