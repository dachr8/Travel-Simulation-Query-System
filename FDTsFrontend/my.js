/**
 * time multipler
 * 3600 means the simulated time is 3600 times
 * faster than the real world
 * which imply that 1 second represent 1 hour
 *
 * @type {number}
 */
const time_multipler = 3600;

const node_radius = 30;

$('.date').datetimepicker(); // init datetimepicker

/**
 * helper function to get object from form serialize data
 *
 * @returns {{}}
 */
$.fn.serializeObject = function()
{
    var o = {};
    var a = this.serializeArray();
    $.each(a, function() {
        if (o[this.name] !== undefined) {
            if (!o[this.name].push) {
                o[this.name] = [o[this.name]];
            }
            o[this.name].push(this.value || '');
        } else {
            o[this.name] = this.value || '';
        }
    });
    return o;
};

/**
 * same as enum strategy in c++ codes
 * @type {{min_cost: string, min_time: string, limited_time: string}}
 */
const STRATEGY = {
    min_cost: "min_cost",
    min_time: "min_time",
    limited_time: "limited_time"
};

/**
 * web socket instance
 * @type {WebSocket}
 */
var socket = new WebSocket('ws://localhost:8145');

/**
 * simulated time container
 * @type {number}
 */
var sim_time = new Date().getTime();

/**
 * get vertex, edges inited mark
 * @type {[number,number,number]}
 */
var part_init = [0, 0, 0];

var full_init = false;

var selected_city = null;

document.body.style.backgroundColor = '#ffffaa';

socket.addEventListener('close', function (event) {
    document.body.style.backgroundColor = '#ff7777';
});

socket.addEventListener('open', function (event) {
    document.body.style.backgroundColor = '';
});

socket.addEventListener('message', function (event) {
    var j = JSON.parse(event.data);

    if (!full_init && "get_all_vertex" === j.function) {
        dataset.nodes = j.vertex;
        var selection = "";
        for (var i in j.vertex) {
            selection += '<option value="'+j.vertex[i].id+'">'+j.vertex[i].id+'</option>';
        }

//            console.log(selection);

        document.getElementsByName('from')[0].innerHTML = selection;
        document.getElementsByName('to')[0].innerHTML = selection;
        document.getElementsByName('pass_by_vertex_display_name')[0].innerHTML = selection;
        $('.selectpicker').selectpicker('refresh');
        part_init[0] = 1;
    }

    if (!full_init && "get_all_display_edge" === j.function) {
        dataset.edges = parse_raw_edge(j.edges);
        part_init[1] = 1;
    }

    if ("sync_time" === j.function) {
        sim_time = j.time;
        part_init[2] = 1;
    }

    if (full_init && "submit_passenger_requirement" === j.function) {
        // fill marker data
        console.log(j);

        if (j.total_transportation_plan.display_info === "error") {
            return;
        }

        var total_transportation_plan = j.total_transportation_plan;

        var existed_marker = get_marker_by_name(total_transportation_plan.passenger);

        if (existed_marker === null) {
            var new_marker = {
                name: total_transportation_plan.passenger,
                display_info: total_transportation_plan.display_info,
                plan: [],
                plan_index: 0
            };
            for (var i in total_transportation_plan.single_transportation_plans) {

                var k = total_transportation_plan.single_transportation_plans[i];
                var new_plan = {
                    start_node: dataset.nodes[get_node_by_id(k.from)],
                    end_node: dataset.nodes[get_node_by_id(k.to)],
                    start_time: k.start_time,
                    end_time: k.end_time
                };

                new_marker.plan.push(new_plan);
            }

            dataset.markers.push(new_marker);
        } else {
            var m = dataset.markers[existed_marker];
            m.display_info = total_transportation_plan.display_info;
            for (var i in total_transportation_plan.single_transportation_plans) {

                var k = total_transportation_plan.single_transportation_plans[i];
                var new_plan = {
                    start_node: dataset.nodes[get_node_by_id(k.from)],
                    end_node: dataset.nodes[get_node_by_id(k.to)],
                    start_time: k.start_time,
                    end_time: k.end_time
                };

                m.plan.push(new_plan);
            }
        }
        update_marker_data();
    }

    if (!full_init && part_init[0] === part_init[1] && part_init[1] === part_init[2] && part_init[0] === 1) {
        init_vertex_edges();
    }
});

function parse_raw_edge(raw) {
    var r = [];
    raw.forEach(function(e) {
        r.push({source: get_node_by_id(e.source), target: get_node_by_id(e.target)});
    });
    return r;
}

