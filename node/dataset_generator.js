//essentially imports [file], i.e draw.js, and lets me use its functions and variables
const draw = require("../common/draw.js");
const constants = require("../common/constants.js");
const utils = require("../common/utils.js");

//pulls the canvas library
const {createCanvas}=require('canvas');
//creates a canvas 400,400
const canvas = createCanvas(400,400);
//the context of the canvas
const ctx = canvas.getContext("2d");

//import the 'fs' library
const fs = require('fs');

/*
pulls the filenames from the raw data
*/
const fileNames = fs.readdirSync(constants.RAW_DIR);
const samples = [];
let id = 1;

/*
loops through all the files

*/
fileNames.forEach(fn =>{
    //takes all the content
    const content = fs.readFileSync(
        constants.RAW_DIR+"/"+fn
    );
    //puts the content into a JSON format
    const {session,student,drawings}=JSON.parse(content);

    //loops through all 8 drawings
    for (let label in drawings) {
        //throws them into the samples
        samples.push({
            id,
            label,
            student_name: student,
            student_id: session,
        });

        //throws the paths into the json dir as a json file
        const paths = drawings[label];
        fs.writeFileSync(
            constants.JSON_DIR + "/" + id + ".json",
            JSON.stringify(paths),
        );

        //creates an image file with paths
        generateImageFile(
            constants.IMG_DIR + "/" + id + ".png",
            paths,
        );

        //prints the progress 
        utils.printProgress(id, fileNames.length * 8);
        id++;
    }
});
//writes all the samples into samples.json
fs.writeFileSync(constants.SAMPLES,
    JSON.stringify(samples)
);
//writes all the samples into samples.js
fs.writeFileSync(constants.SAMPLES_JS,
    "const samples="+JSON.stringify(samples)+";"
);

/*
writes the image file by using draw paths
*/
function generateImageFile(outFile,paths)
{
    ctx.clearRect(0,0,
        canvas.width,canvas.height);
    draw.paths(ctx,paths);
    
    const buffer = canvas.toBuffer("image/png");
    fs.writeFileSync(outFile,buffer);
}
