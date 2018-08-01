window.chartColors = {
	red: 'rgb(255, 99, 132)',
	orange: 'rgb(255, 159, 64)',
	yellow: 'rgb(255, 205, 86)',
	green: 'rgb(75, 192, 192)',
	blue: 'rgb(54, 162, 235)',
	purple: 'rgb(153, 102, 255)',
	grey: 'rgb(201, 203, 207)'
};

var colors = ['red', 'orange', 'yellow', 'green', 'blue', 'purple', 'grey'];

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
				randomScalingFactor()
			],
		}]
	},
	options: {
		responsive: true,
		title: {
			display: false,
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

var workers = {
};


var updateDiagram = function() {
	$.post('/get_gene', function(raw_data) {
		var lines = raw_data.split(';');
		for (var i in lines) {
			var name_content = lines[i].split('#');
			if (name_content.length < 2) {
				continue;
			}
			var name = name_content[0];
			var speed = parseInt(name_content[1].split(',')[0]);
			if (!(name in workers)) {
				var colorid = Math.floor(Math.random() * colors.length);
				workers[name] = {
					color: window.chartColors[colors[colorid]],
					recs: {}
				};
			}
			var tm = Math.floor(Date.now() / 1000);
			workers[name].recs[tm] = speed;
		}
		var dataset = [];
		var tend = Math.floor(Date.now() / 1000);
		for (var i in workers) {
			var data = {
				label: i,
				fill: false,
				backgroundColor: workers[i].color,
				borderColor: workers[i].color,
				data: []
			};
            var last = undefined;
			for (var j = 99; j >= 0; -- j) {
				if (workers[i].recs[tend - j] != undefined) {
					data.data.push(workers[i].recs[tend - j]);
                    last = workers[i].recs[tend - j];
				} else {
					data.data.push(last);
				}
			}
			dataset.push(data);
		}
		var labels = [];
		for (var j = 99; j >= 0; -- j) {
			var d = new Date((tend - j)* 1000);
			labels.push(d.toTimeString().substr(0, 8));
		}
		config.data.datasets = dataset;
		config.data.labels = labels;
		window.myLine.update();
	});
};

window.onload = function() {
	var ctx = document.getElementById('canvas').getContext('2d');
	Chart.defaults.global.animation.duration=0;
	window.myLine = new Chart(ctx, config);

	for (var i in workers) {
		var t = Math.floor(Date.now() / 1000);
		workers[i].recs[t] = Math.random() * 100;
	}

	setInterval(updateDiagram, 800);
};

