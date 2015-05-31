# solarmonj
Automatically exported from code.google.com/p/solarmonj By Adam Gray & John Croucher.

Part of a patch supplied by [GoogleCodeExporter](https://github.com/jcroucher/solarmonj/issues/3) has been applied.

This project was started to address the lack of Linux software available for monitoring JFY solar inverters ( http://www.jfy-tech.com/ ).

Rather than recreating the full featured Windows software in Linux, we decided to develop a small app which can be run using cron to poll the inverter. From here the data can easily be parsed by other software keeping the actual monitoring code small and simple.

This software has been developed specifically for the Raspberry Pi ( raspberrypi.org ) but it should work in all Linux environments.

This project has been extended to send these logs to [Logstash](https://www.elastic.co/products/logstash), via a TCP/IP socket.

### Example Logstash Dashboard

![](https://raw.githubusercontent.com/benleov/solarmonj/master/images/kibana_dashboard.png)

### Requirements

To run this project you will need:

* SUNTWINS 5000TL Series Inverter
* RS232 to USB converter [e.g.](http://www.comsol.com.au/Products-by-Category/USB-Converters/USB2-DB9M-02)
* Some flavour of Linux

### Building

In order to build this project you will need:

* build-essentials 
* g++

### Setup

* Run make.
* Install [Logstash](https://www.elastic.co/products/logstash)
* Edit logstash.conf, placing the contents of /conf/logstash/logstash.conf 
* Install [Elastisearch](https://www.elastic.co/products/elasticsearch)
* Install [Kibana](https://www.elastic.co/products/kibana)
* Start all three applications.
* Run bin/jfycron as root to start sending to logstash.
* Browse to http://localhost:5601 to see your logs.


