::print("something vargv.len(): " + vargv.len());

foreach (i,val in vargv)
{
        ::print("the type of vargv is "+typeof val + " : " + val);
}

local a = [1,2];
local b = "2";
local c = a[1];

local table = {
        a = "10"
        subtable = {
                array = [1,2,3]
        },
        [10 + 123] = "expression index"
}

local array=[ 1, 2, 3, { a = 10, b = "string" } ];

foreach (i,val in array)
{
        ::print("the type of val is"+typeof val);
}

/////////////////////////////////////////////

class Entity
{
        constructor(etype,entityname)
        {
                name = entityname;
                type = etype;
        }

        x = 0;
        y = 0;
        z = 0;
        name = null;
        type = null;
}

function Entity::MoveTo(newx,newy,newz)
{
        x = newx;
        y = newy;
        z = newz;
}

class Player extends Entity {
        constructor(entityname)
        {
                base.constructor("Player",entityname)
        }
        function DoDomething()
        {
                ::print("something");
        }

}

local newplayer = Player("da playar");

newplayer.MoveTo(100,200,300);

return "666" == "666";