var svg_container = document.querySelector('#svg_container');
var linkDistance = 500;

document.body.style.height = window.innerHeight + "px";
document.querySelector(".container-fluid").style.height = (window.innerHeight * 0.9) + "px";

var colors = d3.scale.category10();

var color2 = d3.scale.category20();

var color3 = function(e) {
    return color2(5 + e);
};

color3();

var dataset = {

    nodes: [
        {id: "City_A"},
        {id: "City_B"}
    ],
    edges: [
        {source: 0, target: 1}
    ],
    markers: [
//            {
//                name: "Marker_A",
//                plan: [
//                    {
//                        start_node: null,
//                        end_node: null,
//                        start_time: sim_time,
//                        end_time: sim_time + 10000
//                    },
//                    {
//                        start_node: null,
//                        end_node: null,
//                        start_time: sim_time + 10000,
//                        end_time: sim_time + 20000
//                    }
//                ],
//                plan_index: 0
//            }
    ]
};


// Connection opened
socket.addEventListener('open', function (event) {
    ["get_all_vertex", "get_all_display_edge", "sync_time"].forEach(function (e) {
        socket.send(JSON.stringify({"function": e}));
    });
});


function get_node_by_id(id) {
    for (var i = 0; i < dataset.nodes.length; ++i) {
        if (id === dataset.nodes[i].id) {
            return i;
        }
    }
    return null;
}

function get_marker_by_name(name) {
    for (var i = 0; i < dataset.markers.length; ++i) {
        if (name === dataset.markers[i].name) {
            return i;
        }
    }
    return null;
}

var svg = d3.select("#svg_container").append('svg').attr({
    "width": "100%", "height": "100%"
});

// svg.call(d3.behavior.zoom().on("zoom", function () {
//     svg.attr("transform", "translate(" + d3.event.translate + ")" + " scale(" + d3.event.scale + ")")
// }))
//     .append("g");

var w = svg_container.offsetWidth;
var h = svg_container.offsetHeight;

var force;
var markers;
var marker_text;

var force_timer_started = false;

function init_vertex_edges() {

    var cache_time = sessionStorage.getItem('sim_time');

    // console.log(cache_time);

    if (cache_time != null && sim_time > cache_time) {
        var cached_markers = JSON.parse(sessionStorage.getItem('markers'));
        console.log(cached_markers);
        if (cached_markers != null) {

            for (var i in cached_markers) {
                for (var j in cached_markers[i].plan) {
                    cached_markers[i].plan[j].start_node = dataset.nodes[get_node_by_id(cached_markers[i].plan[j].start_node.id)];
                    // console.log(cached_markers[i].plan[j].start_node);
                    cached_markers[i].plan[j].end_node = dataset.nodes[get_node_by_id(cached_markers[i].plan[j].end_node.id)];
                }
            }

            // console.log(cached_markers);
            dataset.markers = cached_markers;
        }
    } else {
        sessionStorage.clear();
    }


    force = d3.layout.force()
        .nodes(dataset.nodes)
        .links(dataset.edges)
        .size([w, h])
        .linkDistance([linkDistance])
        .charge([-500])
        .theta(0.1)
        .gravity(0.05)
        .start();

    force.links([]);

    setTimeout(function() {
        force.stop();
    }, 10000);

    var edges = svg.selectAll(".line")
        .data(dataset.edges)
        .enter()
        .append("line")
        .attr("id", function (d, i) {
            return 'edge' + i
        })
        .attr('marker-end', 'url(#arrowhead)')
        .style("stroke", "#222")
        .style("pointer-events", "none");

    var nodes = svg.selectAll(".circle")
        .data(dataset.nodes)
        .enter()
        .append("circle")
        .attr({
            "r": node_radius
        })
        .style("fill", function (d, i) {
            return color3(i);
        })
        .call(force.drag);

    nodes.on('click', function(d) {
        selected_city = d;
    });


    var nodelabels = svg.selectAll(".nodelabel")
        .data(dataset.nodes)
        .enter()
        .append("text")
        .attr({
            "x": function (d) {
                return d.x;
            },
            "y": function (d) {
                return d.y;
            },
            "class": "nodelabel",
            "stroke": "black"
        }).attr("text-anchor", "middle")
        .text(function (d) {
            return d.id;
        });

    nodelabels.on('click', function(d) {
        selected_city = d;
    });

    var edgepaths = svg.selectAll(".edgepath")
        .data(dataset.edges)
        .enter()
        .append('path')
        .attr({
            'd': function (d) {
                return 'M ' + d.source.x + ' ' + d.source.y + ' L ' + d.target.x + ' ' + d.target.y
            },
            'class': 'edgepath',
            'fill-opacity': 0,
            'stroke-opacity': 0,
            'fill': 'blue',
            'stroke': 'red',
            'id': function (d, i) {
                return 'edgepath' + i
            }
        })
        .style("pointer-events", "none");

    force.on("tick", function () {

        if (!force_timer_started) {

            force_timer_started = true;

            setTimeout(function(){
                force.stop();
                force_timer_started = false;
            }, 10000);
        }

        edges.attr({
            "x1": function (d) {
                return d.source.x;
            },
            "y1": function (d) {
                return d.source.y;
            },
            "x2": function (d) {
                return d.target.x;
            },
            "y2": function (d) {
                return d.target.y;
            }
        });

        nodes.attr({
            "cx": function (d) {
                return d.x;
            },
            "cy": function (d) {
                return d.y;
            }
        });

        nodelabels.attr("x", function (d) {
            return d.x;
        })
            .attr("y", function (d) {
                return d.y;
            });

        edgepaths.attr('d', function (d) {
            var path = 'M ' + d.source.x + ' ' + d.source.y + ' L ' + d.target.x + ' ' + d.target.y;
            //console.log(d)
            return path;
        });

        if (full_init) update_marker();

    });

//        marker_elements = svg.selectAll('.markers');

    var _markers = svg.selectAll('.markers').data(dataset.markers);
    markers = _markers
        .enter()
        .append('circle')
        .attr({
            "r": 10,
            "class": "markers"
        })
        .style("fill", function (d, i) {
            return colors(i);
        });

    _markers.exit().remove();

//        marker_text_elements = svg.selectAll('.marker_text');

    var _marker_text = svg.selectAll('.marker_text').data(dataset.markers);
    marker_text = _marker_text
        .enter()
        .append("text")
        .attr({
            "x": function (d) {
                return d.x;
            },
            "y": function (d) {
                return d.y;
            },
            "class": "marker_text",
            "stroke": "black"
        })
        .text(function (d) {
            return d.name;
        });

    _marker_text.exit().remove();

    // TODO
//        dataset.markers[0].plan[0].start_node = dataset.nodes[0];
//        dataset.markers[0].plan[0].end_node = dataset.nodes[1];
//        dataset.markers[0].plan[1].start_node = dataset.nodes[1];
//        dataset.markers[0].plan[1].end_node = dataset.nodes[0];

    // TODO

    full_init = true;
}

