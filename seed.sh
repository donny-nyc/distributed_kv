#!/bin/bash

curl -X POST 0.0.0.0:9090/ -H 'content-type: text/html' --data-binary '@static/index.html'
curl -X PUT 0.0.0.0:9090/ -H 'content-type: text/html' --data-binary '@static/index2.html'
