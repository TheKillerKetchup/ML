class SketchPad{
    /*
    */
    constructor(container,size = 400)
    {
        //creates a canvas of size 400
        this.canvas = document.createElement("canvas");
        this.canvas.width = size;
        this.canvas.height = size;
        this.canvas.style = `
            background-color:white;
            box-shadow: 0px 0px 10px 2px black;
        `;

        //adds canvas to container
        container.appendChild(this.canvas);

        //line break
        const lineBreak = document.createElement("br");
        container.appendChild(lineBreak);

        //creates undo button w/ inner HTML "UNDO"
        this.undoBtn = document.createElement("button");
        this.undoBtn.innerHTML="UNDO";
        container.appendChild(this.undoBtn);

        //creates a clear all button w/ inner HTML "Clear All"
        this.clearAllBtn = document.createElement("button");
        this.clearAllBtn.innerHTML = "Clear All";
        container.appendChild(this.clearAllBtn);

        //ctx = content, basically all the content on the canvas.(all the points!)
        this.ctx = this.canvas.getContext("2d");

        //calls reset
        this.reset();
       
        //adds all the event listeners
        this.#addEventListeners();

    }
    
    /*
    resets the canvas
    */
    reset(){
        this.paths = [];
        this.isDrawing = false;
        this.#redraw();
    }

    /*
    adds all the event listeners
    */
    #addEventListeners(){
        /*
        on mouse down, start drawing
        */
        this.canvas.onmousedown=(evt)=>
        {
            const mouse = this.#getMouse(evt);
            //creates a new path
            this.paths.push([mouse]);
            this.isDrawing = true;

        }
        /*
        listens, and only if the mouse was previously down, rather than up, it draws. 
        The mouse is being held down.
        */
        this.canvas.onmousemove=(evt)=>
        {
            if(this.isDrawing){
                //gets mouse array
                const mouse = this.#getMouse(evt);
                //adds a new point to the lastPath
                const lastPath = this.paths[this.paths.length-1];
                lastPath.push(mouse);
                this.#redraw();
            }
        }
        /*
        Not drawing - mouse is up
        */
        document.onmouseup=()=>
        {
            this.isDrawing = false;
        }

        /*
        these all copy the mouse functions for touchscreens. 
        Since multiple touches are allowed, only the first touch is counted.
        */
        this.canvas.ontouchstart=(evt)=>
        {
            const loc = evt.touches[0];
            this.canvas.onmousedown(loc);
        }
        this.canvas.ontouchmove=(evt)=>
        {
            const loc = evt.touches[0];
            this.canvas.onmousemove(loc);
        }
        document.ontouchup=()=>{
            document.onmouseup();
        }
        /*
        On click of undo button, undo last path
        */
        this.undoBtn.onclick=()=>{
            this.paths.pop();
            this.#redraw();
        }
        /*
        Displays the confirmation modal
        */
        this.clearAllBtn.onclick=()=>{
            document.getElementById("confirmationModal").style.display = "block";
        }
        const yesBtn = document.getElementById("yesButton");
        const noBtn = document.getElementById("noButton");
        /*
        If clicked, clears path and redraws canvas - canvas is cleared
        for both - hides confirmation modal
        */
        yesBtn.onclick=()=>
        {

            this.paths = [];
            this.#redraw();
            this.#hideconfirmationModal();
        }
        noBtn.onclick=()=>
        {
            this.#hideconfirmationModal();
        }

    }
    #hideconfirmationModal()
    {
        document.getElementById("confirmationModal").style.display = 'none';
    }
    #redraw(){
        /*
        clears the whole rect
        */
        this.ctx.clearRect(0,0,
            this.canvas.width,this.canvas.height);
        //calls the draw function
        draw.paths(this.ctx,this.paths);
        //both buttons can only be used if there's something to undo/clear
        if(this.paths.length > 0)
        {
            this.undoBtn.disabled=false;
            this.clearAllBtn.disabled=false;
        }else{
            this.undoBtn.disabled=true;
            this.clearAllBtn.disabled=true;
        }
    }

    /*
    Gets The position of the mouse as an array
    */
    #getMouse=(evt)=>{
        const rect = this.canvas.getBoundingClientRect();
        return[
            //positions relative to rect(canvas) borders, instead of absolute
            Math.round(evt.clientX - rect.left),
            Math.round(evt.clientY - rect.top)
        ];
    }       
}