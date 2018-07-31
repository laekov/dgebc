window.chartColors = {
	red: 'rgb(255, 99, 132)',
	orange: 'rgb(255, 159, 64)',
	yellow: 'rgb(255, 205, 86)',
	green: 'rgb(75, 192, 192)',
	blue: 'rgb(54, 162, 235)',
	purple: 'rgb(153, 102, 255)',
	grey: 'rgb(201, 203, 207)'
};

var randomScalingFactor = function() {
	return Math.random();
};

var config = {
	type: 'line',
	data: {
		datasets: [{
			label: 'some initial curve',
			fill: false,
			backgroundColor: window.chartColors.blue,
			borderColor: window.chartColors.blue,
			data: [
				randomScalingFactor(),
				randomScalingFactor(),
				randomScalingFactor(),
				randomScalingFactor(),
				randomScalingFactor(),
				randomScalingFactor(),
				randomScalingFactor()
			],
		}]
	},
	options: {
		responsive: true,
		title: {
			display: true,
			text: 'Chart.js Line Chart'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: true,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: true,
					labelString: 'Steps per second'
				}
			}]
		}
	}
};

var fakeHosts = {
	'10.10.10.1:1234': {
		color: window.chartColors.red,
		recs: {}
	},
	'10.10.10.2:2334': {
		color: window.chartColors.blue,
		recs: {}
	}
};


var updateDiagram = function() {
	for (var i in fakeHosts) {
		var t = Math.floor(Date.now() / 1000);
		fakeHosts[i].recs[t] = Math.random() * 100;
	}
	var dataset = [];
	var tend = Math.floor(Date.now() / 1000);
	for (var i in fakeHosts) {
		var data = {
			label: i,
			fill: false,
			backgroundColor: fakeHosts[i].color,
			borderColor: fakeHosts[i].color,
			data: []
		};
		for (var j = 99; j >= 0; -- j) {
			if (fakeHosts[i].recs[tend - j] != undefined) {
				data.data.push(fakeHosts[i].recs[tend - j]);
			} else {
				data.data.push(0);
			}
		}
		dataset.push(data);
	}
	var labels = [];
	for (var j = 99; j >= 0; -- j) {
		var d = new Date((tend - j)* 1000);
			labels.push(d.toTimeString());
	}
	config.data.datasets = dataset;
	config.data.labels = labels;
	window.myLine.update();
};

window.onload = function() {
	var ctx = document.getElementById('canvas').getContext('2d');
	Chart.defaults.global.animation.duration=0;
	window.myLine = new Chart(ctx, config);

	for (var i in fakeHosts) {
		var t = Math.floor(Date.now() / 1000);
		fakeHosts[i].recs[t] = Math.random() * 100;
	}

	setInterval(updateDiagram, 1000);
};