function update_marker_data() {

    sessionStorage.setItem('markers', JSON.stringify(dataset.markers));

    svg.selectAll('.markers').remove();

    var _markers = svg.selectAll('.markers').data(dataset.markers);

    _markers.exit().remove();

    markers = _markers
        .enter().append('circle').attr({
            "r": 10,
            "class": "markers"
        })
        .style("fill", function (d, i) {
            return colors(i);
        });

    svg.selectAll('.marker_text').remove();

    var _marker_text = svg.selectAll('.marker_text').data(dataset.markers);

    _marker_text.exit().remove();

    marker_text = _marker_text
        .enter().append('text')
        .attr({
            "x": function (d) {
                return d.x;
            },
            "y": function (d) {
                return d.y;
            },
            "class": "marker_text",
            "stroke": "black"
        })
        .text(function (d) {
            return d.name;
        });

}


//    {
//        name: "Marker_A",
//            plan: [
//        {
//            start_node: dataset.nodes[0],
//            end_node: dataset.nodes[1],
//            start_time: sim_time,
//            end_time: sim_time + 10
//        },
//        {
//            start_node: dataset.nodes[1],
//            end_node: dataset.nodes[0],
//            start_time: sim_time + 10,
//            end_time: sim_time + 20
//        }
//    ],
//        plan_index: 0
//    }

