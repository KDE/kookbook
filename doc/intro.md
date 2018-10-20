# KookBook

A simple recipe manager taking structured markdown for recipes.

## A cookbook

A cookbook, or collection of recipes, is a folder structure containing one or more recipes with markdown files and named *.recipe.md. The folder structure can be used as one way to browse and group the recipes.
See the file format section for details on the file format.

Recipes can be synced with others using external tools like git repositories, nextcloud or many other services.

## The application

The application consists of a set of panes. Some are for helping finding recipes in different ways, and others are for help recipe writers figuring out how it is parsed.

Right clicking on the top level toolbar let you hide and show panes.

The top level toolbar contains some actions.
 * Open sets the root folder for the recipe collection, or cookbook.
 * Edit launches the system editor to let you edit the selected recipe.
 * New recipe creates a recipe from template and launches a system editor for editing the recipe.
 * Refresh reloads the entire recipe collection. A edited recipe is refreshed upon save, but if external tools have added or removed files, refresh is not automatic.
 * Print and Print preview allows printing


### Main pane
The central one displays the recipe, or if no recipe is selected, this document.

### Recipe pane
The recipes ordered as how they are laid out on the file system in the selected folder and sub folders. Only files ending with recipe.md is parsed.

### Tags pane
The tags pane shows the recipes indexed by their tags

### Ingredients pane
The recipes indexed by their ingredients. This is for example for helping figure out how to use a surplus of bell peppers.

### Title pane
All recipes by title. Search is available.

### Raw view pane
The raw markdown file. Mostly for recipe writers.

### Meta data pane
The data extracted from the recipe. Mostly for recipe writers.

## File Format

The full recipe is rendered as markdown, and some specific pieces of information is parsed from it in a structured way. The file type is .recipe.md

All in all, 3 groups of data are read. Title, Ingredients and Metadata, and are read in that order.

Title is the first top level header, like

    # Fancy salad
is parsed as "Fancy salad" as the title.

The parser is supposed to ignore anything until reaching a line starting with

    ### Ingred
and at this point switch to ingredients parsing. More on that later. Ingredients parsing stops at a line starting with

    ### Direc
and ignores any lines until a line starting with

    ### Meta
The meta section is a set of lines with specific syntax:

    key: value
More on meta data parsing later.
A key can be repeated and is concatenated.


#### Ingredients parsing

Ingredients lines for parsing needs to be structured as a bullet list like

     * amount unit ingredient name, some additional information

and is parsed by ignoring the asterisk, the first two words, and whatever is after a comma.

An ingredient list like

     * 1 kg flour
     * 2 pcs red bell pepper, alternatively yellow bell pepper

will get the parsed ingredient list for indexing and other purposes as

    flour
    red bell pepper


Ingredients without an amount, e.g.

     * salt
     * pepper

Will not show up in ingredient indexing.


#### Metadata parsing

so far, only tags as key is parsed specifically as comma separated list like

    tags: kid friendly, vegetarian

is parsed into the tags
 * kid friendly
 * vegetarian

An author tag can only contain one value, but it can be repeated.

    author: John Doe
    author: Jane Doe


Other tags with specific parsing rules might come in the future



## About

Kookbook is made by Sune Vuorela <sune@kde.org> and is available under the MIT/X11 license (see license).

Kookbook uses the Qt Toolkit <http://qt.io> for UI and file access functionality, and uses the markdown library Discount <https://github.com/Orc/discount>


## License

    Copyright (c) 2018 Sune Vuorela <sune@vuorela.dk>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.


