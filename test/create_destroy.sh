#!/bin/bash

response="$(curl --silent -X POST 0.0.0.0:9090/ -H 'content-type: text/html' -d '<html><body><h1>Test</h1></body></html>')"

response="$(curl --silent 0.0.0.0:9090/)"

response="$(curl --silent -iX DELETE 0.0.0.0:9090/ | head -n1)"
echo -n "DELETE - "
if [ "$response" = 'HTTP/1.1 204 DELETED' ]
then
  echo "PASS"
else
  echo "FAIL: $response"
fi

response="$(curl --silent -iX DELETE 0.0.0.0:9090/ | head -n1)"
echo -n "DELETE (again) - "
if [ "$response" = 'HTTP/1.1 404 NOT FOUND' ]
then
  echo "PASS"
else
  echo "FAIL: $response"
fi

response=$(curl --silent 0.0.0.0:9090/ | jq '.message')
echo -n "GET (deleted record) - "
if [ "$response" = '"not found"' ]
then
  echo "PASS"
else
  echo "FAIL: $response"
fi

