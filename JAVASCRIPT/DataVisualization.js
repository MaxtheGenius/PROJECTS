// Import D3.js library
import * as d3 from "d3";

// Mock real-time sales data
const salesData = [
  { region: "North America", month: "January", sales: 500 },
  { region: "Europe", month: "January", sales: 400 },
  { region: "Asia", month: "January", sales: 600 },
  { region: "North America", month: "February", sales: 700 },
  { region: "Europe", month: "February", sales: 550 },
  { region: "Asia", month: "February", sales: 650 },
];

// Extract unique regions and months from the data
const regions = Array.from(new Set(salesData.map((d) => d.region)));
const months = Array.from(new Set(salesData.map((d) => d.month)));

// Define chart dimensions
const chartWidth = 400;
const chartHeight = 300;
const margin = { top: 20, right: 30, bottom: 50, left: 50 };

// Function to draw a bar chart: Monthly sales by region
function drawBarChart(containerId) {
  const svg = d3
    .select(containerId)
    .append("svg")
    .attr("width", chartWidth)
    .attr("height", chartHeight);

  const x = d3
    .scaleBand()
    .domain(months)
    .range([margin.left, chartWidth - margin.right])
    .padding(0.1);

  const y = d3
    .scaleLinear()
    .domain([0, d3.max(salesData, (d) => d.sales)])
    .nice()
    .range([chartHeight - margin.bottom, margin.top]);

  svg
    .append("g")
    .selectAll("rect")
    .data(salesData)
    .join("rect")
    .attr("x", (d) => x(d.month))
    .attr("y", (d) => y(d.sales))
    .attr("height", (d) => y(0) - y(d.sales))
    .attr("width", x.bandwidth())
    .attr("fill", "steelblue");

  svg
    .append("g")
    .attr("transform", `translate(0,${chartHeight - margin.bottom})`)
    .call(d3.axisBottom(x));

  svg
    .append("g")
    .attr("transform", `translate(${margin.left},0)`)
    .call(d3.axisLeft(y));
}

// Function to draw a line chart: Monthly trend of sales
function drawLineChart(containerId) {
  const svg = d3
    .select(containerId)
    .append("svg")
    .attr("width", chartWidth)
    .attr("height", chartHeight);

  const x = d3
    .scalePoint()
    .domain(months)
    .range([margin.left, chartWidth - margin.right]);

  const y = d3
    .scaleLinear()
    .domain([0, d3.max(salesData, (d) => d.sales)])
    .nice()
    .range([chartHeight - margin.bottom, margin.top]);

  const line = d3
    .line()
    .x((d) => x(d.month))
    .y((d) => y(d.sales));

  svg
    .append("path")
    .datum(salesData)
    .attr("fill", "none")
    .attr("stroke", "darkorange")
    .attr("stroke-width", 2)
    .attr("d", line);

  svg
    .append("g")
    .attr("transform", `translate(0,${chartHeight - margin.bottom})`)
    .call(d3.axisBottom(x));

  svg
    .append("g")
    .attr("transform", `translate(${margin.left},0)`)
    .call(d3.axisLeft(y));
}

// Function to draw a pie chart: Regional distribution of sales
function drawPieChart(containerId) {
  const radius = Math.min(chartWidth, chartHeight) / 2 - margin.top;

  const svg = d3
    .select(containerId)
    .append("svg")
    .attr("width", chartWidth)
    .attr("height", chartHeight)
    .append("g")
    .attr("transform", `translate(${chartWidth / 2},${chartHeight / 2})`);

  const salesByRegion = d3.rollup(
    salesData,
    (v) => d3.sum(v, (d) => d.sales),
    (d) => d.region
  );

  const pie = d3.pie().value((d) => d[1]);
  const arc = d3.arc().innerRadius(0).outerRadius(radius);

  const color = d3.scaleOrdinal().domain(regions).range(d3.schemeCategory10);

  svg
    .selectAll("path")
    .data(pie(salesByRegion.entries()))
    .join("path")
    .attr("d", arc)
    .attr("fill", (d) => color(d.data[0]))
    .append("title")
    .text((d) => `${d.data[0]}: ${d.data[1]}`);
}

// Main function to render all charts
function renderDashboard() {
  // Create container elements dynamically
  const dashboard = document.createElement("div");
  dashboard.id = "dashboard";
  document.body.appendChild(dashboard);

  const barChartContainer = document.createElement("div");
  barChartContainer.id = "bar-chart";
  barChartContainer.className = "chart";
  dashboard.appendChild(barChartContainer);

  const lineChartContainer = document.createElement("div");
  lineChartContainer.id = "line-chart";
  lineChartContainer.className = "chart";
  dashboard.appendChild(lineChartContainer);

  const pieChartContainer = document.createElement("div");
  pieChartContainer.id = "pie-chart";
  pieChartContainer.className = "chart";
  dashboard.appendChild(pieChartContainer);

  // Draw charts
  drawBarChart("#bar-chart");
  drawLineChart("#line-chart");
  drawPieChart("#pie-chart");
}

// Execute the dashboard rendering
document.addEventListener("DOMContentLoaded", () => {
  renderDashboard();
});