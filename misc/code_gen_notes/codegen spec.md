# codegen spec

## language syntax
### keywords
@this_is_a_keyword

Keywords:
@generate_block // specifies a block that will be used to generate code
@generate_loop // specifies a loop that will loop over all the rows in the table and for that row if 
						       an element accessor is encountered, it will use the row from the loop and the column from the accessor
@index // can be used to generate the current implicit gen_index that is incrimented by loops and by @inc
@inc // increments the implicit gen_index
@dec // decrements the implicit gen_index

### table accessing

$name used to access columns of a table 
$animal will specify the zeroth column of the table_name
$animal[index] will specify zeroth column and index will specify the row


## tables
- tables contain either single elements on each row separated by comma or
tables contain compound elements that must be inside braces separated by whitespace
- tables must be declared before the first @generate_block otherwise it's treated as generated code

Ex: single_element,
Ex: {element1 element2},

table_name(animal, age, speed)
{
	{Dog 10 slow},
	{Cat  9 fast},
}


## generate blocks
syntax
	@generate_block(table_name)

everything implicity beneath generate block gets generated into the file

### input
Example: 
@generate_block(table_name)
enum Animals
{
	$animal[0],
	@generate_loop {
		$animal $age,
	},
	$animal[1],
}

### output
```
enum Animals
{
	Dog,
	Dog 10,
	Cat 9,
	Cat,
}
```

## generate loops