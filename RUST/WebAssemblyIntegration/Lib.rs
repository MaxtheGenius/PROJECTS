use wasm_bindgen::prelude::*;
use image::{DynamicImage, ImageBuffer, Rgba};
use rand::Rng;

#[wasm_bindgen]
pub struct ImageProcessor {
    image: DynamicImage,
}

#[wasm_bindgen]
impl ImageProcessor {
    // Load image data from browser (as base64-encoded PNG/JPEG)
    #[wasm_bindgen(constructor)]
    pub fn new(base64_image: &str) -> Result<ImageProcessor, JsValue> {
        let image_data = base64::decode(base64_image).map_err(|e| e.to_string())?;
        let image = image::load_from_memory(&image_data).map_err(|e| e.to_string())?;
        Ok(ImageProcessor { image })
    }

    // Apply grayscale filter
    pub fn apply_grayscale(&mut self) {
        self.image = self.image.grayscale();
    }

    // Apply inversion filter
    pub fn apply_invert(&mut self) {
        self.image.invert();
    }

    // Apply a random noise filter
    pub fn apply_noise(&mut self) {
        let mut rng = rand::thread_rng();
        let mut img = self.image.to_rgba8();

        for pixel in img.pixels_mut() {
            pixel[0] = pixel[0].saturating_add(rng.gen_range(0..50));
            pixel[1] = pixel[1].saturating_add(rng.gen_range(0..50));
            pixel[2] = pixel[2].saturating_add(rng.gen_range(0..50));
        }

        self.image = DynamicImage::ImageRgba8(img);
    }

    // Export image back to base64
    pub fn export_image(&self) -> Result<String, JsValue> {
        let mut buffer = Vec::new();
        self.image
            .write_to(&mut buffer, image::ImageOutputFormat::Png)
            .map_err(|e| e.to_string())?;
        Ok(base64::encode(&buffer))
    }
}