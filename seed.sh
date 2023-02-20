#!/bin/bash

curl -X PUT 0.0.0.0:9090/ -H 'content-type: text/html' --data-binary '@static/index.html'
