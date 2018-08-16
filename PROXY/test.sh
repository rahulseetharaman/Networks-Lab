#!/bin/bash
g++ -std=c++11 NL/serverdir/server.cpp -o NL/serverdir/server
g++ -std=c++11 NL/proxydir/proxy.cpp -o NL/proxydir/proxy
g++ -std=c++11 NL/client.cpp -o NL/client