//    function update_marker() {}
function update_marker() {
    markers.attr('cx', function (d) {

//            console.log(d);

        if(d.good === undefined) {
            d.good = true;
        }

        if (d.good || d.plan_index < 0) {

            if (d.plan_index < 0) {
                d.plan_index = 0;
            }

//                console.log(d.plan);

            var t = true;

            while (d.plan[d.plan_index].start_time > sim_time) {
                --d.plan_index;
                if (d.plan_index < 0) {
                    t = false;
                    break;
                }
            }

            d.good = t;

            while (d.good && d.plan[d.plan_index].end_time < sim_time) {
                ++d.plan_index;
                if (d.plan_index >= d.plan.length) {
                    d.good = false;
                    break;
                }
            }
        }
        if (d.good) {
            var current_plan = d.plan[d.plan_index];
            d.x = current_plan.start_node.x + (sim_time - current_plan.start_time) / (current_plan.end_time - current_plan.start_time) * (current_plan.end_node.x - current_plan.start_node.x);
            return d.x;
        } else if (d.plan_index >= d.plan.length) {
            dataset.markers.splice(dataset.markers.indexOf(d), 1);
            update_marker_data();
        }

    }).attr('cy', function (d) {

        if (d.good) {

            var current_plan = d.plan[d.plan_index];

            d.y = current_plan.start_node.y + (sim_time - current_plan.start_time) / (current_plan.end_time - current_plan.start_time) * (current_plan.end_node.y - current_plan.start_node.y);

            return d.y;
        }

    });

    marker_text.attr('x', function (d) {
        return d.x;
    }).attr('y', function (d) {
        return d.y;
    });
}

var current_time_element = document.getElementById('current_time');

var time_d = new Date().getTime();

setInterval(function () {

    var n_time_d = new Date().getTime();

    if (full_init) update_marker();
    sim_time += (n_time_d - time_d) * time_multipler;

    time_d = n_time_d;

    var o = $("#form").serializeObject();
    var passenger_marker_index = get_marker_by_name(o.passenger);
    if (passenger_marker_index !== null) {
        var target_marker = dataset.markers[passenger_marker_index];
        if (target_marker.plan_index >= 0) {
            if (document.getElementsByName('from')[0].value != target_marker.plan[target_marker.plan_index].end_node.id) {
                document.getElementsByName('from')[0].value = target_marker.plan[target_marker.plan_index].end_node.id;
                $('.selectpicker[name="from"]').selectpicker('refresh');
            }
            if (target_marker.plan[target_marker.plan_index].end_node !== target_marker.plan[target_marker.plan_index].start_node) {
                $('#datetimepicker1').data("datetimepicker").date(new Date(target_marker.plan[target_marker.plan_index].end_time));
//                    document.getElementsByName('start_time')[0].value = target_marker.plan[target_marker.plan_index].end_time;
            } else {
//                    document.getElementsByName('start_time')[0].value = sim_time;
            }
        }
    } else {
//            document.getElementsByName('start_time')[0].value = sim_time;
    }
    current_time_element.innerHTML = new Date(sim_time);
}, 1);

setInterval(function() {
    if (full_init) {
        socket.send(JSON.stringify({
            "function": "sync_time"
        }));
    }

    sessionStorage.setItem('sim_time', sim_time);

}, 1000);

function new_schedule() {

}

function submit_form() {
    var o = $("#form").serializeObject();

    if (o.pass_by_vertex_display_name === undefined) {
        o.pass_by_vertex_display_name = [];
    }
    if (! (o.pass_by_vertex_display_name instanceof Array)) {
        o.pass_by_vertex_display_name = [o.pass_by_vertex_display_name];
    }
    o['function'] = "submit_passenger_requirement";

    if (o['start_time'] == "") {
        o['start_time'] = parseInt(new Date(sim_time + 10000).getTime());
    } else {
        o['start_time'] = parseInt(new Date(o['start_time']).getTime());
    }

    // o['total_time_limit'] = parseInt(o['total_time_limit']);
    if (o['total_time_limit'] == "") {
        o['total_time_limit'] = 0;
    } else {
        o['total_time_limit'] = parseInt(new Date(o['total_time_limit']).getTime());
    }

    if (o['start_time'] < new Date().getTime(sim_time)) {
        console.log("time invalid");
        return; // if before is a invalid request
    }

    if (o['total_time_limit'] > 0 && o['total_time_limit'] < new Date().getTime(sim_time)) {
        console.log("time invalid");
        return; // if before is a invalid request
    }

    if (o['passenger'] == "") {
        console.log("invalid id");
        return;
    }

    console.log(o);

    var target_marker_index = get_marker_by_name(o['passenger']);

    if (target_marker_index !== null) {
        while(dataset.markers[target_marker_index].plan.length - dataset.markers[target_marker_index].plan_index > 1) {
            dataset.markers[target_marker_index].plan.pop();
//              console.log(dataset.markers[target_marker_index]);

        }

        var current_plan = dataset.markers[target_marker_index].plan[dataset.markers[target_marker_index].plan_index];

        if (current_plan.start_node === current_plan.end_node) {
            dataset.markers[target_marker_index].plan.pop();
        }
    }

    socket.send(JSON.stringify(o));

}

var passenger_input_element = document.querySelector('[name="passenger"]');
