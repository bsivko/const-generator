# Introduction #

The Const Generator allows to create classes to support constant variables.

It's a specific format like enums.

# Details #

The format can be used if we involve constants and we want to extend functionality.

## Functional control of all constants ##

If we use a simple enum list we can't create a function which can process all of them.

For example, if we want to make a variables list to print, or to find a constant with some run-time value.

## Extend constants by additional data ##

The enum-list is simple and can't extend data, which are associated with constant. In this case we should create some addition data structures and create functions to work with them. Such way is spontaneous and isn't systematic. The managing of this process like "Const Generator" should do this easier and more reliable.

## Easy to extend ##

The extending should be easy, fast and reliable.

The extending need minimum user actions to create new constants and allows to control extending of all of data and functions.