# Using #

Use command-line to create base constants files.

If you want to use it without compilation you can get binary files in [tag-bin](http://code.google.com/p/const-generator/source/browse/#svn%2Ftags%2Fbin) directory.

The generator creates only base files by user params. The next extending should be done by hand.

To view help of command line type:

```
>const_generator --help
```

# Example #

Lets look at run of the next command:

```
>const_generator -n generator::main -r date_time current const all_current
```

As a result, "Const Generator" should create two files:

"date\_time.hpp"
"date\_time.cpp"

The internal code will be placed in the namespace `generator::main`.

Base type of class is `date_time_t`, and we'll find three child-classes:
`current_date_time_t`, `const_date_time_t`, `all_current_date_time_t`.

In addition, we'll find a class `date_time_modes_t`, which controls all of its constant types.