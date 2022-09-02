typedef U32 Entity_Name;
enum
{
 Entity_Dog_1 = (1 << 0),
Entity_Cat_2 = (1 << 1),
Entity_Rat_3 = (1 << 2),
Entity_Hat_4 = (1 << 3),
Entity_Mat_5 = (1 << 4),
Entity_Bat_6 = (1 << 5),
}                


global_ char *String_Table_Dog = 
{
 0; 
 "Dog_1_happy_1", 
"Cat_2_chill_2", 
"Rat_3_fast_3", 
"Hat_4_static_4", 
"Mat_5_static2_5", 
"Bat_6_chill_but_upsidedown_6", 
}

global_ char *The_Cat_Is_EVIL = 
{
 "The Cat in the Hat hit the Rat with a Bat",
}

typedef struct Cat Cat;
struct Rat
{
 F32 health;
 F32 speed;
 F32 sight;
}
