//defines what draw will export
const draw={};

/*
draws one path
*/
draw.path=(ctx,path,color="black")=>{
    //sets some styles
    ctx.strokeStyle = color;
    ctx.lineCap = "round";
    ctx.lineJoin = "round";
    ctx.lineWidth = 3;
    //begins path, moves it to the first dot
    ctx.beginPath();
    ctx.moveTo(...path[0]);
    
    //draws lines to all the dots in order
    for(let i = 1;i<path.length;i+=1)
    {
        ctx.lineTo(...path[i]);
    }

    //strokes - sort of like a flush 
    ctx.stroke();
}

/*
draws all the paths
*/
draw.paths=(ctx,paths,color = "black")=>{
    for(const path of paths){
        draw.path(ctx,path,color);
    }
}

if(typeof module !=='undefined')
{
    module.exports = draw;
}

