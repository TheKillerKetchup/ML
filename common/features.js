const features = {};

features.getPathCount=(paths)=>
{
    return paths.length;
}

features.getPointCount=(paths)=>
{
    /*
    const points = 0;
    for(let path of paths)
    {
        points += path.length;
    }
    return points;
    */
   const points = paths.flat();
   return points.length;
}

if(typeof module !== 'undefined')
{
    module.exports = features;
}