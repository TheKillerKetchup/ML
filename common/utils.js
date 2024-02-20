//defines what utils will export
const utils = {};

//flagged users' id's
utils.flaggedUsers=
   [1663882102141,1663900040545,1664485938220];

utils.styles = {
    car:'grey',
    fish:'red',
    house:'yellow',
    tree:'green',
    bicycle:'cyan',
    guitar:'blue',
    pencil:'magenta',
    clock:'lightgrey'
};
//defines the formatpercent function
utils.formatPercent=(n)=>
{
    return (n*100).toFixed(2)+"%";
}

//prints the progress
utils.printProgress=(count,max)=>
{
    //clears the previous line
    process.stdout.clearLine();
    //cursor moves to zero
    process.stdout.cursorTo(0);
    //percent is formatted float of (count/max)
    const percent = utils.formatPercent(
        count/max
    );
    //writes out the count/max, and then the percent
    process.stdout.write(count+"/"+max +
    " ("+percent+")");
}

/*
sorts the objArray into a dictionary based on the key
its a dictionary of objects so for this app it's
[student_id, {object}] where student_id ends up being the key
*/
utils.groupBy=(objArray,key)=>
{
    const groups = {};
    /*
    when I used for(let obj in objArray) it gave me undefined. Why?
    b/c in objArray counts the indicies - it loops over whatever is enumerable. In this case, thats the indexes
    of objArray loops over the objects in the set,list,array.
    */
    for(let obj of objArray)
    {
        //gets the key value
        const val = obj[key];
        //if theres void where its supposed to be pushed, it creates a platform for it to stand on
        if(groups[val] == null)
        {
            groups[val] = [];
        }
        groups[val].push(obj);
    }
    return groups;
}

//makes sure that the code is running on a commonJS environment, meaning the module is there
if(typeof(module) !=='undefined')
{
    module.exports = utils;
}