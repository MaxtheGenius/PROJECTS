// line-chart.component.ts
import { Component, Input, OnChanges } from '@angular/core';
import * as d3 from 'd3';

@Component({
  selector: 'app-line-chart',
  template: `<div class="chart"></div>`,
  styles: [`.chart { width: 100%; height: 400px; }`],
})
export class LineChartComponent implements OnChanges {
  @Input() data: { name: string; value: number }[] = [];

  ngOnChanges() {
    this.createChart();
  }

  private createChart() {
    const element = d3.select('.chart');
    element.selectAll('*').remove();

    const width = element.node()?.clientWidth || 600;
    const height = 400;
    const margin = { top: 20, right: 20, bottom: 30, left: 40 };

    const svg = element.append('svg')
      .attr('width', width)
      .attr('height', height);

    const x = d3.scalePoint()
      .domain(this.data.map(d => d.name))
      .range([margin.left, width - margin.right]);

    const y = d3.scaleLinear()
      .domain([0, d3.max(this.data, d => d.value) || 0])
      .nice()
      .range([height - margin.bottom, margin.top]);

    const line = d3.line<{ name: string; value: number }>()
      .x(d => x(d.name)!)
      .y(d => y(d.value)!);

    svg.append('path')
      .datum(this.data)
      .attr('fill', 'none')
      .attr('stroke', '#007acc')
      .attr('stroke-width', 2)
      .attr('d', line);

    svg.append('g')
      .call(d3.axisBottom(x))
      .attr('transform', `translate(0,${height - margin.bottom})`);

    svg.append('g')
      .call(d3.axisLeft(y))
      .attr('transform', `translate(${margin.left},0)`);
  }
}