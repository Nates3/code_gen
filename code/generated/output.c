typedef U32 Entity_Name;
enum
{
Entity_Name_Dog,
Entity_Name_Cat,
Entity_Name_Rat,
Entity_Name_Hat,
Entity_Name_Mat,
Entity_Name_Bat,
};

global_ char *String_Table = 
{
"Dog_14",
"Cat_9",
"Rat_3",
"Hat_500",
"Mat_1050",
"Bat_50",
};

global_ char *The_Cat_Is_EVIL = 
{
"The Cat in the Hat hit the Rat with a Bat",
};

typedef struct Cat Cat;
 struct Rat
{
F32 health;
F32 speed;
F32 sight;
};

