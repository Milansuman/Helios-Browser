(() => {
    const canvas = document.createElement('canvas');
    canvas.width = window.innerWidth;
    canvas.height = 2; // We only need the top 2 pixels

    // Get the 2D rendering context of the canvas
    const ctx = canvas.getContext('2d');

    // Draw the webpage content onto the canvas
    ctx.drawWindow(window, 0, 0, window.innerWidth, 2, 'rgb(255, 255, 255)');

    // Get the image data of the top 2 pixels
    const imageData = ctx.getImageData(0, 0, window.innerWidth, 2);
    const data = imageData.data;

    // Extract the RGB color values of the top 2 pixels
    const pixel1Color = [data[0], data[1], data[2]];
    const pixel2Color = [data[4], data[5], data[6]];
    return {
        r: pixel1Color[0],
        g: pixel1Color[1],
        b: pixel2Color[2]
    }
})();