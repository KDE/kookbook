#! /usr/bin/python3
#
# Copyright (c) 2019 Sune Vuorela <sune@vuorela.dk>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# Reads a krecipes database, krecipes.krecdb from current directory and outputs
# the data in kookbook markdown ready format also in current directory

import sqlite3
import re
import base64
from collections import defaultdict

connection = sqlite3.connect('krecipes.krecdb')

cursor = connection.cursor()

def filenamefromtitle(string):
    return re.sub("[- ()\"'/,]","",string.lower())

taglist = defaultdict(list)
for recipe_id, tagname in cursor.execute('select recipe_id, categories.name '
                                             'from category_list '
                                             'join categories on categories.id = category_id '
                                             'where category_id != -1 '
                                             'order by recipe_id;'
                                             ):
    taglist[recipe_id].append(tagname)

authorlist = defaultdict(list)
for recipe_id, author_name in cursor.execute('select recipe_id, authors.name '
                                                 'from author_list '
                                                 'join authors on authors.id = author_id;'
                                                 ):
    authorlist[recipe_id].append(author_name)

ingredientlist = defaultdict(list)
for recipe_id, amount, unit, name, additional in cursor.execute('select ingredient_list.recipe_id, ingredient_list.amount, units.plural, ingredients.name, prep_methods.name '
                                                                    'from ingredient_list '
                                                                    'join ingredients on ingredients.id = ingredient_list.ingredient_id '
                                                                    'join units on units.id = ingredient_list.unit_id '
                                                                    'left join prep_method_list on ingredient_list.id = prep_method_list.ingredient_list_id '
                                                                    'left join prep_methods on prep_method_list.prep_method_id = prep_methods.id '
                                                                    'order by recipe_id, ingredient_list.order_index;'
                                                                    ):
    ingredientlist[recipe_id].append((amount, unit, name, additional))

for recipe_id, title, instructions, photo, yield_amount, yield_name, prep_time  in cursor.execute('select recipes.id, recipes.title, recipes.instructions, recipes.photo, recipes.yield_amount, yield_types.name, recipes.prep_time '
                                                                                                  'from recipes '
                                                                                                  'left join yield_types on recipes.yield_type_id = yield_types.id;'
                                                                                                  ):
    filename = filenamefromtitle(title)
    if photo:
        with open("{}.jpg".format(filename), "wb") as picture:
            picture.write(base64.b64decode(photo))
    with open(filename + ".recipe.md", "w", encoding="utf-8") as recipefile:
        recipefile.write("# {}\n\n".format(title))
        if yield_amount and yield_name:
            recipefile.write("Recipe yields {:,g} {} \n\n".format(yield_amount, yield_name))
        if prep_time and prep_time != "00:00:00":
            recipefile.write("Prep time is {} \n\n".format(prep_time))
        if photo:
            recipefile.write("![Image]({}.jpg)\n".format(filename))
        recipefile.write("### Ingredients\n")
        for amount, unit, name, additional  in ingredientlist[recipe_id]:
            recipefile.write(" * ")
            if amount != 0.0:
                recipefile.write('{:,g}'.format(amount))
                recipefile.write(" ")
                if unit:
                    recipefile.write(unit)
                else:
                    recipefile.write("pieces")
                recipefile.write(" ")
            recipefile.write(name)
            if additional:
                recipefile.write(", {}".format(additional))
            recipefile.write("\n")
        recipefile.write("\n")
        recipefile.write("### Directions\n\n")
        recipefile.write(instructions)
        recipefile.write("\n\n")
        recipefile.write("### Meta\n")
        for author in authorlist[recipe_id]:
            recipefile.write("author: {}\n".format(author))
        recipefile.write("\n")
        tags = taglist[recipe_id]
        if tags:
            recipefile.write("tags: {}\n".format(", ".join(tags)))
        recipefile.write("\n")
