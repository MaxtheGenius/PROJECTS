use actix_web::{web, App, HttpServer, Responder, HttpResponse};
use actix_web::web::Json;
use tokio::time::{sleep, Duration};
use std::sync::Arc;
use tokio::sync::Mutex;

#[actix_web::get("/metrics")]
async fn get_metrics(metrics: web::Data<Arc<Mutex<SystemMetrics>>>) -> impl Responder {
    let metrics = metrics.lock().await;
    HttpResponse::Ok().json(metrics.clone())
}

#[actix_web::get("/")]
async fn index() -> impl Responder {
    HttpResponse::Ok().body("<h1>Welcome to the System Monitoring Dashboard</h1><p>Access <a href='/metrics'>metrics</a> to see real-time data.</p>")
}

async fn simulate_metrics(metrics: web::Data<Arc<Mutex<SystemMetrics>>>) {
    loop {
        let simulated_metrics = get_system_metrics();
        let mut metrics = metrics.lock().await;
        *metrics = simulated_metrics;
        sleep(Duration::from_secs(1)).await;  // Update every second
    }
}

#[tokio::main]
async fn main() -> std::io::Result<()> {
    let metrics = web::Data::new(Arc::new(Mutex::new(get_system_metrics())));

    // Start a background task that simulates metrics data
    tokio::spawn(simulate_metrics(metrics.clone()));

    // Start the Actix web server
    HttpServer::new(move || {
        App::new()
            .app_data(metrics.clone())
            .route("/", web::get().to(index))
            .route("/metrics", web::get().to(get_metrics))
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}