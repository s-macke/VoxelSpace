
// ---------------------------------------------
// Viewer information
// ---------------------------------------------

var camera =
{
    x:        512., // x position on the map
    y:        800., // y position on the map
    height:    78., // height of the camera
    angle:      0., // direction of the camera
    horizon:  100., // horizon position (look up and down)
    distance: 800   // distance of map
};
// ---------------------------------------------

// Update the camera for next frame. Dependent on keypresses
function UpdateCamera()
{
    var current = new Date().getTime();

    input.keypressed = false;
    if (input.lookleftright != 0)
    {
        camera.angle += input.lookleftright*0.1*(current-time)*0.03;
        input.keypressed = true;
    }
    if (input.forwardbackward != 0)
    {
        camera.x -= input.forwardbackward * Math.sin(camera.angle) * (current-time)*0.03;
        camera.y -= input.forwardbackward * Math.cos(camera.angle) * (current-time)*0.03;
        input.keypressed = true;
    }
    if (input.leftright != 0)
    {
        camera.x -= input.leftright * Math.sin(1.57+camera.angle) * (current-time)*0.03;
        camera.y -= input.leftright * Math.cos(1.57+camera.angle) * (current-time)*0.03;
        input.keypressed = true;
    }
    if (input.updown != 0)
    {
        camera.height += input.updown * (current-time)*0.03;
        input.keypressed = true;
    }
    if (input.lookupdown)
    {
        camera.horizon += input.lookupdown * (current-time)*0.03;
        input.keypressed = true;
    }

    // Collision detection. Don't fly below the surface.
    var mapoffset = ((Math.floor(camera.y) & (map.width-1)) << map.shift) + (Math.floor(camera.x) & (map.height-1))|0;
    if ((map.altitude[mapoffset]+10) > camera.height) camera.height = map.altitude[mapoffset] + 10;

    time = current;

}

