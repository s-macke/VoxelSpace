
// ---------------------------------------------
// Keyboard and mouse interaction
// ---------------------------------------------

var input =
{
    forwardbackward: 0,
    leftright:       0,
    updown:          0,
    lookleftright:   0,
    lookupdown:      0,
    mouseposition:   null,
    keypressed:      false
}
// ---------------------------------------------

var time = new Date().getTime();

// ---------------------------------------------
// Keyboard and mouse event handlers
// ---------------------------------------------

function GetMousePosition(e)
{
    // fix for Chrome
    if (e.type.startsWith('touch'))
    {
        return [e.targetTouches[0].pageX, e.targetTouches[0].pageY];
    } else
    {
        return [e.pageX, e.pageY];
    }
}


function DetectMouseDown(e)
{
    input.forwardbackward = 3.;
    input.mouseposition = GetMousePosition(e);
    time = new Date().getTime();

    if (!updaterunning) Draw();
    return;
}

function DetectMouseUp()
{
    input.mouseposition = null;
    input.forwardbackward = 0;
    input.lookleftright = 0;
    input.updown = 0;
    return;
}

function DetectMouseMove(e)
{
    e.preventDefault();
    if (input.mouseposition == null) return;

    var currentMousePosition = GetMousePosition(e);

    input.lookleftright = (input.mouseposition[0] - currentMousePosition[0]) / window.innerWidth * 2;
    camera.horizon  = 100 + (input.mouseposition[1] - currentMousePosition[1]) / window.innerHeight * 500;
    input.updown    = (input.mouseposition[1] - currentMousePosition[1]) / window.innerHeight * 10;
}


function DetectKeysDown(e)
{
    switch(e.keyCode)
    {
    case 37:    // left cursor
    case 65:    // a
        input.leftright = +2.;
        break;
    case 39:    // right cursor
    case 68:    // d
        input.leftright = -2.;
        break;
    case 38:    // cursor up
    case 87:    // w
        input.forwardbackward = +3.;
        break;
    case 40:    // cursor down
    case 83:    // s
        input.forwardbackward = -3.;
        break;
    case 82:    // r
        input.updown = +2.;
        break;
    case 70:    // f
        input.updown = -2.;
        break;
    case 69:    // e
        input.lookupdown = +2;
        break;
    case 81:    //q
        input.lookupdown = -2;
        break;
    default:
        return;
        break;
    }

    if (!updaterunning) {
        time = new Date().getTime();
        Draw();
    }
    return false;
}

function DetectKeysUp(e)
{
    switch(e.keyCode)
    {
    case 37:    // left cursor
    case 65:    // a
        input.leftright = 0;
        break;
    case 39:    // right cursor
    case 68:    // d
        input.leftright = 0;
        break;
    case 38:    // cursor up
    case 87:    // w
        input.forwardbackward = 0;
        break;
    case 40:    // cursor down
    case 83:    // s
        input.forwardbackward = 0;
        break;
    case 82:    // r
        input.updown = 0;
        break;
    case 70:    // f
        input.updown = 0;
        break;
    case 69:    // e
        input.lookupdown = 0;
        break;
    case 81:    //q
        input.lookupdown = 0;
        break;
    default:
        return;
        break;
    }
    return false;
}

