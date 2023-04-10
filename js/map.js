
// ---------------------------------------------
// Landscape data
// ---------------------------------------------

var map =
{
    width:    1024,
    height:   1024,
    shift:    10,  // power of two: 2^10 = 1024
    altitude: new Uint8Array(1024*1024), // 1024 * 1024 byte array with height information
    color:    new Uint32Array(1024*1024) // 1024 * 1024 int array with RGB colors
};
// ---------------------------------------------

// ---------------------------------------------
// Init routines

// Util class for downloading the png
function DownloadImagesAsync(urls) {
    return new Promise(function(resolve, reject) {

        var pending = urls.length;
        var result = [];
        if (pending === 0) {
            resolve([]);
            return;
        }
        urls.forEach(function(url, i) {
            var image = new Image();
            //image.addEventListener("load", function() {
            image.onload = function() {
                var tempcanvas = document.createElement("canvas");
                var tempcontext = tempcanvas.getContext("2d");
                tempcanvas.width = map.width;
                tempcanvas.height = map.height;
                tempcontext.drawImage(image, 0, 0, map.width, map.height);
                result[i] = tempcontext.getImageData(0, 0, map.width, map.height).data;
                pending--;
                if (pending === 0) {
                    resolve(result);
                }
            };
            image.src = url;
        });
    });
}

function LoadMap(filenames)
{
    var files = filenames.split(";");
    DownloadImagesAsync(["maps/"+files[0]+".png", "maps/"+files[1]+".png"]).then(OnLoadedImages);
}

function OnLoadedImages(result)
{
    var datac = result[0];
    var datah = result[1];
    for(var i=0; i<map.width*map.height; i++)
    {
        map.color[i] = 0xFF000000 | (datac[(i<<2) + 2] << 16) | (datac[(i<<2) + 1] << 8) | datac[(i<<2) + 0];
        map.altitude[i] = datah[i<<2];
    }
    Draw();
}

