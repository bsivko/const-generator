# Element of Type #

All constant elements are const pointers.

The elements can be created by several ways:

```
const date_time_t * const_time_1 = date_time_modes_t::factory( "const" );
const date_time_t * const_time_2 = date_time_modes_t::factory( current_date_time_t().type() );
```

`const_time_1` was created by string name.
`const_time_1` was created by type.

# Comparing of two elements #

Use a pointer comparison:

```
const_time_1 == const_time_2
```

# Comparing with another constant #

Make a new pointer and compare:

```
const_time_2->type() != current_date_time_t().type()
const_time_2 == date_time_modes_t::factory( const_date_time_t().type() )
```

# Name of Type #

Use `type()` method:

```
const_time_1->type()
current_date_time_t().type()
```

# Make list of constants by delimiter #

Use a function of modes-class:

```
date_time_modes_t::get_list( " | " )
```

# Control of incorrect run-time data #

For example, an unpredictable entering of user:

```
std::string dtm;
std::cin >> dtm;

if ( date_time_modes_t::exist_type( dtm ) )
	const_time = date_time_modes_t::factory( dtm );
```

We use the function `exist_type` to do our decision.

# Extending methods with virtual control #

Example:

```
// Mode of date_time transaction.
class date_time_t
{
	public:
...
		//! Create controller by type.
		virtual a_serial_controller_t *
		get_new_instance( 
			//! Controller name.
			const std::string & name, 
			//! Configuration.
			const serial_cfg_t & send_cfg ) const = 0;
};

//! Use current time at start of transaction.
class current_date_time_t : public date_time_t
{
	public:
...
		virtual a_serial_controller_t *
		get_new_instance( 
			const std::string & name, 
			const serial_cfg_t & send_cfg ) const;
};
```

We use a root class `date_time_t` to config a user time control of transactions. At some moment we want to extend with function. To do this we create a virtual function in the root class. The compiler controls the function creating in every constant. And we can use the function in all of these constants and in the class `date_time_modes_t`.

# Extending new constants #

We should do two actions:
  1. Create a new sub-class of root-class. And create all of virtual methods.
  1. Add in the function `init()` inserting to container by sample.