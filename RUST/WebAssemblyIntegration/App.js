import init, { ImageProcessor } from "../pkg/image_processor.js";

let processor;
let canvas = document.getElementById("canvas");
let ctx = canvas.getContext("2d");

document.getElementById("file-input").addEventListener("change", async (event) => {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = async (e) => {
        const base64 = e.target.result.split(",")[1];
        processor = new ImageProcessor(base64);

        const image = new Image();
        image.onload = () => {
            canvas.width = image.width;
            canvas.height = image.height;
            ctx.drawImage(image, 0, 0);
        };
        image.src = `data:image/png;base64,${base64}`;
    };
    reader.readAsDataURL(file);
});

document.getElementById("grayscale").addEventListener("click", async () => {
    if (!processor) return;
    processor.apply_grayscale();
    updateCanvas();
});

document.getElementById("invert").addEventListener("click", async () => {
    if (!processor) return;
    processor.apply_invert();
    updateCanvas();
});

document.getElementById("noise").addEventListener("click", async () => {
    if (!processor) return;
    processor.apply_noise();
    updateCanvas();
});

async function updateCanvas() {
    const base64 = processor.export_image();
    const image = new Image();
    image.onload = () => {
        ctx.drawImage(image, 0, 0);
    };
    image.src = `data:image/png;base64,${base64}`;
}