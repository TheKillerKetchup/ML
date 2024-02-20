
function createRow(container, studentName, samples)
{
    //creates a row element at the bottom
    const row = document.createElement("div");
    //gives the row element the class "row"
    row.classList.add("row");
    //puts it on the container
    container.appendChild(row);

    //labels the row with the student's name
    const rowLabel = document.createElement("div");
    rowLabel.innerHTML = studentName;
    rowLabel.classList.add("rowLabel");
    
    /*
    I put this line of code
    container.appendChild(rowLabel);
    instead of the line after this comment.
    Which made the container put [drawings] then under it the label. 
    If you're not me, or you're me and don't remember, swap those two lines and see the difference
    */
    row.appendChild(rowLabel);

    //runs through each drawing
    for(let sample of samples)
    {
        //pulls out all the info
        const {id,label,student_id} = sample;

        //creates the sample container, and gives it a unique id
        const sampleContainer = document.createElement("div");
        sampleContainer.id = "sample_"+id;
        sampleContainer.classList.add("sampleContainer");

        //labels the sample with the label pulled from the sample object
        const sampleLabel = document.createElement("div");
        sampleLabel.innerHTML=label;
        sampleContainer.appendChild(sampleLabel);

        //creates an image element, pulled from the img directory
        const img = document.createElement("img");
        img.src = constants.IMG_DIR +"/"+ id+".png";
        //thumb for thumbnail
        img.classList.add("thumb");
        //if it is a flagged user, it blurs it
        if(utils.flaggedUsers.includes(student_id))
        {
            img.classList.add("blur");
        }
        sampleContainer.appendChild(img);

        //adds the sample(the drawing) into the row
        row.appendChild(sampleContainer);
    }
}